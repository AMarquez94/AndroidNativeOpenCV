//
// Created by alejandro on 27/09/16.
//

#include "headers/ObjectKp.h"

using namespace std;
using namespace cv;

ObjectKp::ObjectKp(const bool &null) : Object::Object(null){ }

ObjectKp::ObjectKp(const String& name,const vector < vector<KeyPoint> >& viewsKeyPoints,const vector <Mat>& viewsDescriptors,
               const vector < vector<Point2f> >& viewsCorners,const vector < String >& viewsNames,
               const vector < String >& allergens,const bool& easy)
    : Object::Object(name, viewsNames, allergens, easy)
{
    this->viewsKeyPoints = viewsKeyPoints;
    this->viewsDescriptors = viewsDescriptors;
    this->viewsCorners = viewsCorners;
}

vector < vector<KeyPoint> > ObjectKp::getViewsKeyPoints() const{
    return this->viewsKeyPoints;
}

vector < Mat > ObjectKp::getViewsDescriptors() const{
    return this->viewsDescriptors;
}

vector < vector <Point2f> > ObjectKp::getViewsCorners() const{
    return this->viewsCorners;
}