#include "com_alejandro_nativeopencv_NativeClass.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
JNIEXPORT jstring JNICALL Java_com_alejandro_nativeopencv_NativeClass_getStringFromNative
  (JNIEnv * env, jclass clazz){

    Mat image;

    return env->NewStringUTF("Hola Tontiña");
}