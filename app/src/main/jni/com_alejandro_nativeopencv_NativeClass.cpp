#include "com_alejandro_nativeopencv_NativeClass.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <alien.cpp>
#include <features.cpp>

using namespace std;
using namespace cv;

Ptr<ORB> detector;
vector<KeyPoint> keypoints_object;
Mat descriptors_object;
Ptr<DescriptorMatcher> matcher;
vector<Point2f> obj_corners(4);

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
    detector = ORB::create();
    Mat image = imread("/storage/sdcard1/TFG/Fotos/caja.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    detector->detectAndCompute( image, Mat(), keypoints_object, descriptors_object );
    matcher = DescriptorMatcher::create("BruteForce-Hamming");
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint(image.cols,0);
    obj_corners[2] = cvPoint(image.cols, image.rows);
    obj_corners[3] = cvPoint(0, image.rows);
    return true;
}

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
    features(mgray, mbgr, displayedFrame, detector, matcher, keypoints_object, descriptors_object, obj_corners);

    // Pasa de BGR (formato estandar) a BGRA (formato Android)
    cvtColor(displayedFrame, mbgra, CV_BGR2BGRA);
    /* Finaliza el procesamiento de la imagen */

    // Desbloquear arrays nativas
    env->ReleaseIntArrayElements(bgra, _bgra, 0);
    env->ReleaseByteArrayElements(yuv, _yuv, 0);
    return true;
}