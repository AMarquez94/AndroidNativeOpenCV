//
// Created by alejandro on 3/11/16.
//

#include "headers/RecognizerDl.h"
#include "headers/utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

using namespace std;
using namespace cv;

RecognizerDl::RecognizerDl() {
    this->objects = vector<ObjectDl>();
}

RecognizerDl::RecognizerDl(const vector <ObjectDl> &objects) {
    this->objects = objects;
}

void RecognizerDl::createObjects(const String &path, const bool &add) {

    /* Read mat of floats */
    Mat objectsDescriptors;
    FileStorage fs((path + "/feats.xml").c_str(), FileStorage::READ);
    fs["FeatureMat"] >> objectsDescriptors;

    if(!objectsDescriptors.isContinuous()){
    	objectsDescriptors = objectsDescriptors.clone();
    }

    ifstream file;
    file.open((path + "/info_list.txt").c_str());
    if(!file.is_open()){
        error("RECOGNIZER_DL", "error al abrir ficheros info");
    } else{

        string object_path;
        int row = 0;
        while(file >> object_path){

            ifstream object;
            object.open((path + "/" + object_path + "/info.txt").c_str());
            if(!object.is_open()){
                error("RECOGNIZER_DL", "error al abrir objeto");
            } else{
                String name;
                vector < vector<float> > descriptors;
                vector < String > viewNames;
                vector < String > allergens;
                bool easy;

                string word;
                int j = 0;

                int numVistas;
                while(object >> word){
                    if(j == 0){

                        /* Object name */
                        name = word;

                        log("NOMBRE", name);

                    } else if(j == 1){

                        /* Object easy or not */
                        easy = (word == "true");
                        log("EASY", intToString(easy));
                    } else if(j == 2){

                        numVistas = atoi(word.c_str());
                        log("NUM_VISTAS", intToString(numVistas));

                    } else if(j < (3 + 2 * numVistas) && j%2 == 1){
                    	vector<float> out;
                    	out.insert(out.end(), objectsDescriptors.ptr<float>(row), objectsDescriptors.ptr<float>(row) + objectsDescriptors.cols);
                        descriptors.push_back(out);
                        row++;
                        log("ROW", intToString(row));
                        log("VISTA", intToString((j/2)-1));
                    } else if(j < (3 + 2 * numVistas) && j%2 == 0){

                        /* Object name view */
                        viewNames.push_back(word);

                        String aux = word;
                        log("VISTA NOMBRE", aux);
                    } else{
                        allergens.push_back(word);
                        String aux = word;
                        log("ALERGENO", aux);
                    }
                    j++;
                }

                ObjectDl obj = ObjectDl(name, descriptors, viewNames, allergens, easy);
                this->objects.push_back(obj);
                object.close();
            }
        }

        file.close();
    }
}

void RecognizerDl::deleteObjects() {
    this->objects.clear();
}

int RecognizerDl::getObjectIndex(const String &name) {
    unsigned int i = 0;
    bool found = false;
    while(i < this->objects.size() && !found){
        if(this->objects[i].getName() == name){
            found = true;
        } else{
            i++;
        }
    }

    if(!found){
        return -1;
    } else{
        return i;
    }
}

Result RecognizerDl::recognizeObject(const Mat &sceneImgColour, Mat dstImg) {

    /*TODO Aplicar Deep Learning a la imagen */
    vector<float> sceneImgAttributes;

    int bestMatchObject = -1;
    int bestMatchview = -1;
    float bestDistance = numeric_limits<float>::max();

    if(sceneImgAttributes.size() != 4096) {

        for (unsigned int i = 0; i < this->objects.size(); i++) {
            /* Iterating over object array */


            for (int j = 0; j < this->objects[i].getNumberOfViews(); j++) {
                /* Iterating over object views */

                float euclideanDistance = 0;
                for(unsigned int k = 0; k < sceneImgAttributes.size(); k++){
                    float substraction = sceneImgAttributes[k] - this->objects[i].getViewsDescriptors()[j][k];
                    float squared = substraction * substraction;
                    euclideanDistance = euclideanDistance + squared;
                }
                euclideanDistance = sqrt(euclideanDistance);
                if(euclideanDistance < bestDistance){
                    bestDistance = euclideanDistance;
                    bestMatchObject = i;
                    bestMatchview = j;
                }
            }
        }
    }

    bool isMatch = bestDistance < numeric_limits<float>::max(); //TODO some kind of filter

    String objectName;
    String viewName;
    vector<String> allergens;
    vector<Point2f> scene_corners;

    sceneImgColour.copyTo(dstImg);

    if(isMatch){
        objectName = this->objects[bestMatchObject].getName();
        viewName = this->objects[bestMatchObject].getViewsNames()[bestMatchview];
        allergens = this->objects[bestMatchObject].getAllergens();
    } else{
        objectName = "No object";
        viewName = "";
        allergens = vector<String>(0);
    }

    /* Write object name */
    Size textsize = getTextSize(objectName, FONT_HERSHEY_COMPLEX, 1, 2, 0);

    Point org((640 - textsize.width - 20), textsize.height + 20);
    putText( dstImg, objectName, org, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0), 2);

    for(unsigned int i = 0; i < allergens.size(); i++){
        textsize = getTextSize(allergens[i], FONT_HERSHEY_COMPLEX, 0.75, 2, 0);
        Point org((640 - textsize.width - 20), textsize.height + 20 + 30*(i+1));

        /* Comprobar con lista de alergenos para pintar de verde o de rojo */
        putText( dstImg, allergens[i], org, FONT_HERSHEY_COMPLEX, 0.75, Scalar(0, 0, 255), 2);
    }

    return Result(objectName, viewName, 0, 0, scene_corners, allergens);
}
