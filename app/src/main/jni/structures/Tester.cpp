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
String writeConfusionMatrix(vector <vector <int> > confusionMatrix, vector <Object> objects);

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

long long int Tester::doTest(String path, int iterations) {

    ofstream myfile;
    myfile.open(path + "/resultados.txt");

    long long int totalTime = 0;
    String matrix = "";
    int averagePoints = 0;
    int averageMatches = 0;

    for(int iter = 0; iter < iterations; iter++){

        log("TEST", "Iteration " + intToString(iter));

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

        for(unsigned int i = 0; i < this->images.size(); i++){

            log("TEST", "Image " + intToString(i));

            Mat aux = Mat(this->images[i].getImageColor().size(), CV_8UC3);

            /* Start measuring time */
            high_resolution_clock::time_point begin = high_resolution_clock::now();

            /* Recognize the test image */
            Result resultObject = recognizer.RecognizeObject(this->images[i].getImageGray(),
                                                            this->images[i].getImageColor(), aux);
//            String objectName = recognizer.RecognizeObject(this->images[i].getImageGray(),
//                                                           this->images[i].getImageColor(), aux);

            /* Ends measuring time */
            high_resolution_clock::time_point end = high_resolution_clock::now();

            /* Added to total time */
            auto duration = duration_cast<microseconds>( end - begin ).count();
            totalDuration = totalDuration + duration;

            /* Fill confusion matrix */

            int x = this->recognizer.getObjectIndex(this->images[i].getName());
            int y = this->recognizer.getObjectIndex(resultObject.getObjectName());

            if(y != -1){
                confusionMatrix[x][y] = confusionMatrix[x][y] + 1;
            } else{
                confusionMatrix[x][numObjects] = confusionMatrix[x][numObjects] + 1;
                if(iter == iterations - 1){
                    log("OBJECT NOT FOUND", this->images[i].getFileName());
                }
            }

            if(iter == iterations - 1){
                averagePoints = averagePoints + resultObject.getNPoints();
                averageMatches = averageMatches + resultObject.getNMatches();
            }

        }

        if(iter == iterations - 1){
            /* Print confusion matrix */
            matrix =  writeConfusionMatrix(confusionMatrix, recognizer.getObjects());
            averagePoints = averagePoints/this->images.size();
            averageMatches = averageMatches/this->images.size();
        }

        totalTime = totalTime + totalDuration;
    }


    myfile << "Tiempo: " << longToString(totalTime/iterations) << endl;
    myfile << "Media puntos: " << intToString(averagePoints) << endl;
    myfile << "Media matches buenos: " << intToString(averageMatches) << endl;
    myfile << matrix;
    myfile.close();
    return totalTime/iterations;
}

void printConfusionMatrix(vector <vector <int> > confusionMatrix, vector<Object> objects){

    log("CONFUSION_MATRIX_START", "===================================================================");
    String aux = "--------";
    /* Get first row (names) */
    for(unsigned int i = 0; i < objects.size(); i++){
        aux = aux + " " + objects[i].getName();
    }
    aux = aux + " No_Recognized";
    log("CONFUSION_MATRIX", aux);

    /* Print the other rows */
    for(unsigned int i = 0; i < confusionMatrix.size(); i++){
        aux = objects[i].getName();
        for(unsigned int j = 0; j < confusionMatrix[0].size(); j++){
            aux = aux + " - " + intToString(confusionMatrix[i][j]);
        }
        log("CONFUSION_MATRIX", aux);
    }
    log("CONFUSION_MATRIX_END", "=====================================================================");
}

String writeConfusionMatrix(vector <vector <int> > confusionMatrix, vector<Object> objects){
    String aux = "";
    for(unsigned int i = 0; i < objects.size(); i++){
        aux = aux + objects[i].getName() + " ";
    }
    aux = aux + "No_objeto\n";

    /* Print the other rows */
    for(unsigned int i = 0; i < confusionMatrix.size(); i++){
        for(unsigned int j = 0; j < confusionMatrix[0].size(); j++){
            aux = aux + intToString(confusionMatrix[i][j]) + " ";
        }
        aux = aux + "\n";
    }

    return aux;
}

