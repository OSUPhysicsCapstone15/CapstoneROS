#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Bool.h"
#include "robot/EncoderRequest.h"
#include "robot/LeftMotor.h"
#include "robot/RightMotor.h"
#include <cstdlib> 
#include <sstream>
#include <ctime>

// Some static constants
static const double MOTOR_MAX = 350;

// Global variables
bool paused = false;
int rightEncoder = 0;
int leftEncoder = 0;

void leftMotorCallback(const std_msgs::Float32::ConstPtr& msg)
{
  ROS_INFO("LeftMotorSet: [%f]", msg->data);
}

void rightMotorCallback(const std_msgs::Float32::ConstPtr& msg)
{
  ROS_INFO("RightMotorSet: [%f]", msg->data);
}

void pausedCallback(const std_msgs::Bool::ConstPtr& msg)
{
  ROS_INFO("Paused_state: [%d]", (int)(msg->data));
  paused = msg->data;
}

void leftEncoderCallback(const std_msgs::Int32::ConstPtr& msg)
{
  ROS_INFO("LeftEncoderReading: [%d]", msg->data);
  leftEncoder = msg->data;
}

void rightEncoderCallback(const std_msgs::Int32::ConstPtr& msg)
{
  ROS_INFO("RightEncoderReading: [%d]", msg->data);
  rightEncoder = msg->data;
}

/**
 * Early prototype of the robot main control, adapted from ROS tutorials
 */
int main(int argc, char **argv) {
  ROS_INFO("PROGRAM STARTED");

  std::time_t start_time = std::time(NULL); // Keeps track of time, clock_t is alias of some arithmetic type
  double total_time; // The current time in seconds since the start of the clock
  double last_update = 0; // Keep track of the last time since we made a command change
  
  // Initialize ROS, this must be done before using other ROS components
  ros::init(argc, argv, "robot");

  // Main access point to ROS. Makes a ROS node, which will shut down when this is destructed
  ros::NodeHandle n;

  // Publish topics "RightMotors" and "LeftMotors" of the msg variety of the same (singular) name, with a message queue of 1000
  ros::Publisher left_motor_pub = n.advertise<std_msgs::Float32>("LeftMotors", 1000);
  ros::Publisher right_motor_pub = n.advertise<std_msgs::Float32>("RightMotors", 1000);

  // Subscriber to motor return
  ros::Subscriber subLMotor = n.subscribe("LeftReturn", 1000, leftMotorCallback);
  ros::Subscriber subRMotor = n.subscribe("RightReturn", 1000, rightMotorCallback);
  ros::Subscriber subPaused = n.subscribe("Paused", 1000, pausedCallback);
  ros::Subscriber subLEncoder = n.subscribe("LeftEncoder", 1000, leftEncoderCallback);
  ros::Subscriber subREncoder = n.subscribe("RightEncoder", 1000, rightEncoderCallback);

  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  int count = 0;// Count the iterations, because why not
  double leftWheelSpeed = 0; // Keep track of the wheel speeds
  double rightWheelSpeed = 0;
  while (ros::ok()) {

    // First we update the total time
    total_time = (std::time(NULL) - start_time); // Get the time since start in seconds

    if (!paused) {
      // Update the motor set speeds
      if(total_time - last_update > 3.0) { // Switch every 5 seconds
	if(leftWheelSpeed == 0) { // These don't really have to be separate, but it makes it clear
	  leftWheelSpeed = 0.5;
	} else {
	  leftWheelSpeed = 0;
	}
	if(rightWheelSpeed == 0) {
	  rightWheelSpeed = 0.5;
	} else {
	  rightWheelSpeed = 0;
	}
	last_update = total_time;
      }
      ROS_INFO("Total time since first update: %f", (float)(total_time));
      ROS_INFO("Time since last update: %f", (float)last_update);
      // Pack the motor values into a message object
      std_msgs::Float32 right_msg; // Defined in msg directory
      std_msgs::Float32 left_msg;
      left_msg.data = leftWheelSpeed * MOTOR_MAX;
      right_msg.data = rightWheelSpeed * MOTOR_MAX;
      
      // Publish the motor speed message. Notice that the msg type matches the advertise template <>
      left_motor_pub.publish(left_msg); // Send the new speeds for the arduino to pick up.
      right_motor_pub.publish(right_msg); // Send the new speeds for the arduino to pick up.
      ROS_INFO("Published motor vals: %f,%f", (float)left_msg.data, (float)right_msg.data);
    }
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
    count++; // Increment count
  }

  return 0;
}

