#include "ros/ros.h"
#include "robot/Commands.h"
#include <cstdlib> 
#include <iostream>
#include <string>

/**
 * Robot control console
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

  
  while (ros::ok()) { // Unil ROS is shut down

    std::string command; // The command read in
    int commandOrder = 0; // The command ID to publish
    double val = 0; // The command value to publish

    // Continually 
    while (ros::ok()) {
      // Ask for a command
      std::cout << "Enter Command:\n";
      std::cin >> command;

      // Check what command it is, act accordingly
      if( command == "drive") { // Move the robot forward
	std::cout << "Enter number of meters:\n";
	commandOrder = 1;
	std::cin >> val;
      } else if( command == "drivefast") { // Move the robot at high speed forward
	std::cout << "Enter number of meters:\n";
	commandOrder = 4;
	std::cin >> val;
      } else if( command == "turn"){ // Piviot on a wheel
	std::cout << "Enter number of degrees:\n";
	commandOrder = 2;
	std::cin >> val;
      } else if ( command == "grab" ) { // Tell the arm to grab
	std::cout << "Executing grab\n";
	commandOrder = 3;
      } else if ( command == "turnplace" ) { // Turn, pivoting about the center of the wheel base
	std::cout << "Enter number of degrees\n";
	commandOrder = 5;
	std::cin >> val;
      } else if (command == "exit") { // Exit the terminal
	return 0;
      } else { // Snark at people who aren't careful
	std::cout << "Really? Why. That's not even a command.\n";
	continue;
      }

      // Pack the command values into a message object
      robot::Commands msg; // Defined in msg directory
      msg.commandOrder = commandOrder; // The message ID
      msg.value = val; // The value, if needed (if not, whatever gets put in here doesn't matter)
      command_pub.publish(msg); // Publish the command to the system
      std::cout << "I just published " << command << " " << val << "\n"; // Let the user know it happened
    }
    
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
  }

  return 0;
}

