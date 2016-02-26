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



void removenoise(Mat&);

Mat findBiggestBlob(Mat &src);

void findGrass(Mat &src, Mat &HSV);

void robot_angle(Mat img, int object_rows, int object_cols, bool targetIsBeacon, beacon_loc* orientation);

Point findkeyPoint(vector<KeyPoint> keypoints);

int blob_main();

int beacon_main();

void printDistanceFromLights(vector<KeyPoint> keypoints, beacon_loc* orientation);

Mat getPic(VideoCapture cap);

void createWindows();

void setupBlobParameters(SimpleBlobDetector::Params);

void checkCameraOpen(VideoCapture);

void checkImgFull(Mat);


#endif

