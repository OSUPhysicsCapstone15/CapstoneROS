// Uncomment below for debug messages
//#define DEBUG

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int64.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Bool.h"
#include "robot/Commands.h"
#include "robot/EncoderRequest.h"
#include "robot/LeftMotor.h"
#include "robot/RightMotor.h"
#include <cstdlib> 
#include <sstream>
#include <ctime>
#include <cmath>
#include <chrono>

using namespace std;

// Some static constants
static const double MOTOR_MAX = 400; // Max motor value
static const double PIVOT_SPEED = 0.9; // The speed to run the motors at in a pivot
static const double BREAK_SPEED = -0.25; // Reverse with enough power to stop wheel motion
static const double ANGLE_PRECISION = 5; // Units of degrees
static const double FORWARD_PRECISION = 12; // Units of inches
static const int PULSE_RATIO = 600; // The number of pulses per full rotation in an encoder 
static const double WHEEL_DIAMETER = 12; // Inches
static const double WHEEL_BASE = 42; // Inches
static const double K_p = 1; // PID proportional constant
static const double K_i = 1; // PID integral constant

// Global variables
long long rightEncoder = 0; // Right encoder count
long long leftEncoder = 0; // Left encoder count
int heartbeat = 1; // The heartbeat last sent (used for detecting loss of comms)
int heartbeatconfirm = 1; // The heartbeat last received
double leftWheelSpeed = 0; // Keep track of the wheel speeds
double rightWheelSpeed = 0; // (0.0-1.0)
double current_angle = 0; // The current angle of the robot (relative)
double target_angle = 0; // The current target angle
double current_distance = 0; // The distance traveled since last zero (average of the two encoders)
double target_distance = 0; // The target distance to travel to if driving
long long left_encoder_zeropoint = 0; // Zeropoints for the encoders
long long right_encoder_zeropoint = 0; // These are subtracted from final values.
double left_fudge_factor = 1.0; // Used to correct for drift. 1.0
auto startTime = std::chrono::system_clock::now();

// Flags
bool paused = false; // Whether or not the robot is paused
bool turning = false; // Whether or not we should be executing a turn
bool driving = false; // Whether or not we are driving forward
bool grabbing = false; // Whether or not we are grabbing an object
bool retrievalgo = false;
bool retrievalconfirm = false;
bool firstPIDspin = true; // Something to initialize the clock

// Prototypes
void zero_system();

/***************************************************
 *  Callback Functions for the ROS system          *
 ***************************************************/

// Confirmation of motor value set
void leftMotorCallback(const std_msgs::Float32::ConstPtr& msg)
{
  ROS_INFO("LeftMotorSet: [%f]", msg->data);
}

// Confirmation of motor value set
void rightMotorCallback(const std_msgs::Float32::ConstPtr& msg)
{
  ROS_INFO("RightMotorSet: [%f]", msg->data);
}

// The aruduino has received a remote pause
void pausedCallback(const std_msgs::Bool::ConstPtr& msg)
{
  ROS_INFO("Paused_state: [%d]", (int)(msg->data));
  paused = msg->data;
}

// Left encoder reporting value
void leftEncoderCallback(const std_msgs::Int64::ConstPtr& msg)
{
#ifdef DEBUG
  ROS_INFO("LeftEncoderReading: [%d]", msg->data);
#endif 
  leftEncoder = msg->data;
}

// Right encoder reporting value
void rightEncoderCallback(const std_msgs::Int64::ConstPtr& msg)
{
#ifdef DEBUG
  ROS_INFO("RightEncoderReading: [%d]", msg->data);
#endif
  rightEncoder = msg->data;
}

// Commands received from an external source
void commandsCallback(const robot::Commands::ConstPtr& msg)
{
  ROS_INFO("COMMAND RECEIVED: %i, %f", msg->commandOrder, msg->value);
  zero_system(); // Stop everything, we have a new command
  switch(msg->commandOrder) {
  case 1: // Drive forward a certain distance
    driving = true;
    target_distance = msg->value; // The value is the distance (inches)
    break;
  case 2: // Turn to an angle
    turning = true;
    target_angle = msg->value; // The value is the angle (degrees)
    break;
  case 3: // Grab the target
    grabbing = true;
    break;
  default: // Default is to do nothing
    break;
  }
}

// Makes sure that we haven't lost communication with the encoders
void heartbeatCallback(const std_msgs::Int32::ConstPtr& msg)
{
#ifdef DEBUG
  ROS_INFO("Received heartbeat number: [%i]", msg->data);
#endif
  heartbeatconfirm = msg->data;
}

// Signal the retrieval has succeeded 
void retrievalCallback(const std_msgs::Bool::ConstPtr& msg)
{
  //ROS_INFO("Retrieval finished.");
  retrievalconfirm = msg->data;
}

/***********************************************
 *   Conversions and other useful functions    *
 ***********************************************/

// Converts units of encoder counts to inches
double enc2distance(long long enc) {
  return ((double)enc/PULSE_RATIO)*3.14159*WHEEL_DIAMETER;
}

// Converts units of two encoders to degrees turned
double enc2angle(long long encL, long long encR) {
  double diff = encL - encR;
  return 360.0 * enc2distance(diff)/((2*3.14159)*WHEEL_BASE);
}

double millis() {
  auto endTime = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = endTime - startTime;
  return diff.count();
}

/******************************
 *    Movement corrections    *
 ******************************/

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

/**************************
 *    Movement Control    *
 **************************/

// Sets motor values, returns true if at destination 
bool goXInches(double *leftWheelSpeed, double *rightWheelSpeed, double target, double current, double speed) {
  double diff = target - current; // The difference between where we are at and where we want to be

  if(std::abs(diff) < FORWARD_PRECISION) { // If we are closer than our chosen precision, stop
    *rightWheelSpeed = 0;
    *leftWheelSpeed = 0;
    return 1;
  }
  *rightWheelSpeed = speed; // Other wise move ahead
  *leftWheelSpeed = speed*left_fudge_factor;
  if (diff < 0) { // If we need to be going backwards, make motor setpoints negative
    *rightWheelSpeed *= -1;
    *leftWheelSpeed *= -1;
  }
  return 0;
}

// Sets motor values, returns true if at destination
bool pivotOnWheel(double *leftWheelSpeed, double *rightWheelSpeed, double target, double current) {
  double diff = target - current; // How much we need to turn
  if(abs(diff) < ANGLE_PRECISION) { // If we are closer than out precision, brake
    *rightWheelSpeed = 0;
    *leftWheelSpeed = 0.5*BREAK_SPEED;
    return 1;
  }
  else if(abs(diff) < 10) { // If we are within 10 degrees, start braking
    *leftWheelSpeed = BREAK_SPEED;
    *rightWheelSpeed = 0;
  }
  if(diff < 0) { // If we need to turn left
    *rightWheelSpeed = PIVOT_SPEED;
    *leftWheelSpeed = 0;
  } else { // If we need to turn right
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
  ros::Subscriber subControl = n.subscribe("Commands", 1000, commandsCallback);
  ros::Subscriber subRetrieval = n.subscribe("GrabFinished", 1000, retrievalCallback);

  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  ros::spinOnce(); // Allow values to update

  // Begin stopped
  rightWheelSpeed = 0;
  leftWheelSpeed = 0;

  // Initialize the ROS messages
  std_msgs::Int32 freq_div_msg;
  freq_div_msg.data = 0;
  freq_div_pub.publish(freq_div_msg);

  std_msgs::Int32 heartbeat_check_msg;
  heartbeat_check_msg.data = heartbeat;

  std_msgs::Bool retrieval_go_msg;
  retrieval_go_msg.data = retrievalgo;
  retrieval_arm_pub.publish(retrieval_go_msg);

  std_msgs::Bool enc_reset_msg;
  enc_reset_msg.data = true;
  enc_reset_pub.publish(enc_reset_msg);

  ros::spinOnce();
  loop_rate.sleep();
  enc_reset_msg.data = false;
  enc_reset_pub.publish(enc_reset_msg); // Reset the encoders to 0
  ros::spinOnce();

  zero_system(); // This should get the correct encoder values now
  freq_div_msg.data = 1;
  freq_div_pub.publish(freq_div_msg); // Start frequency division

  while (ros::ok()) { // Stay in this loop for the length of the program
    current_angle = enc2angle(leftEncoder-left_encoder_zeropoint, rightEncoder - right_encoder_zeropoint);
    current_distance = enc2distance(leftEncoder - left_encoder_zeropoint)/2 + enc2distance(rightEncoder - right_encoder_zeropoint)/2;

    ROS_INFO("Encoder [left, right]: [%lld, %lld]", leftEncoder - left_encoder_zeropoint, rightEncoder - right_encoder_zeropoint);

    ros::spinOnce();

    // Make sure that the encoders are responding
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

    // While running
    if (!paused) {
       
      //Turning messages
      ROS_INFO("Current angle: %f", current_angle);
      ROS_INFO("Target angle: %f", target_angle);
       

      // Driving messages
      if(driving) {
	ROS_INFO("Current distance: %f", current_distance);
	ROS_INFO("Target distance: %f", target_distance);
      }
	 
       if(turning) {
	 turning  = !(pivotOnWheel(&leftWheelSpeed, &rightWheelSpeed, target_angle, current_angle)); // Keep moving until we arrive
	 ROS_INFO("Trying to turn: %f %f", leftWheelSpeed, rightWheelSpeed);
       } else if(driving) { // If we are driving forward right now
	 driving = !(goXInches(&leftWheelSpeed, &rightWheelSpeed, target_distance, current_distance, 0.9));

	 // Angle correction, replacement for PID
	
	 if (current_angle > 3) {
	   leftWheelSpeed = leftWheelSpeed*0.5;
	 } else if (current_angle < -3) {
	   rightWheelSpeed = rightWheelSpeed*0.5;
	 }

	 if (current_angle > 8) {
	   leftWheelSpeed = leftWheelSpeed*0.4;
	   rightWheelSpeed = rightWheelSpeed*0.85;
	 } else if (current_angle < -8) {
	   rightWheelSpeed = rightWheelSpeed*0.4;
	   leftWheelSpeed = leftWheelSpeed*0.85;
	 }
	 //forwardPID(&leftWheelSpeed, &rightWheelSpeed, leftEncoder, rightEncoder, &firstPIDspin, K_p, K_i);
	 ROS_INFO("Trying to drive: %f %f", leftWheelSpeed, rightWheelSpeed);
       } else if (grabbing) {
	 ROS_INFO("Grabbing the object.");
	 retrieval_go_msg.data = true;
	 retrieval_arm_pub.publish(retrieval_go_msg); // Tell the arm arduino to start grabbing
	 retrievalconfirm = false;
	 while(!retrievalconfirm){ // Stop everything until the grabbing is finished (this blocks for now)
	   ROS_INFO("Waiting for grabbing to finish.");
	   ros::spinOnce();
	 }
	 retrieval_go_msg.data = false;
	 retrieval_arm_pub.publish(retrieval_go_msg); // Tell the arm arduino not to grab anymore
       }
    } else {
      ROS_INFO("I AM PAUSED");
    }
    

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
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
  }
  return 0;
}
