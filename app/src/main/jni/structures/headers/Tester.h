//
// Created by alejandro on 2/10/16.
//

#ifndef ANDROIDNATIVEOPENCV_TESTER_H
#define ANDROIDNATIVEOPENCV_TESTER_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "TestImage.h"
#include "Recognizer.h"
#include "Network.h"

using namespace std;
using namespace cv;

class Tester {

private:
    Recognizer recognizer;
    vector <TestImage> images;
public:

    /* Constructors */
    Tester(const Recognizer& recognizer);

    /* Getters */
    Recognizer getRecognizer() const;
    vector<TestImage> getTestImages() const;

    /* Setters */
    void setRecognizer(const Recognizer& recognizer);
    void setImages(const vector<TestImage>& images);

    /* Methods */
    void addImage(const TestImage& image);
    void addImagesFromPath(const String& path);
    long long int doTest(const String& path, const int& iterations);
    long long int doTestRemote(const String& path, const int& iterations, Network network);
};


#endif //ANDROIDNATIVEOPENCV_TESTER_H
