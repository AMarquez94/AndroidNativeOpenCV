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

void printConfusionMatrix(const vector <vector <int> >& confusionMatrix,const vector <ObjectKp>& objects);
String writeConfusionMatrix(const vector <vector <int> >& confusionMatrix,const vector <ObjectKp>& objects);

Tester::Tester(const Recognizer& recognizer) {
    this->recognizer = recognizer;
//    this->images = vector<TestImage>();
}

Recognizer Tester::getRecognizer() const{
    return this->recognizer;
}

vector<TestImage> Tester::getTestImages() const{
    return this->images;
}

void Tester::setRecognizer(const Recognizer& recognizer) {
    this->recognizer = recognizer;
}

void Tester::setImages(const vector<TestImage>& images) {
    this->images = images;
}

void Tester::addImage(const TestImage& image) {
    this->images.push_back(image);
}

void Tester::addImagesFromPath(const String& path) {
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
            log("TESTER_LOADER", name);
        }

        file.close();
    }
}

long long int Tester::doTest(const String& path, const int& iterations) {

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
            Result resultObject = recognizer.recognizeObject(this->images[i].getImageGray(),
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

void printConfusionMatrix(const vector <vector <int> >& confusionMatrix,const vector <ObjectKp>& objects){

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

String writeConfusionMatrix(const vector <vector <int> >& confusionMatrix, const vector<ObjectKp>& objects){
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

long long int Tester::doTestRemote(const String& path, const int& iterations, Network network) {

    network = Network("155.210.155.187", 80);
    network.startConnection();

    ofstream myfile;
    myfile.open(path + "/resultados.txt");

    ofstream myfileHand;
    myfileHand.open(path + "/resultadosHand.txt");

    ofstream myfileHandOclusion;
    myfileHandOclusion.open(path + "/resultadosHandOclusion.txt");

    ofstream myfileTable;
    myfileTable.open(path + "/resultadosTable.txt");

    long long int totalTime = 0;
    String matrix = "";
    String matrixHand = "";
    String matrixHandOclusion = "";
    String matrixTable = "";
    int averagePoints = 0;
    int averageMatches = 0;

    for(int iter = 0; iter < iterations; iter++){

        log("TEST", "Iteration " + intToString(iter));

        /* Create confusion matrix */
        int numObjects = this->recognizer.getObjects().size();
        vector <vector <int> > confusionMatrix(numObjects);
        vector <vector <int> > confusionMatrixHand(numObjects);
        vector <vector <int> > confusionMatrixHandOclusion(numObjects);
        vector <vector <int> > confusionMatrixTable(numObjects);
        for(int i = 0; i < numObjects; i ++){
            confusionMatrix[i] = vector <int> (numObjects + 1);
            confusionMatrixHand[i] = vector <int> (numObjects + 1);
            confusionMatrixHandOclusion[i] = vector <int> (numObjects + 1);
            confusionMatrixTable[i] = vector <int> (numObjects + 1);

            for(int j = 0; j < numObjects + 1; j++){
                confusionMatrix[i][j] = 0;
                confusionMatrixHand[i][j] = 0;
                confusionMatrixHandOclusion[i][j] = 0;
                confusionMatrixTable[i][j] = 0;
            }
        }

        /* Measuring time */
        long long int totalDuration = 0.0;

        for(unsigned int i = 0; i < this->images.size(); i++){

            log("TEST", "Image " + intToString(i));

            Mat aux = Mat(this->images[i].getImageColor().size(), CV_8UC3);

            /* Start measuring time */
            high_resolution_clock::time_point begin = high_resolution_clock::now();

            Result resultObject = Result();

            if(!network.sendImage(this->images[i].getImageColor())){
                error("TESTER", "Error al enviar");
            } else{
                if(!network.receiveImage(aux, resultObject)){
                    error("TESTER", "Error al recibir");
                }
            }

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
                if(i % 3 == 0){
                    confusionMatrixHand[x][y] = confusionMatrixHand[x][y] + 1;
                } else if(i % 3 == 1){
                    confusionMatrixHandOclusion[x][y] = confusionMatrixHandOclusion[x][y] + 1;
                } else if(i % 3 == 2){
                    confusionMatrixTable[x][y] = confusionMatrixTable[x][y] + 1;
                }
            } else{
                confusionMatrix[x][numObjects] = confusionMatrix[x][numObjects] + 1;
                if(i % 3 == 0){
                    confusionMatrixHand[x][numObjects] = confusionMatrixHand[x][numObjects] + 1;
                } else if(i % 3 == 1){
                    confusionMatrixHandOclusion[x][numObjects] = confusionMatrixHandOclusion[x][numObjects] + 1;
                } else if(i % 3 == 2){
                    confusionMatrixTable[x][numObjects] = confusionMatrixTable[x][numObjects] + 1;
                }
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
            matrixHand =  writeConfusionMatrix(confusionMatrixHand, recognizer.getObjects());
            matrixHandOclusion =  writeConfusionMatrix(confusionMatrixHandOclusion, recognizer.getObjects());
            matrixTable =  writeConfusionMatrix(confusionMatrixTable, recognizer.getObjects());
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

    myfileHand << "Tiempo: " << longToString(totalTime/iterations) << endl;
    myfileHand << "Media puntos: " << intToString(averagePoints) << endl;
    myfileHand << "Media matches buenos: " << intToString(averageMatches) << endl;
    myfileHand << matrixHand;
    myfileHand.close();

    myfileHandOclusion << "Tiempo: " << longToString(totalTime/iterations) << endl;
    myfileHandOclusion << "Media puntos: " << intToString(averagePoints) << endl;
    myfileHandOclusion << "Media matches buenos: " << intToString(averageMatches) << endl;
    myfileHandOclusion << matrixHandOclusion;
    myfileHandOclusion.close();

    myfileTable << "Tiempo: " << longToString(totalTime/iterations) << endl;
    myfileTable << "Media puntos: " << intToString(averagePoints) << endl;
    myfileTable << "Media matches buenos: " << intToString(averageMatches) << endl;
    myfileTable << matrixTable;
    myfileTable.close();

    network.closeConnection();
    return totalTime/iterations;
}

