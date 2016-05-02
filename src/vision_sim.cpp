#include "ros/ros.h"
#include "robot/BeaconRequest.h"
#include "robot/BeaconResponse.h"
#include <cstdlib>
#include <iostream>
#include <string>

ros::Publisher response_pub; 

// ROS callback function for a beacon request being published
void requestCallback(const robot::BeaconRequest::ConstPtr& msg)
{
	ROS_INFO("Request Recieved");// Report a published beacon request 
	ROS_INFO("x: %f, y: %f, angle_from_robot: %f", msg->x, msg->y, msg->angle_from_robot);
	float angle_from_robot = 0.0; // Initialize the variables we are reporting back
	float x = 0.0;
	float y = 0.0;
	bool only_bottom = 0;
	bool beacon_not_found =0;
	bool beacon_angle_conf = 0;

	// Enter all the reported values from the command line
	std::cout << "Enter angle from robot: ";
	std::cin >> angle_from_robot;
	std::cout << "Enter x distance: ";
	std::cin >> x;
	std::cout << "Enter y distance: ";
	std::cin >> y;
	std::cout << "Enter 1 if only bottom, 0 if more than bottom: ";
	std::cin >> only_bottom;
	std::cout << "Enter 1 if beacon not found, 0 if beacon found: ";
	std::cin >> beacon_not_found;
	std::cout << "Enter 1 if confident in angle, 0 if not: ";
	std::cin >> beacon_angle_conf;

	// Assemble this new message, and report it back
	robot::BeaconResponse responseMsg;
	responseMsg.angle_from_robot=angle_from_robot;
	responseMsg.x = x;
	responseMsg.y = y;
	responseMsg.only_bottom = only_bottom;
	responseMsg.beacon_not_found = beacon_not_found;
	responseMsg.beacon_angle_conf = beacon_angle_conf;
	response_pub.publish(responseMsg);
	std::cout << "Value Published\n";
		
 
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "vision_sim"); // Initialize ROS
	ros::NodeHandle n;
	response_pub =n.advertise<robot::BeaconResponse>("BeaconResponse",1000); // Publish beacon response
	ros::Subscriber request_sub = n.subscribe("BeaconRequest",1000, requestCallback); // Subscribe to the beacon resquest
	ros::Rate loop_rate(10);
	ROS_INFO("Started"); // Now just keep running the program until terminated, and let the callback function take care of it
	while(ros::ok())
	{
	  ros::spinOnce();
	  loop_rate.sleep();
	}
	return 0;
	
}
		
