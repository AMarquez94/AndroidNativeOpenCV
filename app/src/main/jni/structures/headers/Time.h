//
// Created by alejandro on 31/10/16.
//

#ifndef UNIZAR_MOBILECV_TIME_H
#define UNIZAR_MOBILECV_TIME_H

#include "utils.h"

using namespace std;

class Time {
private:
    String tag;
    long long int duration;
    int iterations;
    float variance;

public:

    Time();
    Time(const String& tag);
    Time(const String& tag,const long long int& duration);
    String getTag() const;
    long long int getDuration() const;
    int getIterations() const;
    void addDuration(const long long int& newDuration);
    long long int getAverage() const;
    float getVariance() const;
    void printAverage() const;

};


#endif //UNIZAR_MOBILECV_TIME_H
