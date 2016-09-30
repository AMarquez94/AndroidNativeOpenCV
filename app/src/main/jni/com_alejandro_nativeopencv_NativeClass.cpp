#include "com_alejandro_nativeopencv_NativeClass.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <android/log.h>
#include <alien.cpp>
#include <features.cpp>
#include "structures/headers/Object.h"
#include "structures/headers/Recognizer.h"

using namespace std;
using namespace cv;

Recognizer recognizer;

JNIEXPORT jboolean JNICALL Java_com_alejandro_nativeopencv_NativeClass_ProcImage
  (JNIEnv * env, jclass clazz, jint width, jint height, jbyteArray yuv, jintArray bgra){

    // Acceso nativo a las arrays
    jbyte* _yuv  = env->GetByteArrayElements(yuv, 0);
    jint*  _bgra = env->GetIntArrayElements(bgra, 0);

    // Input de la camara.
    // myuv -> imagen de la camara en su color nativo. Hay que pasarlo a BRGA.
    // mgray -> imagen de la camara directamente en GRAY
    Mat myuv(height + height/2, width, CV_8UC1, (unsigned char *)_yuv); // Wrapper around the _yuv data.
    Mat mgray(height, width, CV_8UC1, (unsigned char *)_yuv); // Also a wrapper around the _yuv data.

    // Output de la camara
    // mbgra -> imagen que se mostrara en la pantalla, en color BGRA (listo para Android).
    // Para procesarla con nuestro codigo, lo normal es que haya que convertirlo a BGR.
    Mat mbgra(height, width, CV_8UC4, (unsigned char *)_bgra);

    // Pasar de YUV420sp (formato camara) a BGR (estandar)
    Mat mbgr(height, width, CV_8UC3);   // Nuevo buffer de imagen
    cvtColor(myuv, mbgr, CV_YUV420sp2BGR);


    /* Comienza el procesamiento de la imagen */
    Mat displayedFrame(mbgra.size(), CV_8UC3);

    // Hace el procesamiento de la imagen.
//    displayedFrame = mbgr;
      alien(mbgr, displayedFrame, 1);

    // Pasa de BGR (formato estandar) a BGRA (formato Android)
    cvtColor(displayedFrame, mbgra, CV_BGR2BGRA);
    /* Finaliza el procesamiento de la imagen */

    // Desbloquear arrays nativas
    env->ReleaseIntArrayElements(bgra, _bgra, 0);
    env->ReleaseByteArrayElements(yuv, _yuv, 0);
    return true;
}


JNIEXPORT jboolean JNICALL Java_com_alejandro_nativeopencv_NativeClass_initRecognizer
        (JNIEnv * env, jclass clazz){

    __android_log_print(ANDROID_LOG_DEBUG, "HOLA", "Peta");

    recognizer = Recognizer("ORB","ORB","BruteForce-Hamming");
    recognizer.createObject("/storage/emulated/0/TFG/Fotos/Carpeta", true);
    recognizer.createObject("/storage/emulated/0/TFG/Fotos/Cereales", true);

    return true;
}

//    obj_names.push_back("teclado");
//    obj_names.push_back("carpeta");
//
//    for(int i = 0; i < obj_names.size(); i ++){
//        keypoints_objects.push_back(vector <KeyPoint>());
//        descriptors_objects.push_back(Mat());
//        objs_corners.push_back(vector <Point2f>(4));
//
//        Mat image = imread("/storage/sdcard1/TFG/Fotos/" + obj_names[i] + ".jpg", CV_LOAD_IMAGE_GRAYSCALE);
//        detector->detectAndCompute( image, Mat(), keypoints_objects[i], descriptors_objects[i] );
//        objs_corners[i][0] = cvPoint(0,0);
//        objs_corners[i][1] = cvPoint(image.cols,0);
//        objs_corners[i][2] = cvPoint(image.cols, image.rows);
//        objs_corners[i][3] = cvPoint(0, image.rows);
//    }
//    return true;

JNIEXPORT jboolean JNICALL Java_com_alejandro_nativeopencv_NativeClass_FindObjects
        (JNIEnv * env, jclass clazz, jint width, jint height, jbyteArray yuv, jintArray bgra){

    // Acceso nativo a las arrays
    jbyte* _yuv  = env->GetByteArrayElements(yuv, 0);
    jint*  _bgra = env->GetIntArrayElements(bgra, 0);

    // Input de la camara.
    // myuv -> imagen de la camara en su color nativo. Hay que pasarlo a BRGA.
    // mgray -> imagen de la camara directamente en GRAY
    Mat myuv(height + height/2, width, CV_8UC1, (unsigned char *)_yuv); // Wrapper around the _yuv data.
    Mat mgray(height, width, CV_8UC1, (unsigned char *)_yuv); // Also a wrapper around the _yuv data.

    // Output de la camara
    // mbgra -> imagen que se mostrara en la pantalla, en color BGRA (listo para Android).
    // Para procesarla con nuestro codigo, lo normal es que haya que convertirlo a BGR.
    Mat mbgra(height, width, CV_8UC4, (unsigned char *)_bgra);

    // Pasar de YUV420sp (formato camara) a BGR (estandar)
    Mat mbgr(height, width, CV_8UC3);   // Nuevo buffer de imagen
    cvtColor(myuv, mbgr, CV_YUV420sp2BGR);


    /* Comienza el procesamiento de la imagen */
    Mat displayedFrame(mbgra.size(), CV_8UC3);

    // Hace el procesamiento de la imagen.
//    displayedFrame = mgray;
    //alien(mbgr, displayedFrame, 1);
//    features(mgray, mbgr, displayedFrame, detector, matcher, keypoints_objects, descriptors_objects, objs_corners, obj_names);
    recognizer.RecognizeObject(mgray,mbgr,displayedFrame);

    // Pasa de BGR (formato estandar) a BGRA (formato Android)
    cvtColor(displayedFrame, mbgra, CV_BGR2BGRA);
    /* Finaliza el procesamiento de la imagen */

    // Desbloquear arrays nativas
    env->ReleaseIntArrayElements(bgra, _bgra, 0);
    env->ReleaseByteArrayElements(yuv, _yuv, 0);
    return true;
}