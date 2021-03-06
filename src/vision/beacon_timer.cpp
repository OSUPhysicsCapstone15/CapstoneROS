#include "beacon.h"
#include "functions.h"
#include <iostream>
#include <string>

void beacon_main(beacon_loc &b_loc)
{
    
    /* 
    	This program takes four consecutive pictures and saves them, and then awaits 
    	console input. This input will be used in naming the saved files. The loop
    	(and program) will terminate when the input is "q".
    */
	
	// declare a string for appending a prefix to the images we want to save
    std::string note;
    std::string foldername = "test_2-15-17";
    
    // get the string for the prefix
    cout << "File prefix for pictures: (dist-16,5) or (dist-16,5-4,5) NO SPACES" << endl;
    // (dist-16,5-4,5) references distance 16.5 m and distance to the side 4.5 m for finding angles
    cin >> note;
    
    while(note != "q")
    {
    	//capture the video from web cam
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
    
		// check if images open
		if(imgOriginal1.empty() || imgOriginal2.empty() ||imgOriginal3.empty() ||imgOriginal4.empty())
		{
		    cout << "can not open " << endl;
		    return;
		}
		
		// save the four images using the prefix "note"
		imwrite(foldername + "/" + note + "_img01.jpg", imgOriginal1);
		imwrite(foldername + "/" + note + "_img02.jpg", imgOriginal2);
		imwrite(foldername + "/" + note + "_img03.jpg", imgOriginal3);
		imwrite(foldername + "/" + note + "_img04.jpg", imgOriginal4);
		
		// close the webcam, opens at beginning of while-loop
    	cap.release();
		
		// get the prefix for the next set of pictures
		cout << "File prefix for pictures: " << endl;
    	cin >> note;
    }
    
	cout << "DONE!" << endl;
}

