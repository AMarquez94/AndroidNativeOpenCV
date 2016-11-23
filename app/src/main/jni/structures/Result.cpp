//
// Created by alejandro on 10/10/16.
//

#include "headers/Result.h"
#include "headers/utils.h"

Result::Result() {

}

Result::Result(const String& objectName,const String& viewName,const int& nPoints,const int& nMatches,
               const vector<Point2f>& corners,const vector<String>& allergens){
    this->objectName = objectName;
    this->viewName = viewName;
    this->nPoints = nPoints;
    this->nMatches = nMatches;
    this->corners = corners;
    this->allergens = allergens;
}

String Result::getObjectName() const{
    return this->objectName;
}

String Result::getViewName() const{
    return this->viewName;
}

int Result::getNPoints() const{
    return this->nPoints;
}

int Result::getNMatches() const{
    return this->nMatches;
}

vector<Point2f> Result::getCorners() const{
    return this->corners;
}

vector<String> Result::getAllergens() const{
    return this->allergens;
}

bool Result::serializeResult(string *output) {
    log("RESULT", "Serializing...");
    ProtoResult protoResult;
    protoResult.set_objectname(objectName);
    protoResult.set_viewname(viewName);
    protoResult.set_npoints(nPoints);
    protoResult.set_nmatches(nMatches);
    if(objectName != "No object" && this->corners.size() == 4){
        log("RESULT", "Is an object...");
        protoResult.set_x1(corners[0].x);
        protoResult.set_y1(corners[0].y);
        protoResult.set_x2(corners[1].x);
        protoResult.set_y2(corners[1].y);
        protoResult.set_x3(corners[2].x);
        protoResult.set_y3(corners[2].y);
        protoResult.set_x4(corners[3].x);
        protoResult.set_y4(corners[3].y);
    }
    for(int i = 0; i < this->allergens.size(); i++){
        protoResult.add_allergens(this->allergens[i]);
    }
    return protoResult.SerializeToString(output);
}

bool Result::deserializeResult(const string& data) {
    log("RESULT", "Deserializing...");

    ProtoResult protoResult;

    bool success = protoResult.ParseFromString(data);

    if(success){
        log("RESULT", "Success in deserializing");
    } else{
        error("RESULT", "Failed to deserialize...");
    }

    this->objectName = protoResult.objectname();
    this->viewName = protoResult.viewname();
    this->nPoints = protoResult.npoints();
    this->nMatches = protoResult.nmatches();
    if(protoResult.has_x1()){
        log("RESULT", "Is an object...");
        this->corners = vector<Point2f>(4);
        corners[0] = Point2f(protoResult.x1(), protoResult.y1());
        corners[1] = Point2f(protoResult.x2(), protoResult.y2());
        corners[2] = Point2f(protoResult.x3(), protoResult.y3());
        corners[3] = Point2f(protoResult.x4(), protoResult.y4());
    }

    this->allergens = vector<String>(protoResult.allergens_size());
    for (int i = 0; i < protoResult.allergens_size(); ++i) {
        this->allergens[i] = protoResult.allergens(i);
    }
    return success;
}


