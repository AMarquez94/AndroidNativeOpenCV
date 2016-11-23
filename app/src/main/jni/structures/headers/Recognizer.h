//
// Created by alejandro on 27/09/16.
//

#ifndef ANDROIDNATIVEOPENCV_RECOGNIZER_H
#define ANDROIDNATIVEOPENCV_RECOGNIZER_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "ObjectKp.h"
#include "Result.h"
#include "Timer.h"

using namespace std;
using namespace cv;

class Recognizer {

private:
    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;
    Ptr<DescriptorMatcher> matcher;
    vector<ObjectKp> objects;
    double matcherDistanceFilter;
    Timer timer;
public:

    /* Constructors */
    Recognizer();
    Recognizer(const String& detector,const String& extractor,const String& matcher);
    Recognizer(const String& detector,const String& extractor,const String& matcher,
               const vector<ObjectKp>& objects);
    Recognizer(const String& detector,const vector<float>& detectorParams,const String& extractor,
               const vector<float>& extractorParams,const String& matcher,const double& matcherDistance);
    Recognizer(const String& detector,const vector<float>& detectorParams,const String& extractor,
               const vector<float>& extractorParams,const String& matcher,const double& matcherDistance,
               const vector<ObjectKp>& objects);

    /* Getters */
    Ptr<FeatureDetector> getDescriptor() const;
    Ptr<DescriptorExtractor> getExtractor() const;
    Ptr<DescriptorMatcher> getMatcher() const;
    vector<ObjectKp> getObjects() const;

    /* Setters */
    void setDetector(const String& detector);
    void setExtractor(const String& extractor);
    void setMatcher(const String& matcher);
    void setObjects(const vector<ObjectKp>& objects);

    /* Methods */
    ObjectKp createObject(const String& path,const bool& add);
    void deleteObjects();
    int getObjectIndex(const String& name);
    Result recognizeObject(const Mat& sceneImgGray,const Mat& sceneImgColour, Mat dstImg);
    void printTimer();

};


#endif //ANDROIDNATIVEOPENCV_RECOGNIZER_H
