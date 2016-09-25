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

void features(Mat imgGray, Mat imgColor, Mat dst, Ptr<ORB> detector, Ptr<DescriptorMatcher> matcher,
              vector<KeyPoint> keypoints_object, Mat descriptors_object, vector<Point2f> obj_corners);

bool isConvex(vector<Point2f> scene_corners);

/**
 * imgGray : camera image in grayscale mode
 * imgColor : camera image in bgr mode
 * dst: image to be displayed in the phone
 * detector: feature detector for the images
 * matcher: matcher between two images' feature points
 * keypoints_object: keypoints of the object to recognize
 * descriptors_object: descriptor of the object to recognize
 * obj_corners: image corners of the object to recognize
 */
void features(Mat imgGray, Mat imgColor, Mat dst, Ptr<ORB> detector, Ptr<DescriptorMatcher> matcher,
              vector<KeyPoint> keypoints_object, Mat descriptors_object, vector<Point2f> obj_corners){

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;
    vector<KeyPoint> keypoints_scene;
    Mat descriptors_scene;
    detector->detectAndCompute( imgGray, Mat(), keypoints_scene, descriptors_scene );

//    __android_log_print(ANDROID_LOG_DEBUG, "SIZE OBJECT", "keypoints %d", keypoints_object.size());
//    __android_log_print(ANDROID_LOG_DEBUG, "SIZE SCENE", "keypoints %d", keypoints_scene.size());

    vector< vector<DMatch> > matches;
    vector<KeyPoint> matched_object, matched_scene;
    vector<Point2f> obj, scene;

    //Peta si no se hace la comprobacion
    if(keypoints_object.size() != 0 && keypoints_scene.size() != 0){
        matcher->knnMatch(descriptors_object, descriptors_scene, matches, 2);
    }

    for(unsigned i = 0; i < matches.size(); i++){
        if(matches[i][0].distance < 0.7*matches[i][1].distance){
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
            vector<Point2f> scene_corners(4);

            perspectiveTransform(obj_corners, scene_corners, H);

            /* Check if convex or complex */
            Scalar colour;
            if(isConvex(scene_corners)){
                colour = Scalar(0,255,0);
            } else{
                colour = Scalar(0,0,255);
            }

            line(imgColor, scene_corners[0], scene_corners[1], colour, 4);
            line(imgColor, scene_corners[1], scene_corners[2], colour, 4);
            line(imgColor, scene_corners[2], scene_corners[3], colour, 4);
            line(imgColor, scene_corners[3], scene_corners[0], colour, 4);
        }
    }

    //-- Draw keypoints
    drawKeypoints( imgColor, matched_scene, dst, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

}

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
