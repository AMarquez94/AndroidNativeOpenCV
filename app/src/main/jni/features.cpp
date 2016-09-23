#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <android/log.h>
//#include "opencv2/xfeatures2d/nonfree.hpp"
//#include "opencv2/xfeatures2d.hpp"


using namespace std;
using namespace cv;

void features(Mat imgGray, Mat imgColor, Mat dst, Mat object);

/**
 * imgGray : camera image in grayscale mode
 * imgColor : camera image in bgr mode
 * dst: image to be displayed in the phone
 * object: image of the object to recognize in grayscale mode
 */
void features(Mat imgGray, Mat imgColor, Mat dst, Mat object){

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;
    Ptr<ORB> detector = ORB::create();
    vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    detector->detectAndCompute( object, Mat(), keypoints_object, descriptors_object );
    detector->detectAndCompute( imgGray, Mat(), keypoints_scene, descriptors_scene );

//    __android_log_print(ANDROID_LOG_DEBUG, "SIZE OBJECT", "keypoints %d", keypoints_object.size());
//    __android_log_print(ANDROID_LOG_DEBUG, "SIZE SCENE", "keypoints %d", keypoints_scene.size());

    vector< vector<DMatch> > matches;
    vector<KeyPoint> matched_object, matched_scene;
    vector<Point2f> obj, scene;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    //Peta si no se hace la comprobacion
    if(keypoints_object.size() != 0 && keypoints_scene.size() != 0){
        matcher->knnMatch(descriptors_object, descriptors_scene, matches, 2);
    }

    for(unsigned i = 0; i < matches.size(); i++){
        if(matches[i][0].distance < 0.8*matches[i][1].distance){
            matched_object.push_back(keypoints_object[matches[i][0].queryIdx]);
            matched_scene.push_back(keypoints_scene[matches[i][0].trainIdx]);
            obj.push_back(keypoints_object[matches[i][0].queryIdx].pt);
            scene.push_back(keypoints_scene[matches[i][0].trainIdx].pt);
        }
    }

    __android_log_print(ANDROID_LOG_DEBUG, "SIZE", "keypoints %d", matched_scene.size());

    if(matched_scene.size() >= 4){

        //Homography matrix
        Mat H = findHomography(obj, scene, RANSAC);

        if(!H.empty()){
            //Corners Object
            vector<Point2f> obj_corners(4);
            obj_corners[0] = cvPoint(0,0);
            obj_corners[1] = cvPoint(object.cols,0);
            obj_corners[2] = cvPoint(object.cols, object.rows);
            obj_corners[3] = cvPoint(0, object.rows);

            vector<Point2f> scene_corners(4);

            perspectiveTransform(obj_corners, scene_corners, H);

            line(imgColor, scene_corners[0], scene_corners[1], Scalar(0,255,0), 4);
            line(imgColor, scene_corners[1], scene_corners[2], Scalar(0,255,0), 4);
            line(imgColor, scene_corners[2], scene_corners[3], Scalar(0,255,0), 4);
            line(imgColor, scene_corners[3], scene_corners[0], Scalar(0,255,0), 4);
        }
    }

    //-- Draw keypoints
    drawKeypoints( imgColor, matched_scene, dst, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
























//    //-- Step 1: Detect the keypoints using SURF Detector
//    int minHessian = 400;
//    Ptr<ORB> detector = ORB::create(minHessian);
//    vector<KeyPoint> keypoints_object, keypoints_scene;
//    Mat descriptors_object, descriptors_scene;
//    detector->detectAndCompute( imgGray, Mat(), keypoints_scene, descriptors_scene );
//    detector->detectAndCompute( object, Mat(), keypoints_object, descriptors_object );
//
////    __android_log_print(ANDROID_LOG_DEBUG, "SIZE OBJECT", "keypoints %d", keypoints_object.size());
////    __android_log_print(ANDROID_LOG_DEBUG, "SIZE SCENE", "keypoints %d", keypoints_scene.size());
//
//    FlannBasedMatcher matcher(/*new flann::LshIndexParams(12,20,2)*/);
//    vector< DMatch > matches;
//    matcher.match(descriptors_object, descriptors_scene, matches);
//
////    __android_log_print(ANDROID_LOG_DEBUG, "SIZE MATCHES", "matches %d", matches.size());
//
//    double max_dist = 0; double min_dist = 100;
//    //-- Quick calculation of max and min distances between keypoints
//    for( int i = 0; i < descriptors_object.rows; i++ )
//    { double dist = matches[i].distance;
//        if( dist < min_dist ) min_dist = dist;
//        if( dist > max_dist ) max_dist = dist;
//    }
//
////    __android_log_print(ANDROID_LOG_DEBUG, "MIN DIST", "minima distancia %f", min_dist);
//
//    vector<DMatch> good_matches;
//    for(int i = 0; i < descriptors_object.rows; i++){
//        if(matches[i].distance < 2*min_dist){
//            good_matches.push_back(matches[i]);
//        }
//    }
//
//    __android_log_print(ANDROID_LOG_DEBUG, "SIZE", "keypoints %d", good_matches.size());
//
//    vector<KeyPoint> obj;
//    vector<KeyPoint> scene;
//
//    for(size_t i = 0; i < good_matches.size(); i++){
//        scene.push_back(keypoints_scene[good_matches[i].trainIdx]);
//    }
//
//
//    //-- Draw keypoints
//    drawKeypoints( imgColor, scene, dst, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

}







//void features(Mat imgGray, Mat imgColor, Mat dst, Mat object);
//
//
///**
// * imgGray : camera image in grayscale mode
// * imgColor : camera image in bgr mode
// * dst: image to be displayed in the phone
// * object: image of the object to recognize in grayscale mode
// */
//void features(Mat imgGray, Mat imgColor, Mat dst, Mat object){
//
//    int minHessian = 400;
//
//    //KeyPoints
//
//    xfeatures2d::SurfFeatureDetector detector;
//
//    //xfeatures2d::SurfFeatureDetector detector(minHessian);
//
//    vector<KeyPoint> keypoints_object, keypoints_scene;
//
//    extractor.compute(object, keypoints_object, descri);
//    extractor.compute(imgGray, keypoints_scene);
//
//    //Descriptors
//
//    SurfDescriptorExtractor extractor;
//
//    Mat descriptors_object, descriptors_scene;
//
//    extractor.compute(object, keypoints_object, descriptors_object);
//    extractor.compute(imgGray, keypoints_scene, descriptors_scene);
//
//
//    FlannBasedMatcher matcher;
//    vector<DMatch> matches;
//    matcher.match(descriptors_object, descriptors_scene, matches);
//
//    double max_dist = 0; double min_dist = 100;
//
//    for(int i = 0; i < descriptors_object.rows; i++){
//        double dist = matches[i].distance;
//        if(dist < min_dist){
//            min_dist = dist;
//        }
//        if(dist > max_dist){
//            max_dist = dist;
//        }
//    }
//
//    //LOG MAN / MIN dist
//
//    vector <DMatch> good_matches;
//
//    for(int i = 0; i < descriptors_object.rows; i++){
//        if(matches[i].distance < 3*min_dist){
//            good_matches.push_back( matches[i] );
//        }
//    }
//
//    vector<Point2f> obj;
//    vector<Point2f> scene;
//
//    for( int i = 0; i < good_matches.size(); i++ )
//    {
//        //-- Get the keypoints from the good matches
//        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
//        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
//    }
//
//    drawKeypoints(imgGray, scene, dst, Scalar(0, 0, 255));
//
//
//
//
//
//
//}
