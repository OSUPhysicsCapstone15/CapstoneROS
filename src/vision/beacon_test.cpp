#include "beacon.h"
#include "functions.h"

void beacon_main(beacon_loc &b_loc)
{


    //Set up blob detection parameters
    SimpleBlobDetector::Params params = setupBeaconBlobParams();
    /* commented out by Kaeli for testing purposes 
    //capture the video from web cam
	VideoCapture cap(0);
	
    //if not success, exit program
    if (!cap.isOpened())
    {
        cout << "Cannot open the web cam" << endl;
        return;    
        }
        
        
    /*    
    Mat img;
    for (int i = 0; i < 100; i++ ) {
   
    auto t = std::chrono::high_resolution_clock::now();
    auto t0 = std::chrono::high_resolution_clock::now();
    while( std::chrono::duration_cast<std::chrono::milliseconds>(t0-t).count() < 200) {
    	t0 = std::chrono::high_resolution_clock::now();
    }
    cap >> img;
    cout << std::chrono::duration_cast<std::chrono::milliseconds>(t0-t).count() << endl;
    }
    */
    /*
    //t1-t4 added by Kaeli, on Nov 1 for testing
	Mat imgOriginal1, imgOriginal2, imgOriginal3, imgOriginal4;
    auto t1 = std::chrono::high_resolution_clock::now();
    imgOriginal1 = getPic(cap); // imread("testPics2/imgOriginal1.jpg", CV_LOAD_IMAGE_COLOR);
    auto t2 = std::chrono::high_resolution_clock::now();
    imgOriginal2 = getPic(cap); //imread("testPics2/imgOriginal2.jpg", CV_LOAD_IMAGE_COLOR); //getPic(cap);
    auto t3 = std::chrono::high_resolution_clock::now();
    imgOriginal3 = getPic(cap); //imread("testPics2/imgOriginal3.jpg", CV_LOAD_IMAGE_COLOR); //getPic(cap);
    auto t4 = std::chrono::high_resolution_clock::now();
    imgOriginal4 = getPic(cap); //imread("testPics2/imgOriginal4.jpg", CV_LOAD_IMAGE_COLOR); //getPic(cap);

    cout << "time difference was " <<endl;
    cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << endl;
    cout << std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count() << endl;
    cout << std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count() << endl;
    */
    /*
    Mat img;//testing purposes (Kaeli)
    Mat img2;//testing purposes (Kaeli)
    //timing for loop (comment out if not testing timing specifically)
    
    for (int n=1; n<100; n++)
      {
	auto t1 = std::chrono::high_resolution_clock::now();
	img = getPic(cap);
	auto t2 = std::chrono::high_resolution_clock::now();
	cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << endl;
	}
	*/
    /* 
    auto t6 = std::chrono::high_resolution_clock::now();
    cout << "first diff not used "<< std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << endl;
    cout << "2nd " << std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count() << endl;
    cout << "3rd " << std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count() << endl;
    cout << "4th " << std::chrono::duration_cast<std::chrono::milliseconds>(t5-t4).count() << endl;
    cout << "5th " << std::chrono::duration_cast<std::chrono::milliseconds>(t6-t5).count() << endl;
    */
    
    /*			********** BEST CODE *********** */
    /*
    VideoCapture cap(-1);
	
    //if not success, exit program
    if (!cap.isOpened())
    {
        cout << "Cannot open the web cam" << endl;
        return;    
    } 
    auto t1 = std::chrono::high_resolution_clock::now();
	Mat noimg = getPic(cap); //first pic out of sync
	auto t2 = std::chrono::high_resolution_clock::now();
    Mat imgOriginal1 = getPic(cap);
    auto t3 = std::chrono::high_resolution_clock::now();
    Mat imgOriginal2 = getPic(cap);
    auto t4 = std::chrono::high_resolution_clock::now();
    Mat imgOriginal3 = getPic(cap);
    auto t5 = std::chrono::high_resolution_clock::now();
    Mat imgOriginal4 = getPic(cap);	
    
    */
    
    string test;
    cout << "Name of picture set: ";
    cin >> test;
    
	Mat imgOriginal1 = imread("11-18-16 atrium testing/" + test + "_img01.jpg");
	Mat imgOriginal2 = imread("11-18-16 atrium testing/" + test + "_img02.jpg");
	Mat imgOriginal3 = imread("11-18-16 atrium testing/" + test + "_img03.jpg");
	Mat imgOriginal4 = imread("11-18-16 atrium testing/" + test + "_img04.jpg");
	
	/*
	Mat imgOriginal1 = imread("11-3-16 atrium testing/" + test + "_imgOriginal1.jpg");
	Mat imgOriginal2 = imread("11-3-16 atrium testing/" + test + "_imgOriginal2.jpg");
	Mat imgOriginal3 = imread("11-3-16 atrium testing/" + test + "_imgOriginal3.jpg");
	Mat imgOriginal4 = imread("11-3-16 atrium testing/" + test + "_imgOriginal4.jpg");
	*/
    
    if(imgOriginal1.empty() || imgOriginal2.empty() ||imgOriginal3.empty() ||imgOriginal4.empty())
    {
        cout << "can not open " << endl;
        return;
    }

    //initialize mats
    Mat diff_and,diff1,diff2,grayDiff,binDiff;

    //diff1 = difference between img1 and img2
    absdiff(imgOriginal1,imgOriginal2,diff1);

    //diff2 = difference between img3 and img4
    absdiff(imgOriginal3,imgOriginal4,diff2);

    //logical AND diff1 and diff2 to eliminate people behind beacon
    bitwise_and(diff1,diff2,diff_and);
    
    /* 
    	Instead of converting to grayscale immediately, we may want to filter out
    	any non-green colors. This would minimize the number of keypoints, and 
    	then repeated threshing won't accidentally remove the keypoints we want.
    */
    
    
    //convert to grayscale
    cvtColor(diff_and, grayDiff, COLOR_BGR2GRAY); //Convert the captured

    //try to fill keypoints with exactly four points
    double thresh = 150.0;
    vector<KeyPoint> keypoints(0);

    threshold(grayDiff, binDiff, thresh, 255, THRESH_BINARY);
    dilate(binDiff, binDiff, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    Ptr<SimpleBlobDetector> blobDetect = SimpleBlobDetector::create(params);
    blobDetect->detect(binDiff, keypoints);
    
    /*
    
    String foldername = "test_11-3_atrium_different distances/";
    std::string note = "";
	cout << "File prefix for pictures: " << endl;
    cin >> note;
    
    imwrite(foldername + note + "_imgOriginal1.jpg", imgOriginal1);
    imwrite(foldername + note  + "_imgOriginal2.jpg", imgOriginal2);
    imwrite(foldername + note  + "_imgOriginal3.jpg", imgOriginal3);
    imwrite(foldername + note  + "_imgOriginal4.jpg", imgOriginal4);
    
    */
    
    cout << "Keypoints size: " << keypoints.size() << endl;

    while(keypoints.size() > 4 && thresh <= 255)
    {
        thresh += 5;
        threshDilateDetect(grayDiff, binDiff, thresh, params, keypoints);
    }

    cout << "Keypoints size: " << keypoints.size() << endl;

    while(keypoints.size() < 4 && thresh >= 50)
    {
        thresh -= 5;
        threshDilateDetect(grayDiff, binDiff, thresh, params, keypoints);
    }

    if(thresh < 50)
    {
	cout << "Failed to find beacon" << endl;
        b_loc.beacon_not_found = 1;
        return;
    }

    cout << "Keypoints size: " << keypoints.size() << endl;

    while(keypoints.size() > 4 && thresh <= 255)
    {
        thresh += .25;
        threshDilateDetect(grayDiff, binDiff, thresh, params, keypoints);
    }

    cout << "Thresh: " << thresh << endl;
    cout << "Keypoints size: " << keypoints.size() << endl;

    //draw important points for output
    Mat out = drawAndCircleKeypoints(keypoints, binDiff);

    if(keypoints.size() == 4)
    {
        b_loc.beacon_not_found = 0;
        b_loc.only_bottom = 0;

        Point cent = findCenterPoint(keypoints);

		beaconLocation(keypoints, &b_loc);

		// getBeaconOrientation(keypoints, &b_loc);
		// printDistanceFromLights(keypoints, &b_loc);
		robot_angle(&b_loc, binDiff, cent.x);

		circle(out, cent, 5, CV_RGB(0,100,0), -1, 8);
    }
    else if(keypoints.size() == 1)
    {
        b_loc.beacon_not_found = 0;
        b_loc.only_bottom = 1;

        robot_angle(&b_loc, binDiff, keypoints.front().pt.x);
    }
    else
    {
        b_loc.beacon_not_found = 1;
    }
    
    /*
    
    imwrite(foldername + note  + "_diff1.jpg", diff1);
    imwrite(foldername + note  + "_diff2.jpg", diff2);
    imwrite(foldername + note  + "_out.jpg", out);

	*/
	
	cout << "HERE!" << endl;
    //output windows to view
    createWindows();
    showWindows(imgOriginal1, imgOriginal2, imgOriginal3, imgOriginal4, diff1, diff2, grayDiff, out);
}

