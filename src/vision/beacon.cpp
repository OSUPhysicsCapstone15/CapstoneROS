#include "beacon.h"
#include "functions.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

const float WIDTH = 5.015625; //meters
const float HEIGHT = 5.96875; //meters

//returns true if success, false otherwise
bool beaconLocation(vector<KeyPoint> keyPoints, beacon_loc *b_loc) {
	//convert keypoints to point2f points
	vector<Point2f> imgPoints;
	KeyPoint::convert(keyPoints, imgPoints);

	//fill known points with beacon dimensions
	vector<Point3f> kwnPoints = {Point3f(0, HEIGHT/2, 0), //Top
								Point3f(-WIDTH/2, 0, 0),  //Left
								Point3f(WIDTH/2, 0, 0), //Right
								Point3f(0, -HEIGHT/2, 0) //Bottom
	}; 

	//get saved calibration matrix 
	string filename = "out_camera_data.xml";
	FileStorage fs(filename, FileStorage::READ);
	if(!fs.isOpened()) {
		cout<<"Calibration file could not be opened"<<endl;
		return false;
	}
	Mat cameraMatrix, distCoeffs;
	fs["Camera_Matrix"]>>cameraMatrix;
	fs["Distortion_Coefficients"]>>distCoeffs;
	if(cameraMatrix.empty() || distCoeffs.empty()) {
		cout<<"Calibration file not formatted correctly"<<endl;
		return false;
	} else {
		cout<<"Camera Matrix"<<endl;
		cout<<cameraMatrix<<endl;
		cout<<"Distortion Coefficients"<<endl;
		cout<<distCoeffs<<endl;
	}

	cout<<"Known Points"<<endl;
	cout<<kwnPoints<<endl;
	cout<<"Image Points"<<endl;
	cout<<imgPoints<<endl;

	//get roation-translation matrix
	Mat rvec, tvec;
	solvePnP(Mat(kwnPoints), Mat(imgPoints), cameraMatrix, distCoeffs, rvec, tvec, false);

	//print out stuff for sanity check
	cout<<"Rotation vector"<<endl;
	cout<<rvec<<endl;
	cout<<"Translation vector"<<endl;
	cout<<tvec<<endl;

	//fill beacon struct with appropriate values
	//TODO

	return true;

}

beacon_loc beacon_main(float min, float max)
 {
	beacon_loc b_loc;
  double thresh=200.;
  namedWindow("Original 1", WINDOW_NORMAL);
  namedWindow("Original 2", WINDOW_NORMAL);
  namedWindow("Original 3", WINDOW_NORMAL);
  namedWindow("Original 4", WINDOW_NORMAL);
  namedWindow("Diff1", WINDOW_NORMAL);
  namedWindow("Diff2", WINDOW_NORMAL);
  namedWindow("bit_and", WINDOW_NORMAL);

  //hsvParams hsv = {76,0,224,97,37,255};
  hsvParams hsv = {20,0,0,97,37,255};

  //Set up blob detection parameters
  SimpleBlobDetector::Params params;
 // params.blobColor //can we use this???
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



    Mat imgOriginal1 = getPic(cap);
    Mat imgOriginal2 = getPic(cap);
    Mat imgOriginal3 = getPic(cap);
    Mat imgOriginal4 = getPic(cap);
    Mat imgHSV1,imgHSV2, imgHSV3;


/*///////////////////////////////////////////////////////////////////////////////////////////////
    cvtColor(bgr_image, hsv_image, COLOR_BGR2HSV);
    // Threshold the HSV image, keep only the red pixels
    Mat lower_red_hue_range;
    Mat upper_red_hue_range;
    inRange(hsv_image, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
    inRange(hsv_image, Sc  namedWindow("Original 3", WINDOW_NORMAL);
alar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);
////////////////////////////////////////////////////////////////////////////////////////////////
*/
    if(imgOriginal1.empty() || imgOriginal2.empty() || imgOriginal3.empty() || imgOriginal4.empty())
    {
       cout << "can not open " << endl;
       return b_loc;
    }

    Mat diff,diff1,diff2,grayDiff,binDiff;
    absdiff(imgOriginal1,imgOriginal2,diff1);
    absdiff(imgOriginal3,imgOriginal4,diff2);
    bitwise_and(diff1,diff2,diff);

    cvtColor(diff, grayDiff, COLOR_BGR2GRAY); //Convert the captured

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
	while(keypoints.size()<4 && thresh>=0){
		thresh-=5;
	  	threshold(grayDiff, binDiff, thresh, 255, cv::THRESH_BINARY);
	  	dilate(binDiff, binDiff, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		blobDetect = SimpleBlobDetector::create(params);
	   	blobDetect->detect( binDiff, keypoints );
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
  /*//finding if things are green or red
  cvtColor(out, out, COLOR_BGR2HSV);
  inRange(out, Scalar(hsv.hL, hsv.sL, hsv.vL),
         Scalar(hsv.hH, hsv.sH, hsv.vH), out);
  blobDetect.detect( out, keypoints );
  drawKeypoints(out, keypoints, out, CV_RGB(0,0,0), DrawMatchesFlags::DEFAULT);

  for(int i=0;i<diff.rows;i++){
     for(int j=0;j<diff.cols;j++){
            if(out.at<Vec3b>(i,j)[0]==0 && out.at<Vec3b>(i,j)[1]==0 && out.at<Vec3b>(i,j)[2]==0){
                imgOriginalON.at<Vec3b>(i,j)=(0,0,0);
            }
       }
    }
    inRange(imgOriginalON, Scalar(hsv.hL, hsv.sL, hsv.vL),
         Scalar(hsv.hH, hsv.sH, hsv.vH), out);
    blobDetect.detect( out, keypoints );
    drawKeypoints(out, keypoints, out, CV_RGB(0,0,0), DrawMatchesFlags::DEFAULT);
    */

    //keypoints=getTop4Keypoints(keypoints);

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
	 //printDistanceFromLights(keypoints, &b_loc);
      circle(out, cent, 5, CV_RGB(0,100,0), -1, 8);
      //robot_angle(diff, cent.y, cent.x, 1, &b_loc);
	if(!beaconLocation(keypoints, &b_loc)) {
		cout<<"Error in finding beacon location"<<endl;
		return b_loc;
	}
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
