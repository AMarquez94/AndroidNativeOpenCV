//
// Created by alejandro on 27/09/16.
//

#ifndef ANDROIDNATIVEOPENCV_OBJECTKP_H
#define ANDROIDNATIVEOPENCV_OBJECTKP_H

#include "Object.h"

using namespace std;
using namespace cv;

class ObjectKp : public Object {

private:

        /* Atribs */
        vector < vector<KeyPoint> > viewsKeyPoints;
        vector <Mat> viewsDescriptors;
        vector < vector<Point2f> > viewsCorners;

public:

        /* Constructor */
        ObjectKp(const bool& null);
        ObjectKp(const String& name,const vector < vector<KeyPoint> >& viewsKeyPoints,const vector <Mat>& viewsDescriptors,
           const vector < vector<Point2f> >& viewsCorners,const vector < String >& viewsNames,const vector < String >& allergens,const bool& easy);

        /* Methods */
        vector < vector<KeyPoint> > getViewsKeyPoints() const;
        vector < Mat > getViewsDescriptors() const;
        vector < vector <Point2f> > getViewsCorners() const;
};


#endif //ANDROIDNATIVEOPENCV_OBJECTKP_H
