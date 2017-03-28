package org.faqrobot.map.echocancellation;

/**
 * @author 候帅
 *         Created by 候帅 on 2017/3/28. 13:34
 */

public class Speex {
    static {
        System.loadLibrary("native-lib");
    }


    public native int open(int compression);

    public native int getFrameSize();

    public native int decode(byte encoded[], short lin[], int size);

    public native int encode(short lin[], int offset, byte encoded[], int size);

    public native void close();

}
