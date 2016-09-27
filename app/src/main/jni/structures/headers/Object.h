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

class Object {

    private:

        /* Atribs */
        String name;
        vector < vector<KeyPoint> > keypoints;
        vector <Mat> descriptors;
        vector < vector<Point2f> > corners;
        vector < String > views_name;
        bool easy;

    public:

        /* Constructor */
        Object(String name, vector < vector<KeyPoint> > keypoints, vector <Mat> descriptors,
           vector < vector<Point2f> > corners, bool easy);

        /* Methods */
        String getName();
        vector < vector<KeyPoint> > getKeypoints();
        vector < Mat > getDescriptors();
        vector < vector <Point2f> > getCorners();
        bool isEasy();

};


#endif //ANDROIDNATIVEOPENCV_OBJECT_H
