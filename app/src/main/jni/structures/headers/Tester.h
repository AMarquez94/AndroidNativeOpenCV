//
// Created by alejandro on 2/10/16.
//

#ifndef ANDROIDNATIVEOPENCV_TESTER_H
#define ANDROIDNATIVEOPENCV_TESTER_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "TestImage.h"
#include "Recognizer.h"

using namespace std;
using namespace cv;

class Tester {

private:
    Recognizer recognizer;
    vector <TestImage> images;
public:

    /* Constructors */
    Tester(Recognizer recognizer);

    /* Getters */
    Recognizer getRecognizer();
    vector<TestImage> getTestImages();

    /* Setters */
    void setRecognizer(Recognizer recognizer);
    void setImages(vector<TestImage> images);

    /* Methods */
    void addImage(TestImage image);
    void addImagesFromPath(String path);
    long long int doTest(String path, int iterations);
};


#endif //ANDROIDNATIVEOPENCV_TESTER_H
