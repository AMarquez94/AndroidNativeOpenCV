//
// Created by alejandro on 2/10/16.
//

#include "headers/TestImage.h"
#include "headers/utils.h"

using namespace std;
using namespace cv;

TestImage::TestImage(String name, String filename, Mat imageColor) {
    this->name = name;
    this->filename = filename;
    this->imageColor = imageColor;
    this->imageGray = Mat(imageColor.size(), CV_8UC1);
    cvtColor(this->imageColor, this->imageGray, CV_BGR2GRAY);
}

String TestImage::getName() {
    return this->name;
}

String TestImage::getFileName() {
    return this->filename;
}

Mat TestImage::getImageColor() {
    return this->imageColor;
}

Mat TestImage::getImageGray() {
    return this->imageGray;
}

void TestImage::setName(String name) {
    this->name = name;
}

void TestImage::setFileName(String filename) {
    this->filename = filename;
}

void TestImage::setImageColor(Mat imageColor) {
    this->imageColor = imageColor;
    this->imageGray = Mat(imageColor.size(), CV_8UC1);
    cvtColor(this->imageColor, this->imageGray, CV_BGR2GRAY);
}