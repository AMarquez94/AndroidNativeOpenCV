//
// Created by alejandro on 27/09/16.
//

#include "headers/Object.h"

using namespace std;
using namespace cv;

Object::Object(String name, vector < vector<KeyPoint> > keypoints, vector <Mat> descriptors,
               vector < vector<Point2f> > corners, bool easy){
    this->name = name;
    this->keypoints = keypoints;
    this->descriptors = descriptors;
    this->corners = corners;
    this->easy = easy;

    /* TODO: init views_name */
}

String Object::getName(){
    return this->name;
}

vector < vector<KeyPoint> > Object::getKeypoints(){
    return this->keypoints;
}

vector < Mat > Object::getDescriptors(){
    return this->descriptors;
}

vector < vector <Point2f> > Object::getCorners(){
    return this->corners;
}

bool Object::isEasy(){
    return this->easy;
}

/* TODO: get views_name */