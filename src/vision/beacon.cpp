#include "beacon.h"
#include "functions.h"

beacon_loc beacon_main(float min, float max)
{
    beacon_loc b_loc;
    createWindows();

    //Set up blob detection parameters
    SimpleBlobDetector::Params params = setupBeaconBlobParams();

    //capture the video from web cam
    VideoCapture cap(0);

    //if not success, exit program
    if (!cap.isOpened())
    {
        cout << "Cannot open the web cam" << endl;
        return b_loc;
    }

    Mat imgOriginal1 = getPic(cap);
    Mat imgOriginal2 = getPic(cap);
    Mat imgOriginal3 = getPic(cap);
    Mat imgOriginal4 = getPic(cap);

    if(imgOriginal1.empty() || imgOriginal2.empty() ||imgOriginal3.empty() ||imgOriginal4.empty())
    {
        cout << "can not open " << endl;
        return b_loc;
    }

    //initialize mats
    Mat diff_and,diff1,diff2,grayDiff,binDiff;

    //diff1 = difference between img1 and img2
    absdiff(imgOriginal1,imgOriginal2,diff1);

    //diff2 = difference between img3 and img4
    absdiff(imgOriginal3,imgOriginal4,diff2);

    //logical AND diff1 and diff2 to eliminate people behind beacon
    bitwise_and(diff1,diff2,diff_and);

    //convert to grayscale
    cvtColor(diff_and, grayDiff, COLOR_BGR2GRAY); //Convert the captured

    //try to fill keypoints with exactly four points
    double thresh = 200.0;
    vector<KeyPoint> keypoints(0);

    threshold(grayDiff, binDiff, thresh, 255, THRESH_BINARY);
    dilate(binDiff, binDiff, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    Ptr<SimpleBlobDetector> blobDetect = SimpleBlobDetector::create(params);
    blobDetect->detect(binDiff, keypoints);

    cout << keypoints.size() << endl;

    while(keypoints.size() > 4 && thresh <= 255)
    {
        thresh += 5;
        threshDilateDetect(grayDiff, binDiff, thresh, params, keypoints);
    }

    cout << keypoints.size() << endl;

    while(keypoints.size() < 4 && thresh >= 50)
    {
        thresh -= 5;
        threshDilateDetect(grayDiff, binDiff, thresh, params, keypoints);
    }

    if(thresh < 50)
    {
	cout << "Failed to find beacon" << endl;
        b_loc.beacon_not_found = 1;
        return b_loc;
    }

    cout << keypoints.size() << endl;

    while(keypoints.size() > 4 && thresh <= 255)
    {
        thresh += .25;
        threshDilateDetect(grayDiff, binDiff, thresh, params, keypoints);
    }

    cout << thresh << endl;
    cout << keypoints.size() << endl;

    //draw important points for output
    Mat out = drawAndCircleKeypoints(keypoints, binDiff);

    if(keypoints.size() == 4)
    {
        b_loc.beacon_not_found = 0;
        b_loc.only_bottom = 0;

        Point cent = findCenterPoint(keypoints);

	beaconLocation(keypoints, &b_loc);

	getBeaconOrientation(keypoints, &b_loc);
        printDistanceFromLights(keypoints, &b_loc);
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

    //output windows to view
    showWindows(imgOriginal1, imgOriginal2, imgOriginal3, imgOriginal4, diff1, diff2, out);

    return b_loc;
}

