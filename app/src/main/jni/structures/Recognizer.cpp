//
// Created by alejandro on 27/09/16.
//

#include "headers/Recognizer.h"
#include <fstream>
#include <iostream>
#include <string>
#include <android/log.h>

using namespace std;
using namespace cv;


/* Aux methods */
bool isConvex(vector <Point2f> scene_corners);


Recognizer::Recognizer() {

}

Recognizer::Recognizer(String detector, String extractor, String matcher){
    if(detector == "ORB"){
        this->detector = ORB::create();
    } else if(detector == "FAST"){
        /* TODO: implement */
    }

    if(extractor == "ORB"){
        this->extractor = ORB::create();
    } else if(extractor == "BRISK"){
        /* TODO: implement */
    }

    this->matcher = DescriptorMatcher::create(matcher);
    this->matcherDistanceFilter = 0.7;
}

Recognizer::Recognizer(String detector, String extractor, String matcher, vector<Object> objects){
    if(detector == "ORB"){
        this->detector = ORB::create();
    } else if(detector == "FAST"){
        /* TODO: implement */
    }

    if(extractor == "ORB"){
        this->extractor = ORB::create();
    } else if(extractor == "BRISK"){
        /* TODO: implement */
    }

    this->matcher = DescriptorMatcher::create(matcher);
    this->matcherDistanceFilter = 0.7;

    this->objects = objects;
}

Recognizer::Recognizer(String detector, vector<int> detectorParams, String extractor,
                       vector<int> extractorParams, String matcher, vector<int> matcherParams){

    /* TODO: Implement */
}

Recognizer::Recognizer(String detector, vector<int> detectorParams, String extractor,
                       vector<int> extractorParams, String matcher, vector<int> matcherParams,
                       vector<Object> objects){

    /* TODO: Implement */
}

/* Getters */

Ptr<FeatureDetector> Recognizer::getDescriptor(){
    return this->getDescriptor();
}


Ptr<DescriptorExtractor> Recognizer::getExtractor(){
    return this->getExtractor();
}

Ptr<DescriptorMatcher> Recognizer::getMatcher(){
    return this->getMatcher();
}

/* Setters */

void Recognizer::setDescriptor(String detector) {
    if(detector == "ORB"){
        this->detector = ORB::create();
    } else if(detector == "FAST"){
        /* TODO: implement */
    }
}

void Recognizer::setExtractor(String extractor) {
    if(extractor == "ORB"){
        this->extractor = ORB::create();
    } else if(extractor == "BRISK"){
        /* TODO: implement */
    }
}

void Recognizer::setMatcher(String matcher) {
    this->matcher = DescriptorMatcher::create(matcher);
    this->matcherDistanceFilter = 0.7;
}

Object Recognizer::createObject(String path, bool add) {
    ifstream file;
    file.open((path + "/info.txt").c_str());
    if (!file.is_open()){
        return Object(true);
    } else{
        String name;
        vector <Mat> images;
        vector < vector<KeyPoint> > keypoints;
        vector <Mat> descriptors;
        vector < vector<Point2f> > corners;
        vector <String> viewNames;
        bool easy;

        string word;
        int i = 0;
        while (file >> word)
        {
            if(i == 0){

                /* Object name */
                name = word;

                __android_log_print(ANDROID_LOG_DEBUG,"NOMBRE","%s",name.c_str());

            } else if(i == 1){

                /* Object easy or not */
                easy = (word == "true");
                __android_log_print(ANDROID_LOG_DEBUG, "EASY", "%d", easy);
            } else if(i%2 == 0){

                /* Object image view*/
                images.push_back(Mat());
                keypoints.push_back(vector <KeyPoint>());
                descriptors.push_back(Mat());
                corners.push_back(vector <Point2f>(4));

                Mat image = imread(path + "/" + word, CV_LOAD_IMAGE_GRAYSCALE);
//                if(!image.data){
//                    __android_log_print(ANDROID_LOG_DEBUG, "TRISTE", "BUAAAAAAAAAAAAAAAAAH");
//                } else{
//                    __android_log_print(ANDROID_LOG_DEBUG, "HAY DATA", "ZI");
//                }
//                __android_log_print(ANDROID_LOG_DEBUG, "EASY", "%s", (path + "/" + word).c_str());
                images[(i/2)-1] = image;
                this->detector->detect(image, keypoints[(i/2)-1]);
                this->extractor->compute(image, keypoints[(i/2)-1], descriptors[(i/2)-1]);
                corners[(i/2)-1][0] = cvPoint(0,0);
                corners[(i/2)-1][1] = cvPoint(image.cols,0);
                corners[(i/2)-1][2] = cvPoint(image.cols, image.rows);
                corners[(i/2)-1][3] = cvPoint(0, image.rows);
                __android_log_print(ANDROID_LOG_DEBUG, "VISTA", "%d", (i/2)-1);
            } else{

                /* Object name view */
                viewNames.push_back(word);

                String aux = word;
                __android_log_print(ANDROID_LOG_DEBUG, "VISTA NOMBRE", "%s", aux.c_str());
            }

            i++;
        }
        Object obj = Object(name, images, keypoints, descriptors, corners, viewNames, easy);

        if(add){
            this->objects.push_back(obj);
        }

        file.close();

        return obj;
    }
}

String Recognizer::RecognizeObject(Mat sceneImgGray, Mat sceneImgColour, Mat dstImg){


    __android_log_print(ANDROID_LOG_DEBUG,"ENTRADA", "entramos");


    /* Extract features from the sceneImage */
    vector<KeyPoint> keypointsScene;
    Mat descriptorsScene;
    this->detector->detect(sceneImgGray, keypointsScene);
    this->extractor->compute(sceneImgGray, keypointsScene, descriptorsScene);

    __android_log_print(ANDROID_LOG_DEBUG,"Extractor", "extraemos");

    /* Extract the matches between the cameraImage and all the objects in the object list */

    /* Data structures */
    vector < vector< vector<KeyPoint> > >matchedObjects, matchedScene;
    vector < vector< vector<Point2f> > >objs, scene;
    vector < vector< vector< vector<DMatch> > > >matches(this->objects.size());

    int bestMatchObject = -1;   //Best object recognized
    int bestMatchView = -1;     //Best view of the best object recognized
    int numberOfMatches = 0;    //Maximum number of matches found

    if(keypointsScene.size() != 0){

        for(int i = 0; i < this->objects.size(); i++){
            /* Iterating over objects */

            matchedObjects.push_back(vector < vector<KeyPoint> >(this->objects[i].getNumberOfViews()));
            matchedScene.push_back(vector < vector<KeyPoint> >(this->objects[i].getNumberOfViews()));
            objs.push_back(vector < vector<Point2f> >(this->objects[i].getNumberOfViews()));
            scene.push_back(vector < vector<Point2f> >(this->objects[i].getNumberOfViews()));
            matches[i] = vector< vector < vector<DMatch> > >(this->objects[i].getNumberOfViews());

            for(int j = 0; j < this->objects[i].getNumberOfViews(); j++){
                /* Iterating over object views */

                if(this->objects[i].getViewsKeyPoints()[j].size() != 0){

                    matcher->knnMatch(this->objects[i].getViewsDescriptors()[j],descriptorsScene,matches[i][j],2);

                    for(int k = 0; k < matches[i][j].size(); k++){

                        /* Iterating over matches */

                        if(matches[i][j][k][0].distance < this->matcherDistanceFilter * matches[i][j][k][1].distance){

                            /* Good match -> save matches points and info */
                            matchedObjects[i][j].push_back(this->objects[i].getViewsKeyPoints()[j][matches[i][j][k][0].queryIdx]);
                            matchedScene[i][j].push_back(keypointsScene[matches[i][j][k][0].trainIdx]);
                            objs[i][j].push_back(this->objects[i].getViewsKeyPoints()[j][matches[i][j][k][0].queryIdx].pt);
                            scene[i][j].push_back(keypointsScene[matches[i][j][k][0].trainIdx].pt);
                        }
                    }

                    if(numberOfMatches < matchedScene[i][j].size()){

                        /* At the moment, object with more matches -> object recognized */
                        bestMatchObject = i;
                        bestMatchView = j;
                        numberOfMatches = matchedScene[i][j].size();
                    }
                }
            }
        }
    }

    bool isMatch = false;

    if(bestMatchObject!=-1 && bestMatchView!=-1 && matchedScene[bestMatchObject][bestMatchView].size() >= 4){

        //Homography matrix
        Mat H = findHomography(objs[bestMatchObject][bestMatchView], scene[bestMatchObject][bestMatchView], RANSAC);

        if(!H.empty()){
            //Corners Object
            vector<Point2f> scene_corners(4);

            perspectiveTransform(this->objects[bestMatchObject].getViewsCorners()[bestMatchView], scene_corners, H);

            /* Check if convex or concav */
            Scalar colour;
            if(isConvex(scene_corners)){
                colour = Scalar(0,255,0);
                isMatch = true;
            } else{
                colour = Scalar(0,0,255);
            }

            line(sceneImgColour, scene_corners[0], scene_corners[1], colour, 4);
            line(sceneImgColour, scene_corners[1], scene_corners[2], colour, 4);
            line(sceneImgColour, scene_corners[2], scene_corners[3], colour, 4);
            line(sceneImgColour, scene_corners[3], scene_corners[0], colour, 4);
        }
    }

    String objectName;

    if(isMatch){
        //-- Draw keypoints
        drawKeypoints( sceneImgColour, matchedScene[bestMatchObject][bestMatchView], dstImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

        objectName = this->objects[bestMatchObject].getName() + "_" +
            this->objects[bestMatchObject].getViewsNames()[bestMatchView];
    }
    else{

        /* No match */
        sceneImgColour.copyTo(dstImg);
        objectName = "No object";
    }

    /* Write object name */
    Size textsize = getTextSize(objectName, FONT_HERSHEY_COMPLEX, 1, 2, 0);

    Point org((640 - textsize.width - 20), textsize.height + 20);
    putText( dstImg, objectName, org, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 2);
    return objectName;
}

/**
 *
 * Return true if the cuadrilateral formed by the scene_corners is convex
 */
bool isConvex(vector <Point2f> scene_corners){

    bool sign = false;
    bool convex = true;

    int i = 0;
    while(i < 4 && convex){
        float dx1 = scene_corners[(i+2)%4].x - scene_corners[(i+1)%4].x;
        float dy1 = scene_corners[(i+2)%4].y - scene_corners[(i+1)%4].y;
        float dx2 = scene_corners[i].x - scene_corners[(i+1)%4].x;
        float dy2 = scene_corners[i].y - scene_corners[(i+1)%4].y;
        float z = dx1*dy2 - dy1*dx2;
        if(i == 0){
            sign = z > 0;
        } else{
            if( sign != (z>0)){
                convex = false;
            }
        }
        i++;
    }

    return convex;
}


