#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "robot/Commands.h"
#include "robot/BeaconResponse.h"
#include "robot/BeaconRequest.h"
#include <cstdlib> 
#include <iostream>
#include <string>
#include <cmath>

// Flags
int state = 2; // 0: Startup, 1: Target seeking, 2: Returning, 3: BeaconApproach, 4: Paused 
double last_angle_from_beacon = 0;
double last_angle_from_robot = 180;
double last_distance_to_beacon = 12;
bool only_bottom_light = true;
bool beacon_found = true;
bool beacon_angle_conf = true;
bool waiting_on_command = false;
bool waiting_on_vision = false;
int command_timeout = 100; //TODO: Add command timeout

// Coordinates
double x_pos = 0;
double y_pos = 1;

void beaconCallback(const robot::BeaconResponse::ConstPtr& msg) {
	ROS_INFO("Response Recieved");
  if(msg->beacon_not_found) {
    beacon_found = false;
    beacon_angle_conf = false;
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
    }
  }
  waiting_on_vision = false;
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
      break;
    case 1: // Target Seeking
      break;
    case 2: // Returning
      b_msg.angle_min = -150;
      b_msg.angle_max = 150;
      while(beacon_request_pub.getNumSubscribers()<1)
	{loop_rate.sleep();}
      beacon_request_pub.publish(b_msg); // Look for the beacon
      ros::spinOnce();
      waiting_on_vision = true;
      ROS_INFO("Request Published");
      while(waiting_on_vision) {
	loop_rate.sleep(); // TODO: Add a timeout here
	ros::spinOnce();
      }
      if(beacon_found) {
	double angle;
	if(x_pos == 0) {
	  if(y_pos < 5) {
	    angle = last_angle_from_robot + 180;
	  } else {
	    angle = last_angle_from_robot; // In line with front of beacon. Avoid 1/0 error.
	  }	
	} else {
	  if(last_angle_from_beacon > 0){
	    angle = last_angle_from_robot + (90 - last_angle_from_beacon - (180/M_PI)*atan((double)(y_pos - 5)/x_pos));
	  } else {
	    angle = last_angle_from_robot - (90 + last_angle_from_beacon + (180/M_PI)*atan((double)(y_pos - 5)/x_pos));
	  }
	}
	double dist = sqrt(x_pos*x_pos + (y_pos-5)*(y_pos-5)); // Distance to staging area
	if(angle > 180) {
	  angle = 360 - angle;
	}

	ROS_INFO("INFO: (x,y) is (%f, %f)", x_pos, y_pos);
	ROS_INFO("Alpha is: (180/M_PI)*atan(%f)",(double)(y_pos - 5)/x_pos);
	ROS_INFO("Alpha is: %f",(180/M_PI)*atan((double)(y_pos - 5)/x_pos));
	ROS_INFO("Angle from robot is: %f", last_angle_from_robot);
	ROS_INFO("Beacon Found, staging at Angle: %f, Distance: %f", angle, dist);
	double driveDist = 0;
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
	  c_msg.commandOrder = 2; // Turning
	  c_msg.value = angle;
	  command_pub.publish(c_msg);
	  waiting_on_command = true;
	  ROS_INFO("Requesting Turn of %f degrees", angle);	  
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
    case 3: // BeaconApproach
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
	  c_msg.commandOrder = 1; // Drive up the ramp
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

