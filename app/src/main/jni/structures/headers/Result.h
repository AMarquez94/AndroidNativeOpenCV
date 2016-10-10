//
// Created by alejandro on 10/10/16.
//

#ifndef ANDROIDNATIVEOPENCV_RESULT_H
#define ANDROIDNATIVEOPENCV_RESULT_H


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class Result {

private:
    String objectName;
    String viewName;
    int nPoints;
    int nMatches;

public:

    Result(String objectName, String viewName, int nPoints, int nMatches);
    String getObjectName();
    String getViewName();
    int getNPoints();
    int getNMatches();

};


#endif //ANDROIDNATIVEOPENCV_RESULT_H
