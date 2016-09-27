//
// Created by alejandro on 27/09/16.
//

#include "headers/Recognizer.h"

using namespace std;
using namespace cv;

//Object createObject(String path, bool add);
//String RecognizeObject(Mat cameraImgGray, Mat cameraImgColour, Mat dstImg);

Recognizer::Recognizer(String descriptor, String extractor, String matcher){
    if(descriptor == "ORB"){
        this->descriptor = ORB::create();
    } else if(descriptor == "FAST"){
        /* TODO: implement */
    }

    if(extractor == "ORB"){
        this->extractor = ORB::create();
    } else if(extractor == "BRISK"){
        /* TODO: implement */
    }

    this->matcher = DescriptorMatcher::create(matcher);
}

Recognizer::Recognizer(String descriptor, String extractor, String matcher, vector<Object> objects){
    if(descriptor == "ORB"){
        this->descriptor = ORB::create();
    } else if(descriptor == "FAST"){
        /* TODO: implement */
    }

    if(extractor == "ORB"){
        this->extractor = ORB::create();
    } else if(extractor == "BRISK"){
        /* TODO: implement */
    }

    this->matcher = DescriptorMatcher::create(matcher);

    this->objects = objects;
}

Recognizer::Recognizer(String descriptor, vector<int> descriptorParams, String extractor,
                       vector<int> extractorParams, String matcher, vector<int> matcherParams){

    /* TODO: Implement */
}

Recognizer::Recognizer(String descriptor, vector<int> descriptorParams, String extractor,
                       vector<int> extractorParams, String matcher, vector<int> matcherParams,
                       vector<Object> objects){

    /* TODO: Implement */
}

/* Getters */

Ptr<FeatureDetector> Recognizer::getDescriptor(){
    return this->getDescriptor();
}


Ptr<FeatureExtractor> Recognizer::getExtractor(){
    return this->getExtractor();
}

Ptr<DescriptorMatcher> Recognizer::getMatcher(){
    return this->getMatcher();
}

/* Setters */

void Recognizer::setDescriptor(String descriptor) {
    if(descriptor == "ORB"){
        this->descriptor = ORB::create();
    } else if(descriptor == "FAST"){
        /* TODO: implement */
    }
}

void Recognizer::setExtractor(String extractor) {
    if(extractor == "ORB"){
        this->extractor = ORB::create();
    } else if(extractor == "BRISK"){
        /* TODO: implement */
    }
}

void Recognizer::setMatcher(String matcher) {
    this->matcher = DescriptorMatcher::create(matcher);
}


