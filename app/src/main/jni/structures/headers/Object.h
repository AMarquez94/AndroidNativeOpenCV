//
// Created by alejandro on 2/11/16.
//

#ifndef UNIZAR_MOBILECV_OBJECT_H
#define UNIZAR_MOBILECV_OBJECT_H

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;
using namespace cv;

class Object {

    private:

        /* Atribs */
        String name;
        vector < String > viewsNames;
        vector < String> allergens;
        bool easy;
        bool null;

    public:

        /* Constructor */
        Object(const bool& null);
        Object(const String& name, const vector < String >& viewsNames,const vector < String >& allergens, const bool& easy);

        /* Methods */
        String getName() const;
        vector < String > getViewsNames() const;
        vector < String > getAllergens() const;
        bool isEasy() const;
        bool isNull() const;
        int getNumberOfViews() const;
};


#endif //UNIZAR_MOBILECV_OBJECT_H
