//
// Created by alejandro on 2/10/16.
//

#ifndef ANDROIDNATIVEOPENCV_TESTIMAGE_H
#define ANDROIDNATIVEOPENCV_TESTIMAGE_H


#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class TestImage {

private:

    String name;
    Mat imageColor;
    Mat imageGray;
public:

    /* Constructors */
    TestImage(String name, Mat imageColor);

    /* Getters */
    String getName();
    Mat getImageColor();
    Mat getImageGray();

    /* Setters */
    void setName(String name);
    void setImageColor(Mat imageColor);
};


#endif //ANDROIDNATIVEOPENCV_TESTIMAGE_H
