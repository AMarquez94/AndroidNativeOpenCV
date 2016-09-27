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
              vector < vector<KeyPoint> > keypoints_objects,
              vector < Mat > descriptors_objects ,
              vector < vector<Point2f> > objs_corners);

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
              vector < vector<KeyPoint> > keypoints_objects,
              vector < Mat > descriptors_objects,
              vector < vector<Point2f> > objs_corners,
              vector < String > obj_names){

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;
    vector<KeyPoint> keypoints_scene;
    Mat descriptors_scene;
    detector->detectAndCompute( imgGray, Mat(), keypoints_scene, descriptors_scene );

//    __android_log_print(ANDROID_LOG_DEBUG, "SIZE OBJECT", "keypoints %d", keypoints_object.size());
//    __android_log_print(ANDROID_LOG_DEBUG, "SIZE SCENE", "keypoints %d", keypoints_scene.size());

    vector < vector<KeyPoint> >matched_objects(descriptors_objects.size()), matched_scene(descriptors_objects.size());
    vector < vector<Point2f> >objs(descriptors_objects.size()), scene(descriptors_objects.size());
    vector < vector < vector<DMatch> > >matches(descriptors_objects.size());


    int best_match = -1;
    int number_of_matches = 0;
    for(int i = 0; i < descriptors_objects.size(); i ++){
        //Peta si no se hace la comprobacion
        if(keypoints_objects[i].size() != 0 && keypoints_scene.size() != 0) {

//            __android_log_print(ANDROID_LOG_DEBUG, "BUCLE", "HABEMUS IMAGE");
            // initial NN matching
            matcher->knnMatch(descriptors_objects[i], descriptors_scene, matches[i], 2);

//            __android_log_print(ANDROID_LOG_DEBUG, "MATCHES", "matches %d", matches[i].size());
            // filter matches with dnn1 < 0.7 dnn2
            for (unsigned j = 0; j < matches[i].size(); j++) {
                if (matches[i][j][0].distance < 0.7 * matches[i][j][1].distance) {
//                    __android_log_print(ANDROID_LOG_DEBUG, "MATCHES", "good match");
                    matched_objects[i].push_back(keypoints_objects[i][matches[i][j][0].queryIdx]);
//                    __android_log_print(ANDROID_LOG_DEBUG, "MATCHES", "good match cont");
                    matched_scene[i].push_back(keypoints_scene[matches[i][j][0].trainIdx]);
//                    __android_log_print(ANDROID_LOG_DEBUG, "MATCHES", "good match cont 2");
                    objs[i].push_back(keypoints_objects[i][matches[i][j][0].queryIdx].pt);
//                    __android_log_print(ANDROID_LOG_DEBUG, "MATCHES", "good match cont 3");
                    scene[i].push_back(keypoints_scene[matches[i][j][0].trainIdx].pt);
//                    __android_log_print(ANDROID_LOG_DEBUG, "MATCHES", "good match cont 4");
                }
            }

//            __android_log_print(ANDROID_LOG_DEBUG, "MATCHES", "good matches %d", matched_objects[i].size());

            if (number_of_matches < matched_scene[i].size()) {
                best_match = i;
                number_of_matches = matched_scene[i].size();
            }
        }
    }


//    __android_log_print(ANDROID_LOG_DEBUG, "BUCLE", "ok");
//    for(int i = 0; i < descriptors_objects.size(); i++){
//        __android_log_print(ANDROID_LOG_DEBUG, "BUCLE", "ITER");
//        __android_log_print(ANDROID_LOG_DEBUG, "BUCLE", "Matched scene %d", matched_scene[i].size());
//        __android_log_print(ANDROID_LOG_DEBUG, "OP", "fin");
//        if(number_of_matches < matched_scene[i].size()){
//            best_match = i;
//            number_of_matches = matched_scene[i].size();
//        }
//    }

    __android_log_print(ANDROID_LOG_DEBUG, "BEST_MATCH", "best match %d", best_match);

    bool isMatch = false;

    if(best_match!=-1 && matched_scene[best_match].size() >= 4){

        //Homography matrix
        Mat H = findHomography(objs[best_match], scene[best_match], RANSAC);

        if(!H.empty()){
            //Corners Object
            vector<Point2f> scene_corners(4);

            perspectiveTransform(objs_corners[best_match], scene_corners, H);

            /* Check if convex or concav */
            Scalar colour;
            if(isConvex(scene_corners)){
                colour = Scalar(0,255,0);
                isMatch = true;
            } else{
                colour = Scalar(0,0,255);
            }

            line(imgColor, scene_corners[0], scene_corners[1], colour, 4);
            line(imgColor, scene_corners[1], scene_corners[2], colour, 4);
            line(imgColor, scene_corners[2], scene_corners[3], colour, 4);
            line(imgColor, scene_corners[3], scene_corners[0], colour, 4);
        }
    }

    if(isMatch){
        //-- Draw keypoints
        drawKeypoints( imgColor, matched_scene[best_match], dst, Scalar::all(-1), DrawMatchesFlags::DEFAULT );


        //-- Write object name
        Size textsize = getTextSize(obj_names[best_match], FONT_HERSHEY_COMPLEX, 1, 2, 0);
//        Point org((640 - textsize.width)/2, (480 - textsize.height)/2);
        Point org((640 - textsize.width - 20), textsize.height + 20);
        putText( dst, obj_names[best_match], org, FONT_HERSHEY_COMPLEX, 1,
                 Scalar(0, 0, 255), 2);
    }
    else{
        imgColor.copyTo(dst);
    }
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