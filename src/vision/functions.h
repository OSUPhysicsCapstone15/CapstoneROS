#ifndef functions_H_INCLUDED
#define functions_H_INCLUDED

#include "beacon.h"
#include "blob.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <string>
#include <ctime>
#include <algorithm>
#include "sys/time.h"
using namespace cv;
using namespace std;

struct hsvParams{
  int hL, sL, vL, hH, sH, vH;
};

void removenoise(Mat);
void removenoise_heatmap(Mat);

Mat findBiggestBlob(Mat);

void findGrass(Mat, Mat);

void robot_angle(beacon_loc *orientation, Mat img, int object_cols);
void robot_angle(sample_loc *orientation, Mat img, int object_cols);

void tilt_turn_degrees(Mat img, int object_rows, int object_cols, sample_loc* orientation);

Point findCenterPoint(vector<KeyPoint>);

KeyPoint getLeftKeyPoint(vector<KeyPoint>);
KeyPoint getRightKeyPoint(vector<KeyPoint>);
KeyPoint getBottomKeyPoint(vector<KeyPoint>);
KeyPoint getTopKeyPoint(vector<KeyPoint>);

Mat getPic(VideoCapture);
double getPicture(VideoCapture);
void createWindows();

SimpleBlobDetector::Params setupBeaconBlobParams();
SimpleBlobDetector::Params setupObjectBlobParams();
SimpleBlobDetector::Params setupObjectBlobParams_heatmap();

void showWindows(Mat, Mat, Mat, Mat, Mat, Mat, Mat, Mat);

Mat drawAndCircleKeypoints(vector<KeyPoint>, Mat);

void threshDilateDetect(Mat, Mat, double, SimpleBlobDetector::Params, vector<KeyPoint>&);

void initializePointAndShoot();
void zoomInFull();
void zoomOutFull();
void shootPic();

bool beaconLocation(vector<KeyPoint>, beacon_loc*);

/*
Old, unused functions

void getBeaconOrientation(vector<KeyPoint>, beacon_loc*);
void printDistanceFromLights(vector<KeyPoint>, beacon_loc*);
*/

#endif

