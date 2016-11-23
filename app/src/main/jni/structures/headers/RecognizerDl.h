//
// Created by alejandro on 3/11/16.
//

#ifndef UNIZAR_MOBILECV_RECOGNIZERDL_H
#define UNIZAR_MOBILECV_RECOGNIZERDL_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "ObjectDl.h"
#include "Result.h"

using namespace std;
using namespace cv;

class RecognizerDl {

private:
    vector<ObjectDl> objects;

public:
    RecognizerDl();
    RecognizerDl(const vector<ObjectDl>& objects);

    vector<ObjectDl> getObjects() const;

    void createObjects(const String& path, const bool& add);
    void deleteObjects();
    int getObjectIndex(const String& name);
    Result recognizeObject(const Mat& sceneImgColour, Mat dstImg);
};


#endif //UNIZAR_MOBILECV_RECOGNIZERDL_H
