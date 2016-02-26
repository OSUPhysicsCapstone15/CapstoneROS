#include "beacon.h"
#include "functions.h"


beacon_loc beacon_main(float min, float max)
{
    beacon_loc b_loc;
    createWindows();

    //Set up blob detection parameters
    SimpleBlobDetector::Params params;
//    setupBlobParameters(params);
  
    params.minDistBetweenBlobs = 10.0f;
    params.filterByInertia = true;
    params.filterByConvexity = false;
    params.filterByColor = false;
    params.filterByCircularity = false;
    params.filterByArea = true;

    params.minThreshold = 100;
    params.maxThreshold = 255;
    params.thresholdStep = 1;
    params.minArea = 0;
    params.minConvexity = 0.50;
    params.minInertiaRatio = 0.50;
    params.maxArea = 2000;
    params.maxConvexity = 10;

    vector<KeyPoint> keypoints;

    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return b_loc;
    }

//    checkCameraOpen(cap);

    Mat imgOriginal1 = getPic(cap);
    Mat imgOriginal2 = getPic(cap);
    Mat imgOriginal3 = getPic(cap);
    Mat imgOriginal4 = getPic(cap);

    if(imgOriginal1.empty() || imgOriginal2.empty() ||imgOriginal3.empty() ||imgOriginal4.empty())
    {
       cout << "can not open " << endl;
       return b_loc;
    }


//    checkImgFull(imgOriginal1);
//    checkImgFull(imgOriginal2);
//    checkImgFull(imgOriginal3);
//    checkImgFull(imgOriginal4);

    //initialize mats
    Mat diff,diff1,diff2,grayDiff,binDiff;

    //diff1 = difference between img1 and img2
    absdiff(imgOriginal1,imgOriginal2,diff1);

    //diff2 = difference between img1 and img2
    absdiff(imgOriginal3,imgOriginal4,diff2);

    //logical AND diff1 and diff2 to eliminate people behind beacon
    bitwise_and(diff1,diff2,diff);

    //convert to grayscale
    cvtColor(diff, grayDiff, COLOR_BGR2GRAY); //Convert the captured

    //threshold and dilate
    double thresh = 200.0;
    threshold(grayDiff, binDiff, thresh, 255, cv::THRESH_BINARY);
    dilate(binDiff, binDiff, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //detect beacon blobs between pictures 1&2
    Ptr<SimpleBlobDetector> blobDetect = SimpleBlobDetector::create(params);
    blobDetect->detect( binDiff, keypoints );

	while(keypoints.size()>4 && thresh<=255){
		thresh+=5;
	  	threshold(grayDiff, binDiff, thresh, 255, cv::THRESH_BINARY);
	  	dilate(binDiff, binDiff, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		blobDetect = SimpleBlobDetector::create(params);
	   	blobDetect->detect( binDiff, keypoints );
	}
	while(keypoints.size()<4 && thresh>=100){
		thresh-=5;
	  	threshold(grayDiff, binDiff, thresh, 255, cv::THRESH_BINARY);
	  	dilate(binDiff, binDiff, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		blobDetect = SimpleBlobDetector::create(params);
	   	blobDetect->detect( binDiff, keypoints );
	}
	if(thresh<100){
	        b_loc.beacon_not_found = 1;
		return b_loc;
	}
	while(keypoints.size()>4 && thresh<=255){
		thresh+=.25;
  		threshold(grayDiff, binDiff, thresh, 255, cv::THRESH_BINARY);
  		dilate(binDiff, binDiff, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		blobDetect = SimpleBlobDetector::create(params);
	   	blobDetect->detect( binDiff, keypoints );
	}

	diff=binDiff;

    cout<<thresh<<endl;
    cout<<keypoints.size()<<endl;

    Mat out;
    drawKeypoints(diff, keypoints, out, CV_RGB(0,0,0), DrawMatchesFlags::DEFAULT);

    //Circle blobs
    for(int i = 0; i < keypoints.size(); i++)
    {
  	if(keypoints[i].size>0)
      	   circle(out, keypoints[i].pt, 1.5*keypoints[i].size, CV_RGB(0,0,255), 5, 8);
    }
    string text;
    if(keypoints.size() == 4)
    {
        text = "Object Found";
      //cout<<endl<<endl<<"Object Found"<<endl;
	b_loc.beacon_not_found = 0;
	b_loc.only_bottom = 0;
      Point cent;
      cent=findkeyPoint(keypoints);
 //     cout<<"dist: "<<printDistanceFromLights(keypoints)<<endl;
	 printDistanceFromLights(keypoints, &b_loc);
      circle(out, cent, 5, CV_RGB(0,100,0), -1, 8);
      robot_angle(diff, cent.y, cent.x, 1, &b_loc);
    }else if(keypoints.size() == 1){
		b_loc.beacon_not_found = 0;
		b_loc.only_bottom = 1;
		robot_angle(diff, keypoints.front().pt.y, keypoints.front().pt.x, 1, &b_loc);
	}
    else
    {
      text = "Error";
      //cout<<endl<<endl<<"No Object Found"<<endl;
	b_loc.beacon_not_found = 1;
  //	while(keypoints.size() > 2)
  //	   thresh+=5;
    }
    imshow("Original 1", imgOriginal1); //show the original image
    imshow("Original 2", imgOriginal2); //show the original image
    imshow("Original 3", imgOriginal3); //show the original image
    imshow("Original 4", imgOriginal4); 
    imshow("Diff1", diff1 );
    imshow("Diff2", diff2 );
    imshow("bit_and", out);
    waitKey(20);
  
return b_loc;
}
