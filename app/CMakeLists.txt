cmake_minimum_required(VERSION 3.4.1)
#定义变量ProjectRoot为工程根目录，用相对路径没通过编译，可能是路径写错，以后再试
#本次使用绝对路径作为参数
set(Project D:/GitHub/EchoCancellation)
#动态方式加载 speex是libxxxx.so的xxxx部分
add_library(speex SHARED IMPORTED)
#设置要连接的so的相对路径，${ANDROID_ABI}表示so文件的ABI类型的路径,这一步引入了动态加入编译的so
set_target_properties(speex PROPERTIES IMPORTED_LOCATION
                      ${Project}/app/src/main/jni/${ANDROID_ABI}/libspeex.so)
#配置加载native依赖
include_directories( ${Project}/app/src/main/jni/include )
include_directories( ${Project}/app/src/main/jni/libspeex )
#将native-lib加入到编译源中
add_library( # Sets the name of the library.
             native-lib
             SHARED
             src/main/cpp/native-lib.cpp
             src/main/cpp/org_faqrobot_map_echocancellation_Speex.h)


find_library( # Sets the name of the path variable.
              log-lib
              log )

#这里多出了引用的speex
target_link_libraries( # Specifies the target library.
                       native-lib
                       speex
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
