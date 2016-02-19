#include "ros/ros.h"
#include "robot/Commands.h"
#include <cstdlib> 
#include <iostream>
#include <string>

/**
 * Early prototype of the robot control console
 */
int main(int argc, char **argv) {
 
  // Initialize ROS, this must be done before using other ROS components
  ros::init(argc, argv, "mastermind");

  // Main access point to ROS. Makes a ROS node, which will shut down when this is destructed
  ros::NodeHandle n;

  // Publish a topic "Motors" of the "Motors" msg variety, with a message queue of 1000
  ros::Publisher command_pub = n.advertise<robot::Commands>("Commands", 1000);
  
  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  
   while (ros::ok()) {

    std::string command;
    int commandOrder = 0;
    double val = 0;

    while (1) {
      std::cout << "Enter Command:\n";
      std::cin >> command;
      //std::getline(std::cin, command);
      if( command == "drive") {
	std::cout << "Enter number of meters:\n";
	commandOrder = 1;
	std::cin >> val;
      } else if( command == "drivefast") {
	std::cout << "Enter number of meters:\n";
	commandOrder = 4;
	std::cin >> val;
      } else if( command == "turn"){
	std::cout << "Enter number of degrees:\n";
	commandOrder = 2;
	std::cin >> val;
      } else if ( command == "grab" ) {
	std::cout << "Executing grab\n";
	commandOrder = 3;
      } else if (command == "exit") {
	return 0;
      } else {
	std::cout << "Really? Why. That's not even a command.\n";
	continue;
      }

      // Pack the command values into a message object
      robot::Commands msg; // Defined in msg directory
      msg.commandOrder = commandOrder;
      msg.value = val;
      command_pub.publish(msg);
      std::cout << "I just published " << command << " " << val << "\n";
    }
    
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
  }

  return 0;
}

