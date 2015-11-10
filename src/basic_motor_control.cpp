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
#include <cmath>

using namespace std;

// Some static constants
static const double MOTOR_MAX = 350;
static const double PIVOT_SPEED = 0.6;
static const double ANGLE_PRECISION = 5; // Units of degrees
static const int PULSE_RATIO = 600;
static const double WHEEL_DIAMETER = 12; // Inches
static const double WHEEL_BASE = 42; // Inches
// Global variables
bool paused = false;
int rightEncoder = 0;
int leftEncoder = 0;
double leftWheelSpeed = 0; // Keep track of the wheel speeds
double rightWheelSpeed = 0;
double current_angle = 0; // The current angle of the robot (relative)
double target_angle = 0; // The current target angle
double left_encoder_zeropoint = 0; // Zeropoints for the encoders
double right_encoder_zeropoint = 0;
// Flags
bool turning = false; // Whether or not we should be executing a turn

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
  //ROS_INFO("LeftEncoderReading: [%d]", msg->data);
  leftEncoder = msg->data;
}

void rightEncoderCallback(const std_msgs::Int32::ConstPtr& msg)
{
  //ROS_INFO("RightEncoderReading: [%d]", msg->data);
  rightEncoder = msg->data;
}

// Distance is in inches
double enc2distance(double enc) {
  return (enc/PULSE_RATIO)*3.14159*WHEEL_DIAMETER;
}

// Degrees
double enc2angle(double encL, double encR) {
  double diff = encL - encR;
  return 360.0 * enc2distance(diff)/((2*3.14159)*WHEEL_BASE);
}

// Sets motor values, returns true if at destination
bool pivotOnWheel(double *leftWheelSpeed, double *rightWheelSpeed, double target, double current) {
  double diff = target - current;
  if(abs(diff) < ANGLE_PRECISION) {
    *rightWheelSpeed = 0;
    *leftWheelSpeed = 0;
    return 1;
  }
  if(diff < 0) {
    *rightWheelSpeed = PIVOT_SPEED;
    *leftWheelSpeed = 0;
  } else {
    *leftWheelSpeed = PIVOT_SPEED;
    *rightWheelSpeed = 0;
  }
}

// Reset local states
void zero_system() {
  left_encoder_zeropoint = leftEncoder;
  right_encoder_zeropoint = rightEncoder;
  current_angle = 0;
  target_angle = 0;
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

  ros::spinOnce(); // Allow values to update

  // Begin stopped
  rightWheelSpeed = 0;
  leftWheelSpeed = 0;

  zero_system();
  while (ros::ok()) {
      ROS_INFO("Current angle: %f", current_angle);
      ROS_INFO("Target angle: %f", target_angle);

      // First we update the total time
      total_time = (std::time(NULL) - start_time); // Get the time since start in seconds

      if (!paused) {
	
	// Turn 45 degrees every 10 seconds
	if(total_time - last_update > 10.0) { // Switch every 5 seconds
	  zero_system();
	  target_angle = 45;
	  last_update = total_time;
	  turning = true;
	}
	ROS_INFO("Total time since first update: %f", (float)(total_time));
	ROS_INFO("Time since last update: %f", (float)last_update);
	
	if(turning) {
	  turning  = !(pivotOnWheel(&leftWheelSpeed, &rightWheelSpeed, target_angle, current_angle));
	  ROS_INFO("Trying to turn: %f %f", leftWheelSpeed, rightWheelSpeed);
	}
	
	current_angle = enc2angle(leftEncoder-left_encoder_zeropoint, rightEncoder - right_encoder_zeropoint);
	
	// Values decided, pass to arduinos
	// Pack the motor values into a message object
	std_msgs::Float32 right_msg; // Defined in msg directory
	std_msgs::Float32 left_msg;
	left_msg.data = leftWheelSpeed * MOTOR_MAX;
	right_msg.data = rightWheelSpeed * MOTOR_MAX;
	
	// Publish the motor speed message. Notice that the msg type matches the advertise template <>
	left_motor_pub.publish(left_msg); // Send the new speeds for the arduino to pick up.
	right_motor_pub.publish(right_msg); // Send the new speeds for the arduino to pick up.
	ROS_INFO("Published motor vals: %f,%f", (float)left_msg.data, (float)right_msg.data);
	ROS_INFO("---------------------------------------------------------------------");
      }
      ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
  }
  
  return 0;
}

// OLD CONTROL SCHEMES
      /*
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
      */
