//
// Created by alejandro on 10/10/16.
//

#ifndef ANDROIDNATIVEOPENCV_RESULT_H
#define ANDROIDNATIVEOPENCV_RESULT_H


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "ProtoResult.pb.h"

using namespace std;
using namespace cv;

class Result {

private:
    String objectName;
    String viewName;
    int nPoints;
    int nMatches;
    vector<Point2f> corners;
    vector<String> allergens;

public:

    Result();
    Result(const String& objectName,const String& viewName,const int& nPoints,const int& nMatches,
           const vector<Point2f>& corners,const vector<String>& allergens);
    String getObjectName() const;
    String getViewName() const;
    int getNPoints() const;
    int getNMatches() const;
    vector<Point2f> getCorners() const;
    vector<String> getAllergens() const;
    bool serializeResult(string* output);
    bool deserializeResult(const string& data);

//    bool SerializeToString(string* output) const;
//    bool ParseFromString(const string& data);

};


#endif //ANDROIDNATIVEOPENCV_RESULT_H
