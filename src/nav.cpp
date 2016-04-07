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

static double Y_STAGE = 5; // Staging distance in front of beacon

// Flags
int state = 3; // 0: Startup, 1: Sample seeking, 2: Returning, 3: BeaconApproach, 4: Paused 

//variables for beacon and approach
double last_angle_from_beacon = 0;
double last_angle_from_robot = 180;
double last_distance_to_beacon = 12;
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
int seeking_state=0; //0 for pcs, 1 for easy, 2 for done
int looking_state=0; //0 for turn left, 1 turn right, 2 beacon look

//variables for traveling to area
double x_pcs = 100*0.3048;
double y_psc = 60*0.3048;
double x_easy = 10*0.3048;
double y_easy = 50*0.3048;

// Reported position
double x_pos = 0;
double y_pos = 1;

// Estimated position
double x_est = 0;
double y_est = 1;

void beaconCallback(const robot::BeaconResponse::ConstPtr& msg) {
	ROS_INFO("Beacon Response Recieved");
  if(msg->beacon_not_found) {
    beacon_found = false;
    beacon_angle_conf = false;
    ROS_INFO("Beacon Lost");
  } else {
    last_angle_from_beacon = msg->angle_from_beacon;
    last_angle_from_robot = msg->angle_from_robot;
    last_distance_to_beacon = msg->distance;
    only_bottom_light = msg->only_bottom;
    beacon_found = true;
    beacon_angle_conf = msg->beacon_angle_conf; 
    ROS_INFO("From beacon: %f, From robot: %f, Distance: %f", last_angle_from_beacon, last_angle_from_robot, last_distance_to_beacon);
    if(true){//beacon_angle_conf) {
      y_pos = last_distance_to_beacon * cos(last_angle_from_beacon * M_PI/180);  //Vision sends deg
      x_pos = last_distance_to_beacon * sin(last_angle_from_beacon * M_PI/180);
      y_est = y_pos;
      x_est = x_pos;
    }
  }
  waiting_on_vision = false;
}


void sampleCallback(const robot::SampleResponse::ConstPtr& msg)
{
	ROS_INFO("Sample Response Recieved");
	if (msg->sample_not_found)
	{
		sample_found=false;
		sample_angle_conf=false;
		ROS_INFO("Target not found");
	}
	else
	{
		sample_found = true;
		sample_angle_conf = msg-> sample_angle_conf;
		last_angle_from_sample = msg->angle_from_robot;
		last_distance_from_sample = msg->distance;
		ROSINFO("From Target: %f, distance: %f", last_angle_from_sample, last_distance_from_sample);
	}
}

void setState(const std_msgs::Int32::ConstPtr& msg) {
  state = msg->data;
}

void commandDone(const std_msgs::Int32::ConstPtr& msg) {
  waiting_on_command = false;
}


  
/**
 * Early prototype of the robot navigation
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
  
  // Subscribe to the "SetState" return from a beacon scan
  ros::Subscriber set_state_msg = n.subscribe("SetState", 1000, setState);
  ros::Subscriber command_done_msg = n.subscribe("CommandDone", 1000, commandDone);

  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  // Make a beacon request
  robot::BeaconRequest b_msg; // Defined in msg directory
  robot::Commands c_msg;
	ROS_INFO("Initializing Complete");
  
  while (ros::ok()) {
    

    switch(state) {
    case 0: // Startup
    	double x_to_travel=0;
    	double y_to_travel=0;
    	double driveDist=0;
    	double angle=0;
    	if(seeking_state=0){
    		x_to_travel = x_pcs;
    		y_to_travel =y_psc;
    	}
    	else{
    		x_to_travel = x_easy;
    		y_to_travel =y_easy;
    	}
  		while(x_est<x_to_travel){
    		c_msg.commandOrder = 1; // Driving
	    	if(dist > 10) {
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
	    	x_to_travel=x_to_Travel-driveDist;
	    	x_est = x_est+driveDist;
  		}
  		c_msg.commandOrder = 2; // Turning, then driving
	    c_msg.value = 90;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Turn of %f degrees", angle);	  
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
			while(y_est<y_to_travel){
    		c_msg.commandOrder = 1; // Driving
	    	if(dist > 10) {
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
	    	y_to_travel=y_to_Travel-driveDist;
	    	y_est=y_est+driveDist;
  		}	    
    }
    break;

    case 1: // Sample Seeking
    
			b_msg.angle_min=-150;
			b_msg.angle_max=150;
			if (sample_state==0){
				b_msg.whiteSample=true;
			}
			else{
				b_msg.whiteSample=false;
			}
	
			sample_request_pub.publish(b_msg);
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
				c_msg.commandOrder = 2; // Turning, then driving
	    	c_msg.value = last_angle_from_sample;
	    	command_pub.publish(c_msg);
	    	waiting_on_command = true;
	    	ROS_INFO("Requesting Turn of %f degrees", angle);	  
	    	while(waiting_on_command){
	      	ros::spinOnce();
	      	loop_rate.sleep(); // TODO: Add a timeout here
	    	}
	    	c_msg.commandOrder = 1; // Driving
	    	if(last_distance_from_sample > 10) {
	      	driveDist = 10;
	    	} else {
	      	driveDist = last_distance_from_sample-0.5;
	    	}
		    c_msg.value = driveDist;
	    	command_pub.publish(c_msg);
	    	waiting_on_command = true;
	    	ROS_INFO("Requesting Drive of %f meters", driveDist);
	    	while(waiting_on_command){
	      	ros::spinOnce();
	      	loop_rate.sleep(); // TODO: Add a timeout here
	    	}
			}
			else
			{
				if(looking_state==0){
					c_msg.commandOrder = 2; // Turning
	    		c_msg.value = 45;
	    		command_pub.publish(c_msg);
	    		waiting_on_command = true;
	    		ROS_INFO("Requesting Turn of %f degrees", angle);	  
	    		while(waiting_on_command){
	      		ros::spinOnce();
	      		loop_rate.sleep(); // TODO: Add a timeout here
	    		}
				}
				else if (looking_state==1)
				{
					c_msg.commandOrder = 2; // Turning
	    		c_msg.value = -90;
	    		command_pub.publish(c_msg);
	    		waiting_on_command = true;
	    		ROS_INFO("Requesting Turn of %f degrees", angle);	  
	    		while(waiting_on_command){
	      		ros::spinOnce();
	      		loop_rate.sleep(); // TODO: Add a timeout here
	    		}
				}
				else {
					  b_msg.angle_min = -150;
      			b_msg.angle_max = 150;
      			beacon_request_pub.publish(b_msg); // Look for the beacon
      			ros::spinOnce();
      			waiting_on_vision = true;
      			ROS_INFO("Request Published");
      			while(waiting_on_vision) {
							loop_rate.sleep(); // TODO: Add a timeout here
							ros::spinOnce();
      			}
				}
			}
      break;

    case 2: // Returning
      // Scanning range
      b_msg.angle_min = -150;
      b_msg.angle_max = 150;

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

	  // If we are confident in the orientation angle reported by vision
	  if(true){//beacon_angle_conf) {
	    // Calculate the angle and distance to the staging area
	    if(x_est == 0) { // This handles the special case of being directly in front of the beacon
	      if(y_est < Y_STAGE) { // Will need to turn around
		angle = last_angle_from_robot + 180;
	      } else {
		angle = last_angle_from_robot; // In line with front of beacon. Avoid 1/0 error.
	      }	
	    } else { // If we are confident in our coordinates
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

	  // Avoid excessive turns
	  if(angle > 180) {
	    angle = angle - 360;
	  }
	  if(angle < -180) {
	    angle = angle + 360;
	  }
	  
	  ROS_INFO("INFO: (x,y) is (%f, %f)", x_pos, y_pos);
	  ROS_INFO("Alpha is: (180/M_PI)*atan(%f)",(double)(y_pos - Y_STAGE)/x_pos);
	  ROS_INFO("Alpha is: %f",(180/M_PI)*atan((double)(y_pos - Y_STAGE)/x_pos));
	  ROS_INFO("Angle from robot is: %f", last_angle_from_robot);
	  ROS_INFO("Beacon Found, staging at Angle: %f, Distance: %f", angle, dist);
	  if(dist < 0.5) {
	    state = 3;
	    ROS_INFO("Changed to appraoch state");
	    break;
	  }
	  if(abs(angle)<5) {
	    c_msg.commandOrder = 1; // Driving
	    if(dist > 10) {
	      driveDist = 10;
	    } else {
	      driveDist = dist;
	    }
	    c_msg.value = driveDist;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Drive of %f meters", driveDist);
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	  } else {
	    c_msg.commandOrder = 2; // Turning, then driving
	    c_msg.value = angle;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Turn of %f degrees", angle);	  
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	    c_msg.commandOrder = 1; // Driving
	    if(dist > 10) {
	      driveDist = 10;
	    } else {
	      driveDist = dist;
	    }
	    c_msg.value = driveDist;
	    command_pub.publish(c_msg);
	    waiting_on_command = true;
	    ROS_INFO("Requesting Drive of %f meters", driveDist);
	    while(waiting_on_command){
	      ros::spinOnce();
	      loop_rate.sleep(); // TODO: Add a timeout here
	    }
	    
	  }
	} else { // We see the beacon, but only the bottom We will drive away Y_STAGE meters and try again
	  c_msg.commandOrder = 2; // Turning, then driving
	  c_msg.value = 180-last_angle_from_robot;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;
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
    case 3: // Beacon Approach
      b_msg.angle_min = -150;
      b_msg.angle_max = 150;
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
    case 4: // Paused
      break;
    } 
          
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
  }
  
  return 0;
}

