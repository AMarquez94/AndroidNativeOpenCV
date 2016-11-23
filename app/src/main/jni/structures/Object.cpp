//
// Created by alejandro on 2/11/16.
//

#include "headers/Object.h"

Object::Object(const bool &null) {
    this->null = null;
}

Object::Object(const String &name, const vector <String> &viewsNames,
               const vector <String> &allergens, const bool &easy) {
    this->name = name;
    this->viewsNames = viewsNames;
    this->allergens = allergens;
    this->easy = easy;
    this->null = false;
}

String Object::getName() const {
    return this->name;
}

vector < String > Object::getViewsNames() const {
    return this->viewsNames;
}

vector < String > Object::getAllergens() const{
    return this->allergens;
}

bool Object::isEasy() const{
    return this->easy;
}

bool Object::isNull() const{
    return this->null;
}

int Object::getNumberOfViews() const{
    return this->getViewsNames().size();
}
