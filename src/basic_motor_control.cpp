#include "ros/ros.h"
#include "std_msgs/String.h"
#include "robot/EncoderRequest.h"
#include "robot/LeftMotor.h"
#include "robot/RightMotor.h"
#include <cstdlib> 
#include <sstream>
#include <ctime>

// Some constants
static const double MOTOR_MAX = 350;


void leftMotorCallback(const robot::LeftMotor::ConstPtr& msg)
{
  ROS_INFO("LeftMotorSet: [%f]", msg->leftMotor);
}

void rightMotorCallback(const robot::RightMotor::ConstPtr& msg)
{
  ROS_INFO("RightMotorSet: [%f]", msg->rightMotor);
}


/**
 * Early prototype of the robot main control, adapted from ROS tutorials
 */
int main(int argc, char **argv) {

  std::time_t start_time = std::time(NULL); // Keeps track of time, clock_t is alias of some arithmetic type
  double total_time; // The current time in seconds since the start of the clock
  double last_update = 0; // Keep track of the last time since we made a command change
  
  // Initialize ROS, this must be done before using other ROS components
  ros::init(argc, argv, "robot");

  // Main access point to ROS. Makes a ROS node, which will shut down when this is destructed
  ros::NodeHandle n;

  // Publish topics "RightMotors" and "LeftMotors" of the msg variety of the same (singular) name, with a message queue of 1000
  ros::Publisher left_motor_pub = n.advertise<robot::LeftMotor>("LeftMotors", 1000);
  ros::Publisher right_motor_pub = n.advertise<robot::RightMotor>("RightMotors", 1000);

  // Subscriber to motor return
  ros::Subscriber subL = n.subscribe("LeftReturn", 1000, leftMotorCallback);
  ros::Subscriber subR = n.subscribe("RightReturn", 1000, rightMotorCallback);

  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  
  int count = 0;// Count the iterations, because why not
  double leftWheelSpeed = 0; // Keep track of the wheel speeds
  double rightWheelSpeed = 0;
  while (ros::ok()) {

    // First we update the total time
    total_time = (std::time(NULL) - start_time); // Get the time since start in seconds

    // Update the motor set speeds
    if(total_time - last_update > 5.0) { // Switch every 5 seconds
      if(leftWheelSpeed == 0) { // These don't really have to be separate, but it makes it clear
	leftWheelSpeed = 0.9;
      } else {
	leftWheelSpeed = 0;
      }
      if(rightWheelSpeed == 0) {
	rightWheelSpeed = 0.9;
      } else {
	rightWheelSpeed = 0;
      }
      last_update = total_time;
    }
    ROS_INFO("Total time since first update: %f", (float)(total_time));
    ROS_INFO("Time since last update: %f", (float)last_update);
    // Pack the motor values into a message object
    robot::RightMotor right_msg; // Defined in msg directory
    robot::LeftMotor left_msg;
    left_msg.leftMotor = leftWheelSpeed * MOTOR_MAX;
    right_msg.rightMotor = rightWheelSpeed * MOTOR_MAX;
    
    // Publish the motor speed message. Notice that the msg type matches the advertise template <>
    left_motor_pub.publish(left_msg); // Send the new speeds for the arduino to pick up.
    right_motor_pub.publish(right_msg); // Send the new speeds for the arduino to pick up.
    ROS_INFO("Published motor vals: %f,%f", (float)left_msg.leftMotor, (float)right_msg.rightMotor);
    
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
    count++; // Increment count
  }

  return 0;
}

