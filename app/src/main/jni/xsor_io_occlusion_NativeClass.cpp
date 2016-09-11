//
// Created by Samed on 10/15/2015.
//

#include "xsor_io_occlusion_NativeClass.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
JNIEXPORT jstring JNICALL Java_xsor_io_occlusion_NativeClass_getStringFromNative
        (JNIEnv * env, jclass clazz){

    Mat image;

    return env->NewStringUTF("Hello from JNI with OPENCV");
}