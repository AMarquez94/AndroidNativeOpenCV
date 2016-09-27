//
// Created by alejandro on 27/09/16.
//

#ifndef ANDROIDNATIVEOPENCV_RECOGNIZER_H
#define ANDROIDNATIVEOPENCV_RECOGNIZER_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <android/log.h>
#include "Object.h"

using namespace std;
using namespace cv;

class Recognizer {

private:
    Ptr<FeatureDetector> descriptor;
    Ptr<FeatureExtractor> extractor;
    Ptr<DescriptorMatcher> matcher;
    vector<Object> objects;
public:

    /* Constructors */
    Recognizer(String descriptor, String extractor, String matcher);
    Recognizer(String descriptor, String extractor, String matcher, vector<Object> objects);
    Recognizer(String descriptor, vector<int> descriptorParams, String extractor,
               vector<int> extractorParams, String matcher, vector<int> matcherParams);
    Recognizer(String descriptor, vector<int> descriptorParams, String extractor,
               vector<int> extractorParams, String matcher, vector<int> matcherParams,
               vector<Object> objects);

    /* Getters */
    Ptr<FeatureDetector> getDescriptor();
    Ptr<FeatureExtractor> getExtractor();
    Ptr<DescriptorMatcher> getMatcher();

    /* Setters */
    void setDescriptor(String descriptor);
    void setExtractor(String extractor);
    void setMatcher(String matcher);

    /* Methods */
    Object createObject(String path, bool add);
    String RecognizeObject(Mat cameraImgGray, Mat cameraImgColour, Mat dstImg);

};


#endif //ANDROIDNATIVEOPENCV_RECOGNIZER_H
