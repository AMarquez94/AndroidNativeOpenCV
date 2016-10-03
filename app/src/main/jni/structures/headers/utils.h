//
// Created by alejandro on 3/10/16.
//

#ifndef ANDROIDNATIVEOPENCV_UTILS_H
#define ANDROIDNATIVEOPENCV_UTILS_H

#include <opencv2/core/core.hpp>
#include <string>
#include <android/log.h>

using namespace std;
using namespace cv;

String intToString(int number);

String longToString(long long int number);

void log(String tag, String text);

void error(String tag, String text);



#endif //ANDROIDNATIVEOPENCV_UTILS_H
