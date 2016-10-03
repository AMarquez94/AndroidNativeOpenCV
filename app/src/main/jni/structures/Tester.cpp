//
// Created by alejandro on 2/10/16.
//

#include "headers/Tester.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include "headers/utils.h"

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
        error("TESTER", "Can't open file " + path + "/info.txt");
    } else{

        string wordImage, wordName;
        while (file >> wordImage)
        {

            Mat image = imread(path + "/" + wordImage, CV_LOAD_IMAGE_COLOR);
            String filename(wordImage);
            file >> wordName;
            String name(wordName);
            TestImage testImage = TestImage(name, filename, image);
            this->images.push_back(testImage);
        }

        file.close();
    }
}

long long int Tester::doTest(int iterations) {

    long long int totalTime = 0;

    for(int iter = 0; iter < iterations; iter++){

        /* Create confusion matrix */
        int numObjects = this->recognizer.getObjects().size();
        vector <vector <int> > confusionMatrix(numObjects);
        for(int i = 0; i < numObjects; i ++){
            confusionMatrix[i] = vector <int> (numObjects + 1);
            for(int j = 0; j < numObjects + 1; j++){
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

            if(y != -1){
                confusionMatrix[x][y] = confusionMatrix[x][y] + 1;
            } else{
                confusionMatrix[x][numObjects] = confusionMatrix[x][numObjects] + 1;
                if(iter == iterations - 1){
                    log("OBJECT NOT FOUND", this->images[i].getFileName());
                }
            }

        }

        if(iter == iterations - 1){
            /* Print confusion matrix */
            printConfusionMatrix(confusionMatrix, recognizer.getObjects());
        }

        totalTime = totalTime + totalDuration;
    }

    return totalTime/iterations;
}

void printConfusionMatrix(vector <vector <int> > confusionMatrix, vector<Object> objects){

    log("CONFUSION_MATRIX_START", "===================================================================");
    String aux = "--------";
    /* Get first row (names) */
    for(int i = 0; i < objects.size(); i++){
        aux = aux + " " + objects[i].getName();
    }
    aux = aux + " No_Recognized";
    log("CONFUSION_MATRIX", aux);

    /* Print the other rows */
    for(int i = 0; i < confusionMatrix.size(); i++){
        aux = objects[i].getName();
        for(int j = 0; j < confusionMatrix[0].size(); j++){
            aux = aux + " - " + intToString(confusionMatrix[i][j]);
        }
        log("CONFUSION_MATRIX", aux);
    }
    log("CONFUSION_MATRIX_END", "=====================================================================");
}
