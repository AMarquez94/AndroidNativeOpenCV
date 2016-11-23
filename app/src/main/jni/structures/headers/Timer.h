//
// Created by alejandro on 31/10/16.
//

#ifndef UNIZAR_MOBILECV_TIMER_H
#define UNIZAR_MOBILECV_TIMER_H

#include "utils.h"
#include "Time.h"

using namespace std;

class Timer {
private:

    vector<Time> times;

public:

    Timer();
    void addTime(const String& tag);
    void addDuration(const String& tag,const long long int& duration);
    void printTimes() const;
    void printTime(const String& tag);
};


#endif //UNIZAR_MOBILECV_TIMER_H
