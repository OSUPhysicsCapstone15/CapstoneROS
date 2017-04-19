#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "robot/Commands.h"
#include "robot/BeaconResponse.h"
#include "robot/BeaconRequest.h"
#include "robot/SampleRequest.h"
#include "robot/SampleResponse.h"
#include <cstdlib> 
#include <iostream>
#include <string>
#include <cmath>

static double Y_STAGE = 0; // Staging distance in front of beacon

// Flags
int state = 2; // -1; Platform dismount 0: Startup, 1: Sample seeking, 2: Returning, 3: BeaconApproach, 4: Paused 

//variables for beacon and approach
double last_angle_from_beacon = 0; // 180 means facing the beacon
double last_angle_from_robot = 0;    // WTF IS THIS???
double last_distance_to_beacon = 0;
bool only_bottom_light = true;
bool beacon_found = true;
bool beacon_angle_conf = true;
bool waiting_on_command = false;
bool waiting_on_vision = false;
int command_timeout = 100; //TODO: Add command timeout
bool move_to_stage = false;

//variables for sample seeking
double last_angle_from_sample =0;
double last_distance_from_sample=0;
bool sample_found=false;
bool sample_angle_conf;
int seeking_state=2; //0 for pcs, 1 for easy, 2 for done
int looking_state=0; //0 for turn left, 1 turn right, 2 beacon look

//variables for turning camera
bool camera_success;

//variables for traveling to area
double x_pcs = 103*0.3048; // Precached sample
double y_psc = 63*0.3048;
double x_easy = 5*0.3048; // Easy sample
double y_easy = 45*0.3048;

// Reported position
double x_pos = 0.5;
double y_pos = 2;

// Estimated position
double x_est = 0.5;
double y_est = 2;

// Callback function that records newest beacon location data
void beaconCallback(const robot::BeaconResponse::ConstPtr& msg) {
  ROS_INFO("Beacon Response Recieved");
  if(msg->beacon_not_found) { // If the beacon isn't found, mark it as lost
    beacon_found = false;
    beacon_angle_conf = false;
    ROS_INFO("Beacon Lost");
  } else { // If the beacon is found, record its position data
    last_angle_from_robot = msg->angle_from_robot;
    only_bottom_light = msg->only_bottom;
    beacon_found = true;
    beacon_angle_conf = msg->beacon_angle_conf; 
    ROS_INFO("From robot: %f, Distance: %f", last_angle_from_robot, last_distance_to_beacon);
    if(true){//beacon_angle_conf) {
      y_pos = msg->y;//last_distance_to_beacon * cos(last_angle_from_beacon * M_PI/180);  //Vision sends degrees, so we convert to radians
      x_pos = msg->x;//last_distance_to_beacon * sin(last_angle_from_beacon * M_PI/180);
      last_angle_from_beacon = (180/M_PI)*atan(y_pos/x_pos);
      if(y_pos < 0) {
	if(last_angle_from_beacon < 0) {
	  last_angle_from_beacon += 180;
	} else {
	  last_angle_from_beacon -= 180;
	}
      }
      last_distance_to_beacon = sqrt(x_pos*x_pos + y_pos*y_pos);
      y_est = y_pos;
      x_est = x_pos;
    }
  }
  waiting_on_vision = false; // Stop waiting for vision to respond
}

// Callback function for sample location
void sampleCallback(const robot::SampleResponse::ConstPtr& msg)
{
  ROS_INFO("Sample Response Recieved");
  if (msg->sample_not_found) // If the target isn't found, mark it as such
    {
      sample_found=false;
      sample_angle_conf=false;
      ROS_INFO("Target not found");
    }
  else // Otherwise mark the sample as found, and record its positions
    {
      sample_found = true;
      sample_angle_conf = msg-> sample_angle_conf;
      last_angle_from_sample = msg->angle_from_robot;
      last_distance_from_sample = msg->distance;
      ROS_INFO("From Target: %f, distance: %f", last_angle_from_sample, last_distance_from_sample);
    }
}

void cameraCallback(const robot::CameraResponse::ConstPtr& msg)
{
  ROS_INFO("Camera Response Recieved");
  if (msg->camera_not_found)
    {
      camera_success=false;
      ROS_INFO("Camera error");
    }
  else
    {
      camera_success=true;
      ROS_INFO("Camera successfully turned");
    }
}

// An external callback that can set the state of the system
void setState(const std_msgs::Int32::ConstPtr& msg) {
  state = msg->data;
}

// Callback function to report a command finshing
void commandDone(const std_msgs::Int32::ConstPtr& msg) {
  waiting_on_command = false;
}


  
/**
 * Robot navigation
 */
int main(int argc, char **argv) {
  ROS_INFO("Initializing...");
  // Initialize ROS, this must be done before using other ROS components
  ros::init(argc, argv, "navigation");
  
  // Main access point to ROS. Makes a ROS node, which will shut down when this is destructed
  ros::NodeHandle n;

  // Publish a topic "BeaconRequest", used to ask for a scan for the Beacon
  ros::Publisher beacon_request_pub = n.advertise<robot::BeaconRequest>("BeaconRequest", 1000);

  // Publish a topic "BeaconRequest", used to ask for a scan for the Beacon
  ros::Publisher command_pub = n.advertise<robot::Commands>("Commands", 1000);
  
  // Subscribe to the "BeaconResponse" return from a beacon scan
  ros::Subscriber beacon_response_msg = n.subscribe("BeaconResponse", 1000, beaconCallback);

  ros::Publisher sample_request_pub = n.advertise<robot::SampleRequest>("SampleRequest",1000);


  ros::Subscriber sample_response_msg = n.subscribe("SampleResponse",1000,sampleCallback);
  
  ros::Publisher camera_request_pub = n.advertise<robot::CameraRequest>("CameraRequest",1000);
  ros::Subscriber camera_response_msg = n.subscribe("CameraResponse",1000,cameraCallback);

  // Subscribe to the "SetState" return from a beacon scan
  ros::Subscriber set_state_msg = n.subscribe("SetState", 1000, setState);
  ros::Subscriber command_done_msg = n.subscribe("CommandDone", 1000, commandDone);

  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  // Make a beacon request
  robot::BeaconRequest b_msg; // Defined in msg directory
  robot::Commands c_msg;
  robot::SampleRequest s_msg;
  robot::CameraRequest camera_msg;

  // Scanning range
  b_msg.x = x_est;
  b_msg.y = y_est;
  b_msg.angle_from_robot = last_angle_from_robot;
  // Wait until someone is listening to vision requests
  while(beacon_request_pub.getNumSubscribers()<1) {
    loop_rate.sleep();
  }
  
  /* Request a scan for the beacon
  beacon_request_pub.publish(b_msg); // Look for the beacon
  ros::spinOnce();
  waiting_on_vision = true;
  ROS_INFO("Request Published");
  while(waiting_on_vision) {
    loop_rate.sleep(); // TODO: Add a timeout here
    ros::spinOnce();
  }
	*/
  ros::spinOnce();
  ROS_INFO("Initializing Complete");

  while (ros::ok()) {

    switch(state) {
    case -1: // Dismount
      {
      ROS_INFO("DISMOUNT");
      c_msg.commandOrder = 1; // Drive
      c_msg.value = 5.0; // 5 meters
      command_pub.publish(c_msg);
      waiting_on_command = true;
      ROS_INFO("Requesting Drive of %f meters", 5.0);
      while(waiting_on_command) {
	ros::spinOnce();
	//ROS_INFO("Exiting Platform");
	loop_rate.sleep(); // TODO: Add a timeout here
      }
      state = 0;
      break;
      }
    case 0: // Startup
      {
      ROS_INFO("STARTUP");
      double x_to_travel=0;
      double y_to_travel=0;
      double driveDist=0;
      double angle=0;
      c_msg.value = 180;
      c_msg.commandOrder = 2;
      command_pub.publish(c_msg);
      waiting_on_command = true;
      ROS_INFO("Requesting turn of %f degrees", 180.0);
      while(waiting_on_command){
	ros::spinOnce();
	loop_rate.sleep(); // TODO: Add a timeout here
      }
      b_msg.x = x_est;
      b_msg.y = y_est;
      b_msg.angle_from_robot = 0;
      beacon_request_pub.publish(b_msg); // Look for the beacon
      waiting_on_vision = true;
      while(waiting_on_vision) {
	ros::spinOnce();
	loop_rate.sleep(); // TODO: Add a timeout here
      }
      if(beacon_found) { // Turn to face away from the beacon
	c_msg.value = 180 + last_angle_from_robot;
	c_msg.commandOrder = 2;
	command_pub.publish(c_msg);
	waiting_on_command = true;
	ROS_INFO("Requesting Drive of %f degrees", c_msg.value);
	while(waiting_on_command){
	  ros::spinOnce();
	  loop_rate.sleep(); // TODO: Add a timeout here
	}
      } else { // beacon not found, so we turn to find it
	c_msg.value = 45;
	c_msg.commandOrder = 2;
	command_pub.publish(c_msg);
	waiting_on_command = true;
	ROS_INFO("Requesting Drive of %f meters", driveDist);
	while(waiting_on_command){
	  ros::spinOnce();
	  loop_rate.sleep(); // TODO: Add a timeout here
	}	
	continue; // Now we try again at the top of the loop
      }
      //set distance to travel to get to sample area
      if(seeking_state=0){
	x_to_travel = x_pcs;
	y_to_travel =y_psc;
      }
      else{
	x_to_travel = x_easy;
	y_to_travel =y_easy;
      }
      //drive over to sample area
      while(x_est<x_to_travel){
	c_msg.commandOrder = 1; // Driving
	if(x_to_travel > 10) {
	  driveDist = 10;
	} else {
	  driveDist = x_to_travel;
	}
	c_msg.value = driveDist;
	command_pub.publish(c_msg);
	waiting_on_command = true;
	ROS_INFO("Requesting Drive of %f meters", driveDist);
	while(waiting_on_command){
	  ros::spinOnce();
	  loop_rate.sleep(); // TODO: Add a timeout here
	}
	x_est = x_est+driveDist;
      }
      //turn left (?) 90 degrees
      c_msg.commandOrder = 2; // Turning to go up
      c_msg.value = -90;
      command_pub.publish(c_msg);
      waiting_on_command = true;
      ROS_INFO("Requesting Turn of %f degrees", angle);	  
      while(waiting_on_command){
	ros::spinOnce();
	loop_rate.sleep(); // TODO: Add a timeout here
      }
      //drive up to sample area
      while(y_est<y_to_travel){
	c_msg.commandOrder = 1; // Driving
	if(y_to_travel > 10) {
	  driveDist = 10;
	} else {
	  driveDist = y_to_travel;
	}
	c_msg.value = driveDist;
	command_pub.publish(c_msg);
	waiting_on_command = true;
	ROS_INFO("Requesting Drive of %f meters", driveDist);
	while(waiting_on_command){
	  ros::spinOnce();
	  loop_rate.sleep(); // TODO: Add a timeout here
	}
	y_est=y_est+driveDist;
      }
      
      c_msg.commandOrder = 2; // turning 45 degrees to face circle
      c_msg.value = 45;
      command_pub.publish(c_msg);
      waiting_on_command = true;
      ROS_INFO("Requesting Turn of %f degrees", angle);	  
      while(waiting_on_command){
	ros::spinOnce();
	loop_rate.sleep(); // TODO: Add a timeout here
      }
      state = 1; //change to seeking state
      ROS_INFO("Changed to seeking state");
      break;
      }
    case 1: // Sample Seeking
      {
      ROS_INFO("LOOKING FOR SAMPLE");
      //look for sample
      s_msg.angle_min=-150;
      s_msg.angle_max=150;
      if (seeking_state==0){
	s_msg.whiteSample=true;
      }
      else{
	s_msg.whiteSample=false;
      }
      
      sample_request_pub.publish(s_msg);
      ros::spinOnce();	
      waiting_on_vision = true;
      ROS_INFO("Sample Request Published");
      while(waiting_on_vision)
	{
	  loop_rate.sleep();
	  ros::spinOnce();
	}
      
      if(sample_found)
	{
	  c_msg.commandOrder = 2; // Turn toward sample
	  c_msg.value = last_angle_from_sample;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;
	  ROS_INFO("Requesting Turn of %f degrees", last_angle_from_sample);	  
	  while(waiting_on_command){
	    ros::spinOnce();
	    loop_rate.sleep(); // TODO: Add a timeout here
	  }
	  if(last_distance_from_sample < .2) { // We are right in front of it
	    c_msg.commandOrder = 3; // Grab the sample
	    c_msg.value = 0;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting a grab");	  
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	    state = 2; // Go home
	    seeking_state++; // Move on to next step
	  } else { // Otherwise drive closer
	    c_msg.commandOrder = 1; // Drive to sample
	    if(last_distance_from_sample > 10) {
	      c_msg.value = 10;
	    } else {
	      c_msg.value = last_distance_from_sample-0.5; //How far to be from sample?
	    }
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Drive of %f meters", c_msg.value);
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	  }
	}
      
      //sample not found
      else
	{
	  //turn 45 degrees and look for sample
	  if(looking_state==0){
	    c_msg.commandOrder = 2; // Turning
	    c_msg.value = 45;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Turn of %f degrees", c_msg.value);	  
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	    looking_state=1; //increase looking state
	  }
	  //turn 90 degrees oppposite direction and look
	  else if (looking_state==1)
	    {
	      c_msg.commandOrder = 2; // Turning
	      c_msg.value = -90;
	      command_pub.publish(c_msg);
	      waiting_on_command = true;
	      ROS_INFO("Requesting Turn of -90 degrees");	  
	      while(waiting_on_command){
		ros::spinOnce();
		loop_rate.sleep(); // TODO: Add a timeout here
	      }
	      looking_state=2;
	    }
	  //look for beacon and make sure in correct spot
	  else {
	    b_msg.x = x_est;
	    b_msg.y = y_est;
	    b_msg.angle_from_robot = last_angle_from_robot;
	    beacon_request_pub.publish(b_msg); // Look for the beacon
	    ros::spinOnce();
	    waiting_on_vision = true;
	    ROS_INFO("Request Published");
	    while(waiting_on_vision) {
	      loop_rate.sleep(); // TODO: Add a timeout here
	      ros::spinOnce();
	    }
	    state = 0; //back to startup state to make sure in correct area
	    ROS_INFO("Changed to startup state");
	    looking_state=0;
	  }
	}
      break;
      }
    case 2: // Returning
      {
      // #################
      /* CHange these for different tests, this is for straight on return 
      x_est = 0.01;
      y_est = 10;
      last_angle_from_robot = 180;
      // Testing calibration ends 
      */
            
      ROS_INFO("RETURNING TO BEACON");
      // Scanning range
      b_msg.x = x_est;
      ROS_INFO("X Estimate: [%f]",(float) x_est);
      b_msg.y = y_est;   
      ROS_INFO("Y Estimate: [%f]", (float)y_est);
      b_msg.angle_from_robot = last_angle_from_robot;
      ROS_INFO("Angle Estimate: [%f]", (float)last_angle_from_robot);   
      // Wait until someone is listening to vision requests
      while(beacon_request_pub.getNumSubscribers()<1) {
	loop_rate.sleep();
      }
      
      // Request a scan for the beacon
      beacon_request_pub.publish(b_msg); // Look for the beacon
      ros::spinOnce();
      waiting_on_vision = true;
      ROS_INFO("Request Published");
      while(waiting_on_vision) {
	loop_rate.sleep(); // TODO: Add a timeout here
	ros::spinOnce();
      }

      // Scan has returned, begin moving
      if(beacon_found) {
	double driveDist = 0;
	double angle = 0;
	double dist = last_distance_to_beacon;

	// If we are far enough away we see the entire beacon
	if(!only_bottom_light) {

	  /* If we are confident in the orientation angle reported by vision
	  if(true){//beacon_angle_conf) {
	    /* Calculate the angle and distance to the staging area
	    if(x_est == 0) { // This handles the special case of being directly in front of the beacon
	      if(y_est < Y_STAGE) { // Will need to turn around
		angle = last_angle_from_robot + 180;
	      } 
		angle = last_angle_from_robot; // In line with front of beacon. Avoid 1/0 error.
	    } /*else { // If we are confident in our coordinates
	      if(last_angle_from_beacon > 0){
		angle = last_angle_from_robot + (90 - last_angle_from_beacon - (180/M_PI)*atan((double)(y_pos - Y_STAGE)/x_pos));
	      } else {
		angle = last_angle_from_robot + (-90 - last_angle_from_beacon - (180/M_PI)*atan((double)(y_pos - Y_STAGE)/x_pos));
	      }
	    }
	    dist = sqrt(x_pos*x_pos + (y_pos-Y_STAGE)*(y_pos-Y_STAGE));
	  } else { // We are not confident in our orientation
	    int z = 1;
	  }
		*/
	  // Avoid excessive turns
	  if(last_angle_from_robot > 180) {
	    last_angle_from_robot = last_angle_from_robot - 360;
	  }
	  if(last_angle_from_robot < -180) {
	    last_angle_from_robot = last_angle_from_robot + 360;
	  }
	  
	  ROS_INFO("INFO: (x,y) is (%f, %f)", x_pos, y_pos);
	  ROS_INFO("Alpha is: (180/M_PI)*atan(%f)",(double)(y_pos)/x_pos);
	  ROS_INFO("Alpha is: %f",(180/M_PI)*atan((double)(y_pos)/x_pos));
	  ROS_INFO("Angle from robot is: %f", last_angle_from_robot);
	  //ROS_INFO("Beacon Found, staging at Angle: %f, Distance: %f", angle, dist);
	  if(dist < 0.5) {
	    if(seeking_state > 1) {
	      state = 4;
	    } else {
	      state = 0;
	    }
	    ROS_INFO("Changed to appraoch state");
	    break;
	  }
	  if(abs(last_angle_from_robot)<5) {
	    c_msg.commandOrder = 1; // Driving
	    if(dist > 10) {
	      driveDist = 10;
	    } else {
	      driveDist = dist-1;
	    }
	    c_msg.value = driveDist;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Drive of %f meters", driveDist);
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	    if(last_distance_to_beacon-driveDist<2){
	    	state=4;
	    }
	  } else {
	    c_msg.commandOrder = 2; // Turning, then driving
	    c_msg.value = last_angle_from_robot ;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Turn of %f degrees", last_angle_from_robot);	  
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	    c_msg.commandOrder = 1; // Driving
	    if(dist > 10) {
	      driveDist = 10;
	    } else {
	      driveDist = dist-1;
	    }
	    c_msg.value = driveDist;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Drive of %f meters", driveDist);
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	    if(last_distance_to_beacon-driveDist<2){
	    	state=4;
	    }
	  }
	} else { // We see the beacon, but only the bottom We will stop
	  /*c_msg.commandOrder = 2; // Turning, then driving
	  c_msg.value = 180-last_angle_from_robot;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;*/
	  ROS_INFO("Only bottom");
	  ROS_INFO("Stop");
	  state = 4;
	 	/*
	  ROS_INFO("Requesting Turn of %f degrees", 180-last_angle_from_robot); // Back Up	  
	  while(waiting_on_command){
	    ros::spinOnce();
	    loop_rate.sleep(); // TODO: Add a timeout here
	  }
	  c_msg.commandOrder = 1; // Driving
	  driveDist = Y_STAGE-0.5;
	  c_msg.value = driveDist;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;
	  ROS_INFO("Requesting Drive of %f meters", driveDist);
	  while(waiting_on_command){
	    ros::spinOnce();
	    loop_rate.sleep(); // TODO: Add a timeout here
	  } 
	  c_msg.commandOrder = 2; // Turning, then driving
	  c_msg.value = 180;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;
	  ROS_INFO("Requesting Turn of %f degrees", 180.0); // Back Up	  
	  while(waiting_on_command){
	    ros::spinOnce();
	    loop_rate.sleep(); // TODO: Add a timeout here
	  }
	*/
	}

      } else { // Lost the beacon, start spinning and searching
	c_msg.commandOrder = 2; // Turning
	c_msg.value = 35;
	command_pub.publish(c_msg);
	waiting_on_command = true;
	ROS_INFO("Beacon lost, turning");
	while(waiting_on_command){
	  ros::spinOnce();
	  loop_rate.sleep(); // TODO: Add a timeout here
	}
	
      }
      break;
      }
    case 3: // Beacon Approach
      {
      ROS_INFO("GET BACK ON BEACON");
      b_msg.x = x_est;
      b_msg.y = y_est;
      b_msg.angle_from_robot = 0; // Should be changed
      beacon_request_pub.publish(b_msg); // Look for the beacon
      waiting_on_vision = true;
      while(waiting_on_vision) {
	ros::spinOnce();
	loop_rate.sleep(); // TODO: Add a timeout here
      }
      if(beacon_found) {
	double angle = last_angle_from_robot;
	if(abs(angle) > 10) {
	  ROS_INFO("Turning to approach");
	  c_msg.commandOrder = 2; // Turning
	  c_msg.value = angle;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;
	  while(waiting_on_command){
	    ros::spinOnce();
	    loop_rate.sleep(); // TODO: Add a timeout here
	  }	  
	} else {
	  ROS_INFO("Approaching");
	  c_msg.commandOrder = 4; // Drive up the ramp (high speed)
	  c_msg.value = 5.2;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;
	  while(waiting_on_command){
	    ros::spinOnce();
	    loop_rate.sleep(); // TODO: Add a timeout here
	  }
	  state = 4;
	}
      } else {
	c_msg.commandOrder = 2; // Turning
	c_msg.value = 180;
	command_pub.publish(c_msg);
	waiting_on_command = true;
	while(waiting_on_command){
	  ros::spinOnce();
	  loop_rate.sleep(); // TODO: Add a timeout here
	}	
      }
      break;
      }
    case 4: // Paused
      {
      ROS_INFO("We Here Yo");
      break;
      }
    } 
          
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
  }
  
  return 0;
}

