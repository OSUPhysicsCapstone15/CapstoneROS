#include "functions.h"
#include "blob.h"

void blob_main(sample_loc &s_loc)
{
    hsvParams hsvWhite = {20,0,0,180,80,255};
    hsvParams hsvPurple = {80,60,0,130,255,255};
    
    hsvParams hsvHeat = {0, 60, 0, 15, 255, 255};

    // s_loc.whiteSample = false;
    // hsvParams hsv = s_loc.whiteSample==true? hsvWhite:hsvPurple;
    hsvParams hsv = hsvHeat;

    //Set up blob detection parameters
    SimpleBlobDetector::Params params = setupObjectBlobParams_heatmap();

    vector<KeyPoint> keypoints;

    const string filename("/home/buckeye/catkin_ws/src/CapstoneROS/src/vision/heatmaps/heatmap_1.jpg");
    //Initialize camera
    /*
    VideoCapture cap(0);
    if ( !cap.isOpened() ){
        cout << "Cannot open the web cam" << endl;
        return;
    }
    */

    while(true){
        Mat img, imgHSV, imgTHRESH, out;
		img = imread(filename, CV_LOAD_IMAGE_COLOR);
       // cap>>img;

        if(img.empty()){
            cout << "can not open image" << endl;
	    s_loc.sample_not_found=true;
            return;
        }
	
	// namedWindow("First", WINDOW_AUTOSIZE);
	// imshow("First", img);

        //convert color to HSV, threshold and remove noise
        /*
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        findGrass(img,imgHSV);
        */
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(hsv.hL, hsv.sL, hsv.vL), Scalar(hsv.hH, hsv.sH, hsv.vH), imgTHRESH);
        removenoise(imgTHRESH);

        namedWindow("Input", WINDOW_AUTOSIZE);
        namedWindow("Detection", WINDOW_AUTOSIZE);
        namedWindow("Filter", WINDOW_AUTOSIZE);

        Ptr<SimpleBlobDetector> blobDetect = SimpleBlobDetector::create(params);
        blobDetect->detect(imgTHRESH, keypoints );

        drawKeypoints(imgTHRESH, keypoints, out, CV_RGB(0,0,255), DrawMatchesFlags::DEFAULT);
        //Circle blobs
        for(int i = 0; i < keypoints.size(); i++)
            circle(out, keypoints[i].pt, 1.5*keypoints[i].size, CV_RGB(0,255,0), 20, 8);

        if(keypoints.size() == 1){
            cout<<endl<<endl<<"Object Found"<<endl;
            tilt_turn_degrees(imgTHRESH, keypoints[0].pt.y, keypoints[0].pt.x, &s_loc);
	    robot_angle(&s_loc, imgTHRESH, keypoints[0].pt.x);
        }
        else{
            int lowKey = 0;
            int lowVal = keypoints[0].pt.y;
            for(int i = 0; i < keypoints.size(); i++){
				if (keypoints[i].pt.y > lowVal){
					lowKey = i;
					lowVal = keypoints[i].pt.y;
					}
			}
            // cout<<"No Object Found"<<endl;
            circle(out, keypoints[lowKey].pt, 1.5*keypoints[lowKey].size, CV_RGB(255,0,0), 20, 8);
            cout<<endl<<endl<<"Object Found?"<<endl;
            tilt_turn_degrees(imgTHRESH, keypoints[lowKey].pt.y, keypoints[lowKey].pt.x, &s_loc);
	    	robot_angle(&s_loc, imgTHRESH, keypoints[lowKey].pt.x);
        }

        imshow("Input", img);
        imshow("Filter", imgTHRESH);
        // imwrite("/home/buckeye/catkin_ws/src/CapstoneROS/src/vision/heatmaps/filter_1.jpg",imgTHRESH);
        imshow("Detection", out);
        waitKey(-1);

    }
  }



