//
// Created by alejandro on 3/11/16.
//

#include "headers/ObjectDl.h"

using namespace std;
using namespace cv;

ObjectDl::ObjectDl(const bool &null) : Object::Object(null) { }

ObjectDl::ObjectDl(const String &name, const vector <vector<float>> &viewsDescriptors,
                   const vector <String> &viewsNames, const vector <String> &allergens,
                   const bool &easy)
    : Object::Object(name, viewsNames, allergens, easy)
{
    this->viewsDescriptors = viewsDescriptors;
}

vector < vector <float> > ObjectDl::getViewsDescriptors() const {
    return this->viewsDescriptors;
}
