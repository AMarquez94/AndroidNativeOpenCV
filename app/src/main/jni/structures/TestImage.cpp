//
// Created by alejandro on 2/10/16.
//

#include "headers/TestImage.h"
#include "headers/utils.h"

using namespace std;
using namespace cv;

TestImage::TestImage(const String& name,const String& filename,const Mat& imageColor) {
    this->name = name;
    this->filename = filename;
    this->imageColor = imageColor;
    this->imageGray = Mat(imageColor.size(), CV_8UC1);
    cvtColor(this->imageColor, this->imageGray, CV_BGR2GRAY);
}

String TestImage::getName() const{
    return this->name;
}

String TestImage::getFileName() const{
    return this->filename;
}

Mat TestImage::getImageColor() const{
    return this->imageColor;
}

Mat TestImage::getImageGray() const{
    return this->imageGray;
}

void TestImage::setName(const String& name) {
    this->name = name;
}

void TestImage::setFileName(const String& filename) {
    this->filename = filename;
}

void TestImage::setImageColor(const Mat& imageColor) {
    this->imageColor = imageColor;
    this->imageGray = Mat(imageColor.size(), CV_8UC1);
    cvtColor(this->imageColor, this->imageGray, CV_BGR2GRAY);
}