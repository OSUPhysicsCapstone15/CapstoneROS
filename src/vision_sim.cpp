#include "ros/ros.h"
#include "robot/BeaconRequest.h"
#include "robot/BeaconResponse.h"
#include <cstdlib>
#include <iostream>
#include <string>

ros::Publisher response_pub; 

void requestCallback(const robot::BeaconRequest::ConstPtr& msg)
{
	ROS_INFO("Request Recieved");
	ROS_INFO("%f", msg->angle_min);
	float angle_from_robot = 0.0;
	float distance =0.0;
	float angle_from_beacon =0.0;
	bool only_bottom = 0;
	bool beacon_not_found =0;
	bool beacon_angle_conf = 0;

	std::cout << "Enter angle from robot: ";
	std::cin >> angle_from_robot;
	std::cout << "Enter distance: ";
	std::cin >> distance;
	std::cout << "Enter angle_from_beacon: ";
	std::cin >> angle_from_beacon;
	std::cout << "Enter 1 if only bottom, 0 if more than bottom: ";
	std::cin >> only_bottom;
	std::cout << "Enter 1 if beacon not found, 0 if beacon found: ";
	std::cin >> beacon_not_found;
	std::cout << "Enter 1 if confident in angle, 0 if not: ";
	std::cin >> beacon_angle_conf;

	robot::BeaconResponse responseMsg;
	responseMsg.angle_from_robot=angle_from_robot;
	responseMsg.distance = distance;
	responseMsg.angle_from_beacon = angle_from_beacon;
	responseMsg.only_bottom = only_bottom;
	responseMsg.beacon_not_found = beacon_not_found;
	responseMsg.beacon_angle_conf = beacon_angle_conf;
	response_pub.publish(responseMsg);
	std::cout << "Value Published\n";
		
 
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "vision_sim");
	ros::NodeHandle n;
	response_pub =n.advertise<robot::BeaconResponse>("BeaconResponse",1000);
	ros::Subscriber request_sub = n.subscribe("BeaconRequest",1000, requestCallback);
	ros::Rate loop_rate(10);
	ROS_INFO("Started");
	while(ros::ok())
	{
	  ros::spinOnce();
	  loop_rate.sleep();
	}
	return 0;
	
}
		
