//
// Created by alejandro on 3/10/16.
//

#include "headers/utils.h"

using namespace std;
using namespace cv;

String intToString(int number){
    string Result;//string which will contain the result

    stringstream convert; // stringstream used for the conversion

    convert << number;//add the value of Number to the characters in the stream

    return convert.str();
}

String longToString(long long int number){
    string Result;

    stringstream convert;

    convert << number;

    return convert.str();
}

void log(String tag, String text){
    __android_log_print(ANDROID_LOG_DEBUG, tag.c_str(), text.c_str());
}

void error(String tag, String text){
    __android_log_print(ANDROID_LOG_ERROR, tag.c_str(), text.c_str());
}