#ifndef functions_H_INCLUDED
#define functions_H_INCLUDED

#include "beacon.h"
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

Mat findBiggestBlob(Mat);

void findGrass(Mat, Mat);

void robot_angle(Mat, int, beacon_loc*);

Point findCenterPoint(vector<KeyPoint>);

int blob_main();

int beacon_main();

KeyPoint getLeftKeyPoint(vector<KeyPoint>);
KeyPoint getRightKeyPoint(vector<KeyPoint>);
KeyPoint getBottomKeyPoint(vector<KeyPoint>);
KeyPoint getTopKeyPoint(vector<KeyPoint>);

void printDistanceFromLights(vector<KeyPoint>, beacon_loc*);

void getBeaconOrientation(vector<KeyPoint>, beacon_loc*);

Mat getPic(VideoCapture);

void createWindows();

SimpleBlobDetector::Params setupBeaconBlobParams();
SimpleBlobDetector::Params setupObjectBlobParams();

void showWindows(Mat, Mat, Mat, Mat, Mat, Mat, Mat);

Mat drawAndCircleKeypoints(vector<KeyPoint>, Mat);

void threshDilateDetect(Mat, Mat, double, SimpleBlobDetector::Params, vector<KeyPoint>&);

void initializePointAndShoot();
void zoomInFull();
void zoomOutFull();
void shootPic();

bool beaconLocation(vector<KeyPoint>, beacon_loc*);

#endif

