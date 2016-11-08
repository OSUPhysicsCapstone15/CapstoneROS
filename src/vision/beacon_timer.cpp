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
    
    //capture the video from web cam
    VideoCapture cap(0);
    
    //if not success, exit program
    if (!cap.isOpened())
    {
        cout << "Cannot open the web cam" << endl;
        return;
	}
	
	// declare a string for appending a prefix to the images we want to save
    std::string note;
    std::string foldername = "timerPics_11-3_b";    
    std::string term = "1";
    
    // get the string for the prefix
    cout << "File prefix for pictures: " << endl;
    cin >> note;
    
    while(term != "q")
    {
    	Mat noimg = getPic(cap); //first pic out of sync
    	// take four pictures
    	
    	Mat imgOriginal1 = getPic(cap);
    	Mat imgOriginal2 = getPic(cap); 
    	Mat imgOriginal3 = getPic(cap);
    	Mat imgOriginal4 = getPic(cap);
    	
    	/*
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
    auto t6 = std::chrono::high_resolution_clock::now();
    cout << "2nd " << std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count() << endl;
    cout << "3rd " << std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count() << endl;
    cout << "4th " << std::chrono::duration_cast<std::chrono::milliseconds>(t5-t4).count() << endl;
    cout << "5th " << std::chrono::duration_cast<std::chrono::milliseconds>(t6-t5).count() << endl;
    */
    
		if(imgOriginal1.empty() || imgOriginal2.empty() ||imgOriginal3.empty() ||imgOriginal4.empty())
		{
		    cout << "can not open " << endl;
		    return;
		}
		
		imwrite(foldername + "/" + note + "_" + term + "_img01.jpg", imgOriginal1);
		imwrite(foldername + "/" + note + "_" + term + "_img02.jpg", imgOriginal2);
		imwrite(foldername + "/" + note + "_" + term + "_img03.jpg", imgOriginal3);
		imwrite(foldername + "/" + note + "_" + term + "_img04.jpg", imgOriginal4);
		
		cout << "Next: ";
		cin >> term;
    }
    
	cout << "DONE!" << endl;
}

