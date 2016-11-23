//
// Created by alejandro on 3/11/16.
//

#ifndef UNIZAR_MOBILECV_OBJECTDL_H
#define UNIZAR_MOBILECV_OBJECTDL_H

#include "Object.h"

using namespace std;
using namespace cv;

class ObjectDl : public Object {

private:

    /* Atribs */
    vector < vector<float> > viewsDescriptors;

public:

    /* Constructors */
    ObjectDl(const bool& null);
    ObjectDl(const String& name, const vector < vector<float> >& viewsDescriptors,
             const vector<String>& viewsNames, const vector < String >& allergens,const bool& easy);

    /* Methods */
    vector < vector <float> > getViewsDescriptors() const;
};


#endif //UNIZAR_MOBILECV_OBJECTDL_H
