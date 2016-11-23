//
// Created by alejandro on 20/10/16.
//

#ifndef UNIZAR_MOBILECV_NETWORK_H
#define UNIZAR_MOBILECV_NETWORK_H

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include "Result.h"
#include "utils.h"
#include "Timer.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>


#define BUF_LEN 65540

using namespace std;
using namespace cv;

class Network {

private:
    int sokt;
    const char* serverIP;
    int serverPort;
    Timer timer;
    bool open;
    bool deepLearning;

public:
    Network();
    Network(const char * serverIP,const int& serverPort);
    bool isInit() const;
    bool startConnection();
    bool closeConnection();
    bool sendImage(const Mat& image);
    bool receiveImage(Mat image, Result& result);
    bool sendInt(const int& n);
    int receiveInt();
    void printTimer();
    bool isOpen() const;
};


#endif //UNIZAR_MOBILECV_NETWORK_H
