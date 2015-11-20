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
#include <chrono>

using namespace std;
// using namespace std::chrono;

// Some static constants
static const double MOTOR_MAX = 350;
static const double PIVOT_SPEED = 0.5;
static const double BREAK_SPEED = -0.25; // Reverse with enough power to stop wheel motion
static const double ANGLE_PRECISION = 5; // Units of degrees
static const double FORWARD_PRECISION = 12; // Units of inches
static const int PULSE_RATIO = 600;
static const double WHEEL_DIAMETER = 12; // Inches
static const double WHEEL_BASE = 42; // Inches
static const double K_p = 1; // PID proportional constant
static const double K_i = 1; // PID integral constant
// Global variables
bool paused = false;
int rightEncoder = 0;
int leftEncoder = 0;
int heartbeat = 1;
int heartbeatconfirm = 1;
double leftWheelSpeed = 0; // Keep track of the wheel speeds
double rightWheelSpeed = 0;
double current_angle = 0; // The current angle of the robot (relative)
double target_angle = 0; // The current target angle
double current_distance = 0;
double target_distance = 0;
double left_encoder_zeropoint = 0; // Zeropoints for the encoders
double right_encoder_zeropoint = 0;
double left_fudge_factor = 0.9;
auto startTime = std::chrono::system_clock::now();
// Flags
bool turning = false; // Whether or not we should be executing a turn
bool driving = false; // Whether or not we are driving forward
bool firstPIDspin = true; // Something to initialize the clock

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

void heartbeatCallback(const std_msgs::Int32::ConstPtr& msg)
{
  ROS_INFO("Received heartbeat number: [%i]", msg->data);
  heartbeatconfirm = msg->data;
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

double millis() {
  auto endTime = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = endTime - startTime;
  return diff.count();
}


double newtime = millis();
int newRightEnc, newLeftEnc;
double leftErrorSum = 0;
double rightErrorSum = 0;
bool first_done = false;
void forwardPID(double *leftWheelSpeed, double *rightWheelSpeed, int leftEnc, int rightEnc, bool *firstRun, double Kp, double Ki) {

  if(!first_done) {
    newRightEnc = rightEnc;
    newLeftEnc = leftEnc;
    first_done = true;
    return;
  }

  // Time
  double oldtime = newtime;
  newtime = millis(); // TODO: Replace
  double timediff = newtime - oldtime;
  
  // Right Encoders
  int oldRightEnc = newRightEnc;
  newRightEnc = rightEnc;
  int rightEncDiff = newRightEnc - oldRightEnc;

  // Left Encoders
  int oldLeftEnc = newLeftEnc;
  newLeftEnc = leftEnc;
  int leftEncDiff = newLeftEnc - oldLeftEnc;

  // The speeds will need a conversion to the normalized motor speeds that the arduino takes
  double leftEncSpeed = leftEncDiff*1.0/(timediff);
  double rightEncSpeed = rightEncDiff*1.0/(timediff);

  // The actual PID
  double leftError = *leftWheelSpeed - leftEncSpeed;  // Proportional term
  double rightError = *rightWheelSpeed - rightEncSpeed;
  
  leftErrorSum += leftError;  // Integral term
  rightErrorSum += rightError;

  *leftWheelSpeed = Kp*leftError + Ki*leftErrorSum;
  *rightWheelSpeed = Kp*rightError + Ki*rightErrorSum;
  
  // Evidently many systems don't use D terms in real applications, so we'll wait
}

// Sets motor values, returns true if at destination 
bool goXInches(double *leftWheelSpeed, double *rightWheelSpeed, double target, double current, double speed) {
  double diff = target - current;

  if(diff < FORWARD_PRECISION) {
    *rightWheelSpeed = 0;
    *leftWheelSpeed = 0;
    return 1;
  } 
  *rightWheelSpeed = speed;
  *leftWheelSpeed = speed*left_fudge_factor;
  return 0;
}

// Sets motor values, returns true if at destination
bool pivotOnWheel(double *leftWheelSpeed, double *rightWheelSpeed, double target, double current) {
  double diff = target - current;
  if(abs(diff) < ANGLE_PRECISION) {
    *rightWheelSpeed = 0;
    *leftWheelSpeed = 0.5*BREAK_SPEED;
    return 1;
  }
  else if(abs(diff) < 10) {
    *leftWheelSpeed = BREAK_SPEED;
    *rightWheelSpeed = 0;
  }
  if(diff < 0) {
    *rightWheelSpeed = PIVOT_SPEED;
    *leftWheelSpeed = 0;
  } else {
    *leftWheelSpeed = PIVOT_SPEED;
    *rightWheelSpeed = 0;
  }
  return 0;
}


// Reset local states
void zero_system() {
  left_encoder_zeropoint = leftEncoder;
  right_encoder_zeropoint = rightEncoder;
  current_angle = 0;
  target_angle = 0;
  current_distance = 0;
  target_distance = 0;
  turning = false;
  driving = false;
}

  
/**
 * Early prototype of the robot main control, adapted from ROS tutorials
 */
int main(int argc, char **argv) {
  ROS_INFO("PROGRAM STARTED");

  std::time_t start_time = std::time(NULL); // Keeps track of time, clock_t is alias of some arithmetic type
  double total_time; // The current time in seconds since the start of the clock
  double last_update = -30; // Keep track of the last time since we made a command change
  
  // Initialize ROS, this must be done before using other ROS components
  ros::init(argc, argv, "robot");

  // Main access point to ROS. Makes a ROS node, which will shut down when this is destructed
  ros::NodeHandle n;

  // Publish topics "RightMotors" and "LeftMotors" of the msg variety of the same (singular) name, with a message queue of 1000
  ros::Publisher left_motor_pub = n.advertise<std_msgs::Float32>("LeftMotors", 1000);
  ros::Publisher right_motor_pub = n.advertise<std_msgs::Float32>("RightMotors", 1000);
  ros::Publisher freq_div_pub = n.advertise<std_msgs::Int32>("FreqDiv", 1000);
  ros::Publisher heartbeatcheck_pub = n.advertise<std_msgs::Int32>("Heartbeat",1000);  
  ros::Publisher enc_reset_pub = n.advertise<std_msgs::Bool>("EncReset",1000);
  ros::Publisher retrieval_arm_pub = n.advertise<std_msgs::Bool>("GrabObject",1000);

  // Subscriber to motor return
  ros::Subscriber subLMotor = n.subscribe("LeftReturn", 1000, leftMotorCallback);
  ros::Subscriber subRMotor = n.subscribe("RightReturn", 1000, rightMotorCallback);
  ros::Subscriber subPaused = n.subscribe("Paused", 1000, pausedCallback);
  ros::Subscriber subLEncoder = n.subscribe("LeftEncoder", 1000, leftEncoderCallback);
  ros::Subscriber subREncoder = n.subscribe("RightEncoder", 1000, rightEncoderCallback);
  ros::Subscriber subHeartbeatConfirm = n.subscribe("ConfHeartbeat", 1000, heartbeatCallback);

  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  ros::spinOnce(); // Allow values to update

  // Begin stopped
  rightWheelSpeed = 0;
  leftWheelSpeed = 0;

  std_msgs::Int32 freq_div_msg;
  freq_div_msg.data = 0;
  freq_div_pub.publish(freq_div_msg);

  std_msgs::Int32 heartbeat_check_msg;
  heartbeat_check_msg.data = heartbeat;

  std_msgs::Bool enc_reset_msg;
  enc_reset_msg.data = true;
  enc_reset_pub.publish(enc_reset_msg);

  ros::spinOnce();
  loop_rate.sleep();
  enc_reset_msg.data = false;
  enc_reset_pub.publish(enc_reset_msg);
  ros::spinOnce();

  zero_system(); // This should get the correct encoder values now
  freq_div_msg.data = 1;
  freq_div_pub.publish(freq_div_msg);

  while (ros::ok()) {
    current_angle = enc2angle(leftEncoder-left_encoder_zeropoint, rightEncoder - right_encoder_zeropoint);
    current_distance = enc2distance(leftEncoder - left_encoder_zeropoint)/2 + enc2distance(rightEncoder - right_encoder_zeropoint)/2;

    ROS_INFO("Encoder [left, right]: [%f, %f]", leftEncoder - left_encoder_zeropoint, rightEncoder - right_encoder_zeropoint);
    // First we update the total time
    total_time = (std::time(NULL) - start_time); // Get the time since start in seconds

    // Not robust to slow signals
    ros::spinOnce();
    if (heartbeatconfirm < heartbeat-15) {
      paused = true;
      rightWheelSpeed = 0;
      leftWheelSpeed = 0;
    }
    else {
      paused = false;    
      heartbeat_check_msg.data = heartbeat;
      heartbeatcheck_pub.publish(heartbeat_check_msg);
      heartbeat++;
    }

    if (!paused) {
      
      // Turn 45 degrees every 10 seconds
      
      /*if(total_time - last_update > 10.0) { // Switch every 5 seconds
	zero_system();
	target_angle = 45;
	last_update = total_time;
	turning = true;
	}*/
      
       if(total_time - last_update > 30.0) { // Switch every 20 seconds
	zero_system();
	target_distance = 180; // Go 20 tiles
	last_update = total_time;
	driving = true;
	}
      
       
	 ROS_INFO("Current angle: %f", current_angle);
	 ROS_INFO("Target angle: %f", target_angle);
       
       if(driving) {
	 ROS_INFO("Current distance: %f", current_distance);
	 ROS_INFO("Target distance: %f", target_distance);
       }
	 
      ROS_INFO("Total time since first update: %f", (float)(total_time));
      ROS_INFO("Time since last update: %f", (float)last_update);
      if(turning) {
	turning  = !(pivotOnWheel(&leftWheelSpeed, &rightWheelSpeed, target_angle, current_angle));
	ROS_INFO("Trying to turn: %f %f", leftWheelSpeed, rightWheelSpeed);
      } else if(driving) {
	driving = !(goXInches(&leftWheelSpeed, &rightWheelSpeed, target_distance, current_distance, 0.55));
	forwardPID(&leftWheelSpeed, &rightWheelSpeed, leftEncoder, rightEncoder, &firstPIDspin, K_p, K_i);
	ROS_INFO("Trying to drive: %f %f", leftWheelSpeed, rightWheelSpeed);
	
	/*
	// Angle correction
	
	if (current_angle > 4) {
	  leftWheelSpeed = leftWheelSpeed*0.5;
	} else if (current_angle < -4) {
	  rightWheelSpeed = rightWheelSpeed*0.5;
	  }
	  }*/

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
    } else {
      ROS_INFO("I AM PAUSED");
    }
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency

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
