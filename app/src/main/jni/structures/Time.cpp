//
// Created by alejandro on 31/10/16.
//

#include "headers/Time.h"

Time::Time() {

}

Time::Time(const String& tag) {
    this->tag = tag;
    this->iterations = 0;
    this->duration = 0;
    this->variance = 0;
}

Time::Time(const String& tag,const long long int& duration) {
    this->tag = tag;
    this->duration = duration;
    this->iterations = 1;
}

String Time::getTag() const{
    return this->tag;
}

long long int Time::getDuration() const{
    return this->duration;
}

int Time::getIterations() const{
    return this->iterations;
}

long long int Time::getAverage() const{
    if(this->iterations != 0){
        return this->duration/this->iterations;
    } else{
        return 0;
    }
}

float Time::getVariance() const {
    if(this->iterations != 0){
        return sqrt(this->variance/this->iterations);
    } else{
        return 0;
    }
}

void Time::addDuration(const long long int& newDuration) {
    float previousMedia;
    if(this->iterations == 0){
        previousMedia = 0;
    } else{
        previousMedia = (duration/this->iterations);
    }


    this->duration = this->duration + newDuration;
    this->iterations = this->iterations + 1;

    float newMedia = this->duration/this->iterations;
    this->variance = this->variance + (newDuration - previousMedia)*(newDuration - newMedia);
}

void Time::printAverage() const{
    log(this->tag, "Resultado " + longToString(this->duration) + " en " + intToString(this->getIterations()) +
            " iteraciones (Media: " + longToString(this->getAverage()) + " / varianza: " + floatToString(this->getVariance()) + ")");
}


