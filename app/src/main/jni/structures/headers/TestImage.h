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
    String filename;
    Mat imageColor;
    Mat imageGray;
public:

    /* Constructors */
    TestImage(const String& name, const String& filename, const Mat& imageColor);

    /* Getters */
    String getName() const;
    String getFileName() const;
    Mat getImageColor() const;
    Mat getImageGray() const;

    /* Setters */
    void setName(const String& name);
    void setFileName(const String& filename);
    void setImageColor(const Mat& imageColor);
};


#endif //ANDROIDNATIVEOPENCV_TESTIMAGE_H
