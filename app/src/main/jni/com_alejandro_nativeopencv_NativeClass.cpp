#include "com_alejandro_nativeopencv_NativeClass.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "structures/headers/utils.h"
#include "structures/headers/Object.h"
#include "structures/headers/Recognizer.h"
#include "structures/headers/Tester.h"
#include "structures/headers/TestImage.h"

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
    displayedFrame = mbgr;

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

    /* Creates recognizer */
    recognizer = Recognizer("FAST","ORB","BruteForce-Hamming");

    /* Adds objects */
    recognizer.createObject("/storage/emulated/0/TFG/BD/Asturiana", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Ariel", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Cafe", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Celta", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Nocilla", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Orlando", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Agua", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Ambar", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Licor43", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Manocao", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Nescafe", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Sal_Hacendado", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Tortitas_arroz", true);

    return true;
}

JNIEXPORT jboolean JNICALL Java_com_alejandro_nativeopencv_NativeClass_FindObjects
        (JNIEnv * env, jclass clazz, jint width, jint height, jbyteArray yuv, jintArray bgra){

    // Native access to image arrays
    jbyte* _yuv  = env->GetByteArrayElements(yuv, 0);
    jint*  _bgra = env->GetIntArrayElements(bgra, 0);

    // Camera input.
    // myuv -> camera image in its native colour. Has to be converted to BGRA.
    // mgray -> camera image in GRAY mode.
    Mat myuv(height + height/2, width, CV_8UC1, (unsigned char *)_yuv); // Wrapper around the _yuv data.
    Mat mgray(height, width, CV_8UC1, (unsigned char *)_yuv); // Also a wrapper around the _yuv data.

    // Camera output
    // mbgra -> image to be shown on screen. BGRA mode (ready for Android)
    // To process it in our code, it should be converted to BGR (usually)
    Mat mbgra(height, width, CV_8UC4, (unsigned char *)_bgra);

    // Convert image from YUV420sp (camera format) to BGR (standard)
    Mat mbgr(height, width, CV_8UC3);   // New image buffer
    cvtColor(myuv, mbgr, CV_YUV420sp2BGR);


    /* Image processing start */
    Mat displayedFrame(mbgra.size(), CV_8UC3);

    // Makes image processing.
    recognizer.RecognizeObject(mgray,mbgr,displayedFrame);
//    displayedFrame = mgray;
    //alien(mbgr, displayedFrame, 1);
//    features(mgray, mbgr, displayedFrame, detector, matcher, keypoints_objects, descriptors_objects, objs_corners, obj_names);

    // Converts from BGR (standard format) to BGRA (Android format)
    cvtColor(displayedFrame, mbgra, CV_BGR2BGRA);
    /* Image processing end */

    // Unlock native arrays
    env->ReleaseIntArrayElements(bgra, _bgra, 0);
    env->ReleaseByteArrayElements(yuv, _yuv, 0);
    return true;
}

JNIEXPORT void JNICALL Java_com_alejandro_nativeopencv_NativeClass_doTest
        (JNIEnv * env, jclass clazz){

    log("TESTER", "Starting tester");

    Tester tester = Tester(recognizer);
    tester.addImagesFromPath("/storage/emulated/0/TFG/Test");

    int iterations = 5;
    long long int totalTime = tester.doTest("/storage/emulated/0/TFG/Resultados", iterations);

    log("DOTEST()", "Average time " + longToString(totalTime));
}