//
// Created by alejandro on 27/09/16.
//

#include "headers/Object.h"

using namespace std;
using namespace cv;

Object::Object(bool null) {
    this->null = null;
}

Object::Object(String name, vector < vector<KeyPoint> > viewsKeyPoints, vector <Mat> viewsDescriptors,
               vector < vector<Point2f> > viewsCorners, vector <String> viewsName, bool easy){
    this->name = name;
    this->viewsKeyPoints = viewsKeyPoints;
    this->viewsDescriptors = viewsDescriptors;
    this->viewsCorners = viewsCorners;
    this->viewsNames = viewsName;
    this->easy = easy;
    this->null = false;

    /* TODO: init views_name */
}

String Object::getName(){
    return this->name;
}

vector < vector<KeyPoint> > Object::getViewsKeyPoints(){
    return this->viewsKeyPoints;
}

vector < Mat > Object::getViewsDescriptors(){
    return this->viewsDescriptors;
}

vector < vector <Point2f> > Object::getViewsCorners(){
    return this->viewsCorners;
}

vector < String > Object::getViewsNames() {
    return this->viewsNames;
}

bool Object::isEasy(){
    return this->easy;
}

bool Object::isNull(){
    return this->null;
}

int Object::getNumberOfViews(){
    return this->getViewsKeyPoints().size();
}

/* TODO: get views_name */