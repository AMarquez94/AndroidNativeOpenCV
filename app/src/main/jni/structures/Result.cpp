//
// Created by alejandro on 10/10/16.
//

#include "headers/Result.h"

Result::Result(String objectName, String viewName, int nPoints, int nMatches) {
    this->objectName = objectName;
    this->viewName = viewName;
    this->nPoints = nPoints;
    this->nMatches = nMatches;
}

String Result::getObjectName() {
    return this->objectName;
}

String Result::getViewName() {
    return this->viewName;
}

int Result::getNPoints() {
    return this->nPoints;
}

int Result::getNMatches() {
    return this->nMatches;
}


