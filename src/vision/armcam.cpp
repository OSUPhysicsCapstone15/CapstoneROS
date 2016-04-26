#include "functions.h"
#include "blob.h"

void blob_main(sample_loc &s_loc)
{
    hsvParams hsvWhite = {0,0,200,180,90,255};
    hsvParams hsvPurple = {45,0,20,150,90,255};
    hsvParams hsv = s_loc.whiteSample==true? hsvWhite:hsvPurple;

    //Set up blob detection parameters
    SimpleBlobDetector::Params params = setupObjectBlobParams();

    vector<KeyPoint> keypoints;

    const string filename("/home/buckeye/catkin_ws/src/CapstoneROS/src/vision/closePurple.jpg");
    //Initialize camera
/*    VideoCapture cap(1);
    if ( !cap.isOpened() ){
        cout << "Cannot open the web cam" << endl;
        return -1;
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

        //convert color to HSV, threshold and remove noise
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        findGrass(img,imgHSV);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(hsv.hL, hsv.sL, hsv.vL), Scalar(hsv.hH, hsv.sH, hsv.vH), imgTHRESH);
        removenoise(imgTHRESH);

        namedWindow("Input", WINDOW_AUTOSIZE);
        namedWindow("Detection", WINDOW_AUTOSIZE);

        Ptr<SimpleBlobDetector> blobDetect = SimpleBlobDetector::create(params);
        blobDetect->detect( imgTHRESH, keypoints );

        drawKeypoints(imgTHRESH, keypoints, out, CV_RGB(0,0,255), DrawMatchesFlags::DEFAULT);
        //Circle blobs
        for(int i = 0; i < keypoints.size(); i++)
            circle(out, keypoints[i].pt, 1.5*keypoints[i].size, CV_RGB(0,255,0), 20, 8);

        if(keypoints.size() == 1){
            cout<<endl<<endl<<"Object Found"<<endl;

//find the difference in pixels of the center of the object and the center of the image for x
            float xDiff =  (float)img.cols/2.0 - keypoints[0].x;
//calc. the bottom of the blob
	    float yBound = keypoints[0].y + keypoints[0].size;

	//Find the distance between the sample and robot
	      float distToSample =4.42*yBound-.0527*yBound+21.88;
		cout<<"dist: "<<distToSample<<endl;

	     /* if(yBound > maxGrassBuffer){
		if(yBound < minGrassBuffer){
		   distToSample =0;
		}
		else{
		   distToSample = minGrassBuffer - yBound;
		}
	      }
	      else{
		distToSample = maxGrassBuffer - yBound
	      }*/
	
		

	//find the angle that the robot must turn so that the claw lines up on the sample
	    float xAngle = .0758 * xDiff - .2716;
	    cout<<"angle: "<<xAngle<<endl;
	    
	    
	
        }
        else{
            cout<<"No Object Found"<<endl;
        }

        imshow("Input", img);
        imshow("Detection", out);
        waitKey(-1);

    }
  }




