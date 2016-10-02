//
// Created by alejandro on 2/10/16.
//

#include "headers/Tester.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <android/log.h>

using namespace std;
using namespace std::chrono;
using namespace cv;

void printConfusionMatrix(vector <vector <int> > confusionMatrix, vector <Object> objects);

Tester::Tester(Recognizer recognizer) {
    this->recognizer = recognizer;
//    this->images = vector<TestImage>();
}

Recognizer Tester::getRecognizer() {
    return this->recognizer;
}

vector<TestImage> Tester::getTestImages() {
    return this->images;
}

void Tester::setRecognizer(Recognizer recognizer) {
    this->recognizer = recognizer;
}

void Tester::setImages(vector <TestImage> images) {
    this->images = images;
}

void Tester::addImage(TestImage image) {
    this->images.push_back(image);
}

void Tester::addImagesFromPath(String path) {
    ifstream file;
    file.open((path + "/info.txt").c_str());
    if (!file.is_open()){
        __android_log_print(ANDROID_LOG_ERROR, "TESTER", "Can't open file %s", (path + "/info.txt").c_str());
    } else{

        string wordImage, wordName;
        while (file >> wordImage)
        {

            Mat image = imread(path + "/" + wordImage, CV_LOAD_IMAGE_COLOR);
            file >> wordName;
            String name(wordName);
            TestImage testImage = TestImage(name, image);
            this->images.push_back(testImage);
        }

        file.close();
    }
}

long long int Tester::doTest() {

    /* Create confusion matrix */
    int numObjects = this->recognizer.getObjects().size();
    vector <vector <int> > confusionMatrix(numObjects);
    for(int i = 0; i < numObjects; i ++){
        confusionMatrix[i] = vector <int> (numObjects);
        for(int j = 0; j < numObjects; j++){
            confusionMatrix[i][j] = 0;
        }
    }

    /* Measuring time */
    long long int totalDuration = 0.0;

    for(int i = 0; i < this->images.size(); i++){

        Mat aux = Mat(this->images[i].getImageColor().size(), CV_8UC3);

        /* Start measuring time */
        high_resolution_clock::time_point begin = high_resolution_clock::now();

        /* Recognize the test image */
        String objectName = recognizer.RecognizeObject(this->images[i].getImageGray(),
                                                       this->images[i].getImageColor(), aux);

        /* Ends measuring time */
        high_resolution_clock::time_point end = high_resolution_clock::now();

        /* Added to total time */
        auto duration = duration_cast<microseconds>( end - begin ).count();
        totalDuration = totalDuration + duration;

        /* Fill confusion matrix */

        int x = this->recognizer.getObjectIndex(this->images[i].getName());
        int y = this->recognizer.getObjectIndex(objectName);

        confusionMatrix[x][y] = confusionMatrix[x][y] + 1;

    }

    /* Print confusion matrix */
    printConfusionMatrix(confusionMatrix, recognizer.getObjects());

    return totalDuration;
}

String intToString(int number){
    string Result;//string which will contain the result

    stringstream convert; // stringstream used for the conversion

    convert << number;//add the value of Number to the characters in the stream

    return convert.str();
}

void printConfusionMatrix(vector <vector <int> > confusionMatrix, vector<Object> objects){

    __android_log_print(ANDROID_LOG_DEBUG, "CONFUSION_MATRIX_START", "===================================================================");
    String aux = "--------";
    /* Get first row (names) */
    for(int i = 0; i < objects.size(); i++){
        aux = aux + " " + objects[i].getName();
    }
    __android_log_print(ANDROID_LOG_DEBUG, "CONFUSION_MATRIX", "%s", aux.c_str());

    /* Print the other rows */
    for(int i = 0; i < confusionMatrix.size(); i++){
        aux = objects[i].getName();
        for(int j = 0; j < confusionMatrix[0].size(); j++){
            aux = aux + " - " + intToString(confusionMatrix[i][j]);
        }
        __android_log_print(ANDROID_LOG_DEBUG, "CONFUSION_MATRIX", "%s", aux.c_str());
    }
    __android_log_print(ANDROID_LOG_DEBUG, "CONFUSION_MATRIX_END", "=====================================================================");
}
