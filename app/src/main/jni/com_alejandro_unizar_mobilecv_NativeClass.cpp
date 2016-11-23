#include "com_alejandro_unizar_mobilecv_NativeClass.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "structures/headers/utils.h"
#include "structures/headers/ObjectKp.h"
#include "structures/headers/ObjectDl.h"
#include "structures/headers/Recognizer.h"
#include "structures/headers/Tester.h"
#include "structures/headers/TestImage.h"
#include "structures/headers/Network.h"
#include "structures/headers/Result.h"
#include "structures/headers/Timer.h"
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;

Recognizer recognizer;
Network network;
long long int totalDuration = 0.0;
int numberOfProcesing = 0;
Timer timer;

JNIEXPORT jboolean JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_ProcImage
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


JNIEXPORT jboolean JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_initRecognizer
        (JNIEnv * env, jclass clazz){

    /* Creates recognizer */
    recognizer = Recognizer("ORB","ORB","BruteForce-Hamming");

    timer = Timer();
    timer.addTime("LOCAL - Cargar objs");
    timer.addTime("LOCAL - Total");
    timer.addTime("REMOTE - Total");

    /* Adds objects */
    high_resolution_clock::time_point begin = high_resolution_clock::now();
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
    recognizer.createObject("/storage/emulated/0/TFG/BD/Aceite", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Axe", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Cesar_Heinz", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Enjuague", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Frenadol", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Gel", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/KH7", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Levadura", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Mayonesa", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Tonica", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Tortitas_arroz", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Aceite_corporal", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Aftersun", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Anis", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Betadine", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Cafe_hacendado", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Caldo", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Champu", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Chromecast", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Cicaderma", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Compeed", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Crema", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Emulsion", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Espuma", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Fideos", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Film", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Galletas_danesas", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Peroxiben", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Talco", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Ultima", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Cereales", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Aceite_botella", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Aceitunas", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Atun", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Esparragos", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Leche_hacendado", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Miel", true);
    recognizer.createObject("/storage/emulated/0/TFG/BD/Tomate_carrefour", true);
    /* Ends measuring time */
    high_resolution_clock::time_point end = high_resolution_clock::now();

    /* Added to total time */
    auto duration = duration_cast<microseconds>( end - begin ).count();
    timer.addDuration("LOCAL - Cargar objs", duration);



    int memorySize = 0;
    int memorySize2 = 0;
    int memorySize3 = 0;
    int keypointSize = 0;
    int k = 0;
    for(int i = 0; i < recognizer.getObjects().size(); i++){
        for(int j = 0; j < recognizer.getObjects()[i].getViewsDescriptors().size(); j++){
            memorySize = memorySize +
                    (recognizer.getObjects()[i].getViewsDescriptors()[j].rows) *
                            (recognizer.getObjects()[i].getViewsDescriptors()[j].cols);

            memorySize2 = memorySize2 +
                    (recognizer.getObjects()[i].getViewsDescriptors()[j].total()) *
                    (recognizer.getObjects()[i].getViewsDescriptors()[j].elemSize());

            memorySize3 = memorySize3 +
                    (recognizer.getObjects()[i].getViewsDescriptors()[j].step[0]) *
                    (recognizer.getObjects()[i].getViewsDescriptors()[j].rows);

            keypointSize = keypointSize +
                          (recognizer.getObjects()[i].getViewsKeyPoints().size()) * sizeof(KeyPoint);
            k++;
        }
    }
    memorySize = memorySize * 32;
    log("MEMORIA", intToString(memorySize) + " B");
    log("MEMORIA2", intToString(memorySize2) + " B");
    log("MEMORIA3", intToString(memorySize3) + " B");
    log("KEYPOINTSIZE", intToString(keypointSize));
    log("K", intToString(k));


    return true;
}

JNIEXPORT jboolean JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_FindObjects
        (JNIEnv * env, jclass clazz, jint width, jint height, jbyteArray yuv, jintArray bgra){

//    numberOfProcesing = numberOfProcesing + 1;

    high_resolution_clock::time_point begin = high_resolution_clock::now();

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
    Result result = recognizer.recognizeObject(mgray,mbgr,displayedFrame);

    // Converts from BGR (standard format) to BGRA (Android format)
    cvtColor(displayedFrame, mbgra, CV_BGR2BGRA);
    /* Image processing end */

    // Unlock native arrays
    env->ReleaseIntArrayElements(bgra, _bgra, 0);
    env->ReleaseByteArrayElements(yuv, _yuv, 0);

    /* Ends measuring time */
    high_resolution_clock::time_point end = high_resolution_clock::now();

    /* Added to total time */
    auto duration = duration_cast<microseconds>( end - begin ).count();
    timer.addDuration("LOCAL - Total", duration);
    return true;
}

JNIEXPORT void JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_doTest
        (JNIEnv * env, jclass clazz, jboolean remote){

    log("TESTER", "Starting tester");

    Tester tester = Tester(recognizer);
    tester.addImagesFromPath("/storage/emulated/0/TFG/Test");


    int iterations = 5;
    long long int totalTime;

    if(remote){
        log("TESTER", "Remote");
        totalTime = tester.doTestRemote("/storage/emulated/0/TFG/Resultados", iterations, network);
    } else{
        log("TESTER", "Local");
        totalTime = tester.doTest("/storage/emulated/0/TFG/Resultados", iterations);
    }

    log("DOTEST()", "Average time " + longToString(totalTime));
}

JNIEXPORT jboolean JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_startClient
        (JNIEnv * env, jclass clazz){

//    network = Network("192.168.1.20", 8888);
    network = Network("155.210.155.187", 80);
    network.startConnection();

}

JNIEXPORT jboolean JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_endClient
        (JNIEnv * env, jclass clazz){

    network.closeConnection();
}


JNIEXPORT jstring JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_sendRemote
        (JNIEnv * env, jclass clazz, jint width, jint height, jbyteArray yuv, jintArray bgra){

//    numberOfProcesing = numberOfProcesing + 1;

    high_resolution_clock::time_point begin = high_resolution_clock::now();

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

    Mat displayedFrame(mbgr.size(), CV_8UC3);
    displayedFrame = mbgr;
    Result result = Result();

    if(!network.sendImage(mbgr)){
        displayedFrame = mbgr;
    } else{
        if(!network.receiveImage(displayedFrame, result)){
            displayedFrame = mbgr;
        }
    }

    /* Ends measuring time */
    high_resolution_clock::time_point end = high_resolution_clock::now();

    /* Added to total time */
    auto duration = duration_cast<microseconds>( end - begin ).count();
    timer.addDuration("REMOTE - Total", duration);
//    totalDuration = totalDuration + duration;

    // Converts from BGR (standard format) to BGRA (Android format)
    cvtColor(displayedFrame, mbgra, CV_BGR2BGRA);
    /* Image processing end */

    // Unlock native arrays
    env->ReleaseIntArrayElements(bgra, _bgra, 0);
    env->ReleaseByteArrayElements(yuv, _yuv, 0);


    return env->NewStringUTF("");
}


JNIEXPORT void JNICALL Java_com_alejandro_unizar_1mobilecv_NativeClass_showStats
        (JNIEnv * env, jclass clazz){
    timer.printTimes();
    recognizer.printTimer();
    if(network.isInit()){
        network.printTimer();
    }
//    log("RESULTADOS", "Duracion total: " + longToString(totalDuration) + " en " + intToString(numberOfProcesing) + " iteraciones (Media: " + longToString(totalDuration/numberOfProcesing) + ")");
}

