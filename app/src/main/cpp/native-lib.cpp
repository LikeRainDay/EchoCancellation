#include <jni.h>
#include <string>
#include <include/speex/speex.h>
#include <unistd.h>

#include "org_faqrobot_map_echocancellation_Speex.h"



static int codec_open=0;

static int dec_frame_size;
static int enc_frame_size;


static SpeexBits ebits,dbits;
void *enc_state;
void *dec_state;

static JavaVM *gJavaVm;


/*
 * Class:     org_faqrobot_map_echocancellation_Speex
 * Method:    open
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_org_faqrobot_map_echocancellation_Speex_open
  (JNIEnv *env, jobject obj, jint compression){
    int temp;

    if(codec_open++!=0)
        return (jint)0;
    speex_bits_init(&ebits)
    speex_bits_init(&dbits)

    enc_state=speex_encoder_init(&speex_nb_mode);
    dec_state=speex_decoder_init(&speex_nb_mode);
    tmp=compression;
    speex_encoder_ctl(enc_state,SPEEX_SET_QUALITY,&tmp);
    speex_encoder_ctl(enc_state,SPEEX_GET_FRAME_SIZE,&enc_frame_size);
    speex_encoder_ctl(dec_state,SPEEX_GET_FRAME_SIZE,&dec_frame_size);
    return (jint)0;
  }

/*
 * Class:     org_faqrobot_map_echocancellation_Speex
 * Method:    getFrameSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_faqrobot_map_echocancellation_Speex_getFrameSize
  (JNIEnv *env, jobject obj){

    if(!codec_open)
        return 0;
    return (jint)enc_frame_size;
  }

/*
 * Class:     org_faqrobot_map_echocancellation_Speex
 * Method:    decode
 * Signature: ([B[SI)I
 */
JNIEXPORT jint JNICALL Java_org_faqrobot_map_echocancellation_Speex_decode
  (JNIEnv *env, jobject obj, jbyteArray encoded, jshortArray lin, jint size){


    jbyte buffer[dec_frame_size];
    jshort output_buffer[dec_frame_size];
    jsize encoded_length = size;

    if (!codec_open)
         return 0;

    env->GetByteArrayRegion(encoded, 0, encoded_length, buffer);
    speex_bits_read_from(&dbits, (char *)buffer, encoded_length);
    speex_decode_int(dec_state, &dbits, output_buffer);
    env->SetShortArrayRegion(lin, 0, dec_frame_size,
                 output_buffer);

    return (jint)dec_frame_size;
  }

/*
 * Class:     org_faqrobot_map_echocancellation_Speex
 * Method:    encode
 * Signature: ([SI[BI)I
 */
JNIEXPORT jint JNICALL Java_org_faqrobot_map_echocancellation_Speex_encode
  (JNIEnv *env, jobject obj, jshortArray lin, jint offset, jbyteArray encoded, jint size){
       jshort buffer[enc_frame_size];
       jbyte output_buffer[enc_frame_size];
       int nsamples=(size-1)/enc_frame_size+1;
       int i,tot_bytes=0;

       if(!codec_open)
            return 0;

       speex_bits_reset(&ebits);

       for(i=0;i<nsamples;i++){
           env->GetShortArrayRegion(lin,offset+i*enc_frame_size,enc_frame_size,buffer);
           speex_encode_init(enc_state,buffer,&ebits);
       }

       tot_bytes=speex_bits_write(&ebits,(char *)output_buffer,enc_frame_size);
       env->SetByteArrayRegion(encoded,0,tot_bytes,output_buffer);
       return (jint)tot_bytes;
  }

/*
 * Class:     org_faqrobot_map_echocancellation_Speex
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_faqrobot_map_echocancellation_Speex_close
  (JNIEnv * env, jobject obj){

   if(--codec_open!=0){
    return;
   }
   speex_bits_destroy(&ebits);
   speex_bits_destroy(&dbits);
   speex_decoder_destroy(dec_state);
   speex_encoder_destroy(enc_state);
  }

