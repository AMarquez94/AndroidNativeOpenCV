//
// Created by alejandro on 27/09/16.
//

#ifndef ANDROIDNATIVEOPENCV_OBJECT_H
#define ANDROIDNATIVEOPENCV_OBJECT_H

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace cv;

/* TODO: delete images */

class Object {

    private:

        /* Atribs */
        String name;
        vector <Mat> viewsImages;
        vector < vector<KeyPoint> > viewsKeyPoints;
        vector <Mat> viewsDescriptors;
        vector < vector<Point2f> > viewsCorners;
        vector < String > viewsNames;
        bool easy;
        bool null;

    public:

        /* Constructor */
        Object(bool null);
        Object(String name, vector <Mat> viewsImages, vector < vector<KeyPoint> > viewsKeyPoints, vector <Mat> viewsDescriptors,
           vector < vector<Point2f> > viewsCorners, vector < String > viewsNames, bool easy);

        /* Methods */
        String getName();
        vector <Mat> getViewsImages();
        vector < vector<KeyPoint> > getViewsKeyPoints();
        vector < Mat > getViewsDescriptors();
        vector < vector <Point2f> > getViewsCorners();
        vector < String > getViewsNames();
        bool isEasy();
        bool isNull();
        int getNumberOfViews();
};


#endif //ANDROIDNATIVEOPENCV_OBJECT_H
