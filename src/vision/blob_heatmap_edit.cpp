#include "functions.h"
#include "blob.h"

void blob_main(sample_loc &s_loc)
{
    hsvParams hsvWhite = {20,0,0,180,80,255};
    hsvParams hsvPurple = {80,60,0,130,255,255};
    
    // hsvParams hsvHeat = {0, 60, 0, 15, 255, 255};
    hsvParams hsvHeat_1 = {0, 127, 0, 15, 255, 255};
    hsvParams hsvHeat_2 = {150, 127, 0, 180, 255, 255};

    //Set up blob detection parameters
    SimpleBlobDetector::Params params = setupObjectBlobParams_heatmap();

    vector<KeyPoint> keypoints;

    const string filename("/home/buckeye/catkin_ws/src/CapstoneROS/src/vision/heatmaps/heatmap_5.jpg");
    //Initialize camera
    /*
    VideoCapture cap(0);
    if ( !cap.isOpened() ){
        cout << "Cannot open the web cam" << endl;
        return;
    }
    */

    while(true){
        // Mat img, imgHSV, imgTHRESH, out;
        Mat img, imgHSV, imgTHRESH, imgTHRESH_1, imgTHRESH_2, out;
		img = imread(filename, CV_LOAD_IMAGE_COLOR);
       // cap>>img;

        if(img.empty()){
            cout << "can not open image" << endl;
	    s_loc.sample_not_found=true;
            return;
        }
	
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(hsvHeat_1.hL, hsvHeat_1.sL, hsvHeat_1.vL), Scalar(hsvHeat_1.hH, hsvHeat_1.sH, hsvHeat_1.vH), imgTHRESH_1);
        // removenoise(imgTHRESH_1);
        inRange(imgHSV, Scalar(hsvHeat_2.hL, hsvHeat_2.sL, hsvHeat_2.vL), Scalar(hsvHeat_2.hH, hsvHeat_2.sH, hsvHeat_2.vH), imgTHRESH_2);
        // removenoise(imgTHRESH_2);
        imgTHRESH = imgTHRESH_1 + imgTHRESH_2;
        
        /*
        inRange(imgHSV, Scalar(hsvHeat.hL, hsvHeat.sL, hsvHeat.vL), Scalar(hsvHeat.hH, hsvHeat.sH, hsvHeat.vH), imgTHRESH);
        removenoise(imgTHRESH);
        */
        

        

        Ptr<SimpleBlobDetector> blobDetect = SimpleBlobDetector::create(params);
        blobDetect->detect(imgTHRESH, keypoints );

        drawKeypoints(imgTHRESH, keypoints, out, CV_RGB(0,0,255), DrawMatchesFlags::DEFAULT);
        //Circle blobs
        for(int i = 0; i < keypoints.size(); i++)
            circle(out, keypoints[i].pt, 1.5*keypoints[i].size, CV_RGB(0,255,0), 2, 8);
		int largestKey = 0;
        if(keypoints.size() == 1){
            cout<<endl<<endl<<"Object Found"<<endl;
            circle(out, keypoints[0].pt, 1.5*keypoints[0].size, CV_RGB(255,0,0), 2, 8);
            tilt_turn_degrees(imgTHRESH, keypoints[0].pt.y, keypoints[0].pt.x, &s_loc);
	    robot_angle(&s_loc, imgTHRESH, keypoints[0].pt.x);
        }
        else{
           // we find the blob that has the largest area, this is usually the one we are looking for
           // int largestKey = 0;
            int area = keypoints[0].size;
            for(int i = 0; i < keypoints.size(); i++){
				if (keypoints[i].size > area){
					largestKey = i;
					area = keypoints[i].size;
					}
			}
            // cout<<"No Object Found"<<endl;
            circle(out, keypoints[largestKey].pt, 1.5*keypoints[largestKey].size, CV_RGB(255,0,0), 2, 8);
            cout<<endl<<endl<<"Object Found?"<<endl;
            tilt_turn_degrees(imgTHRESH, keypoints[largestKey].pt.y, keypoints[largestKey].pt.x, &s_loc);
	    	robot_angle(&s_loc, imgTHRESH, keypoints[largestKey].pt.x);
        }

		Mat canny_output;
	  	vector<vector<Point> > contours;
	  	vector<Vec4i> hierarchy;
	  	int thresh = 100;
	  	/// Detect edges using canny
	  	Canny( imgTHRESH, canny_output, thresh, thresh*2, 3 );
	  	/// Find contours
	  	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	  	RNG rng(12345);
	  	int closest_point = 1000;
	  	int closest_contour_index=0;
	  	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	  	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	  	Rect bounding_rect;
	  	cout << "number of contours " << contours.size() << endl;
		 	for( int i = 0; i< contours.size(); i++ ) { // iterate through each contour. 
		   		int a=abs(contours[i][i].y - keypoints[largestKey].pt.y) + abs(contours[i][i].x - keypoints[largestKey].pt.x);  //  Find the closest contour to keypoint
		   		if(a<closest_point){
		   		closest_point=a;
		   		closest_contour_index=i;                //Store the index of largest contour
		   		bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
		   		}
	  	}
		   	cout << "closest point index "<< closest_contour_index << endl;
		   	drawContours( drawing, contours, closest_contour_index, color, 2, 8, hierarchy, 0, Point() );
		   	rectangle(drawing, bounding_rect, Scalar(0,255,0),1, 8,0); 
	  	cout << "width = " << bounding_rect.width << endl;
	  	//cout << "focal length = pixel_width * distance / actual_width = " << bounding_rect.width*6/.09 << endl;
	  	double focal_len = 1200;
	  	double actual_width = .09;
	  	double better_dist = focal_len * actual_width / bounding_rect.width;
	  	cout << "new dist = " << better_dist << " meters" << endl;
	  	/// Show in a window

        namedWindow("Input", WINDOW_AUTOSIZE);
        namedWindow("Detection", WINDOW_AUTOSIZE);
        // namedWindow("Contours", CV_WINDOW_AUTOSIZE );
        namedWindow("Filter", WINDOW_AUTOSIZE);
        namedWindow("Filter1", WINDOW_AUTOSIZE);
        namedWindow("Filter2", WINDOW_AUTOSIZE);
        
        imshow("Input", img);
		imshow("Detection", out);
        // imshow("Contours", drawing );
        imshow("Filter", imgTHRESH);
        imshow("Filter1", imgTHRESH_1);
        imshow("Filter2", imgTHRESH_2);
        
        // imwrite("/home/buckeye/catkin_ws/src/CapstoneROS/src/vision/heatmaps/filter_5.jpg",imgTHRESH);
        waitKey(-1);

    }
  }



