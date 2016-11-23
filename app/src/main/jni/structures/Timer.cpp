//
// Created by alejandro on 31/10/16.
//

#include "headers/Timer.h"

Timer::Timer() {
    this->times = vector<Time>();
}

void Timer::addTime(const String& tag) {
    this->times.push_back(Time(tag));
}

void Timer::addDuration(const String& tag,const long long int& duration) {
    bool found = false;
    int i = 0;
    while(!found && i < this->times.size()){
        if(this->times[i].getTag() == tag){
            found = true;
        } else{
            i++;
        }
    }
    this->times[i].addDuration(duration);
}

void Timer::printTime(const String& tag) {
    bool found = false;
    int i = 0;
    while(!found && i < this->times.size()){
        if(this->times[i].getTag() == tag){
            found = true;
        } else{
            i++;
        }
    }
    this->times[i].printAverage();
}

void Timer::printTimes() const{
    for (int i = 0; i < this->times.size(); i++) {
        this->times[i].printAverage();
    }
}