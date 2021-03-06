// Commending and uncommenting the #define lines turns compilation of the relevant code sections
// Look up preprocessor directives if you are unfamiliar
//#define DEBUG // Print the detailed debug messages
//#define HEARTBEAT_MONITOR // Pause the robot if encoder heartbeat is lost
#define DEBUG_LITE // Print the most necessary debug messages
#define ANGLE_CORRECTION // Correct the driving by keeping the angle close to 0
//#define PID17 // PID controls Class 2017
#define DRIVE_REPORT // Progrom Writes motor values and other info to File 


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
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <chrono>

using namespace std;

// Static constants (remain the same)
static const double MOTOR_MAX = 1.0; // Max motor value (1.0 max, as defined by arduino code)
static const double PIVOT_SPEED = 0.8; // The speed to run the motors at in a pivot
static const double ENC_FUDGE = 1.0; // Encoder fudge factor. Keep at 1.0. Only use if absolutely necessary (unused)
static const double BRAKE_SPEED = -0.1; // Reverse with enough power to stop wheel motion
static const double DRIVE_SPEED = 1.0; // The standard drive speed (% of MOTOR_MAX)
static const double DRIVE_SPEED_FAST = 1.0; // The fast drive speed
static const double ANGLE_PRECISION = 5; // The precision to which angle movements are made, units of degrees
static const double FORWARD_PRECISION = 20; // The precision to which linear movements are made, units of inches
static const int PULSE_RATIO = 2400; // The number of pulses per full rotation in an encoder 
static const double WHEEL_DIAMETER = 11.5; // The wheel diameter in inches
static const double WHEEL_BASE = 39; // The width of the wheel base in inches
static const double K_p = 1; // PID proportional constant (unused)
static const double K_i = 1; // PID integral constant (unused)
static const double RAMP_STEP = 53; // The distance that the robot has to move in RAMP_TIME
static const double RAMP_ANGLE_STEP = 5; // The angle that the robot has to move in RAMP_TIME
static const double RAMP_TIME = 2.0; // The time the checkpoints are checked for the ramping procedure

// Global variables
long long rightEncoder = 0; // Right encoder count
long long leftEncoder = 0; // Left encoder count
int heartbeat = 1; // The heartbeat ID last sent (used for detecting loss of comms)
int heartbeatconfirm = 1; // The heartbeat last received
double leftWheelSpeed = 0; // Keep track of the wheel speeds
double rightWheelSpeed = 0; // (0.0-1.0)
double current_angle = 0; // The current angle of the robot (relative).
double target_angle = 0; // The angle the robot "wants" to be at (if turning)
double current_distance = 0; // The distance traveled since last zero (average of the two encoders)
double target_distance = 0; // The target distance to travel to if driving
long long left_encoder_zeropoint = 0; // Zeropoints for the encoders
long long right_encoder_zeropoint = 0; // These are subtracted from reported values to make the values used
double ramping_seconds = 0; // The number of seconds ramped
double ramp_factor = 0.5; // Percentage of the motor values used (will increase as robot ramps)
double ramping_distance = 0; // The distance at the last checkpoint
double ramping_angle = 0; // The angle at the last checkpoint
auto startTime = std::chrono::system_clock::now(); // The start time for the program

// Flags (Keep track of robot state)
bool paused = false; // Whether or not the robot is paused
bool turning = false; // Whether or not we should be executing a turn
bool turningInPlace = false; // If turning, whether or not to turn in place
bool driving = false; // Whether or not we are driving forward
bool drivingFast = false; // Whether or not the driving is at fast speed
bool grabbing = false; // Whether or not we are grabbing an object
bool retrievalgo = false;
bool retrievalconfirm = false;
bool firstPIDspin = true; // Something to initialize the clock
bool turningLeft = true; // Keep track of which direction the turn should be (false for right)
bool rampingStart = true; // Start the ramping sequence
bool ramping = false; // Whether ramping should run at all

// Prototypes (decalre methods here if needed before they are deifined)
void zero_system();

/***************************************************
 *  Callback Functions for the ROS system          *
 ***************************************************/

// Confirmation of motor value set
void leftMotorCallback(const std_msgs::Float32::ConstPtr& msg)
{
#ifdef DEBUG_LITE
  ROS_INFO("LeftMotorSet: [%f]", msg->data);
#endif
}

// Confirmation of motor value set
void rightMotorCallback(const std_msgs::Float32::ConstPtr& msg)
{
#ifdef DEBUG_LITE
  ROS_INFO("RightMotorSet: [%f]", msg->data);
#endif
}

// The aruduino has received a remote pause
void pausedCallback(const std_msgs::Bool::ConstPtr& msg)
{
  ROS_INFO("Paused_state: [%d]", (int)(msg->data));
  paused = msg->data; // Set the paused state value
}

// Left encoder reporting value
void leftEncoderCallback(const std_msgs::Int64::ConstPtr& msg)
{
#ifdef DEBUG
  ROS_INFO("LeftEncoderReading: [%d]", msg->data);
#endif 
  leftEncoder = msg->data; // Set the left encoder value
}

// Right encoder reporting value
void rightEncoderCallback(const std_msgs::Int64::ConstPtr& msg)
{
#ifdef DEBUG
  ROS_INFO("RightEncoderReading: [%d]", msg->data);
#endif
  rightEncoder = msg->data; // Set the right encoder value
}

// Commands received from an external source, set the states accordingly
void commandsCallback(const robot::Commands::ConstPtr& msg)
{
  ROS_INFO("COMMAND RECEIVED: %i, %f", msg->commandOrder, msg->value);
  zero_system(); // Stop everything, we have a new command
  switch(msg->commandOrder) {
  case 1: // Drive forward a certain distance
    driving = true;
    drivingFast = false;
    target_distance = msg->value * 39.37; // The value is the distance (m -> in)
    ramping = false;
    rampingStart = true;
    break;
  case 2: // Turn to an angle
    turning = true;
    target_angle = msg->value; // The value is the angle (degrees)
    turningLeft = target_angle<0;
    turningInPlace = false;
    ramping = true;
    rampingStart = true;
    break;
  case 3: // Grab the target
    grabbing = true;
    break;
  case 4: // Drive forward a certain distance, fast
    driving = true;
    drivingFast = true;
    target_distance = msg->value * 39.37; // The value is the distance (m -> in)
    ramping = false;
    break;
  case 5: // Turn in place
    turning = true;
    target_angle = msg->value;
    turningInPlace = true;
    rampingStart = true;
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
  heartbeatconfirm = msg->data; // Report that this hearbeat ID has made a round trip
}

// Signal from retrieval 
void retrievalCallback(const std_msgs::Bool::ConstPtr& msg)
{
  ROS_INFO("Retrieval finished.");
  retrievalconfirm = msg->data; // Report the state of retreival (true if success)
}

/***********************************************
 *   Conversions and other useful functions    *
 ***********************************************/

// Converts units of encoder counts to inches
double enc2distance(long long enc) {
  return ((double)enc/PULSE_RATIO)*3.14159*WHEEL_DIAMETER*ENC_FUDGE;
}

// Converts units of two encoders to degrees turned
double enc2angle(long long encL, long long encR) {
  double diff = encL - encR;
  return 360.0 * enc2distance(diff)/((2*3.14159)*WHEEL_BASE);
}

// Report the time since program start in seconds
double seconds() {
  auto endTime = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = endTime - startTime;
  return diff.count();
}

/******************************
 *    Movement corrections    *
 ******************************/

// Left as a possible suggestion on format for future teams, this code is untested and possible non-functional
double newtime = seconds();
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
  newtime = seconds(); // TODO: Replace
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
  if(diff == 0){ // Perfectly in place, don't do anything
    *rightWheelSpeed = 0;
    *leftWheelSpeed = 0;
    return 1;
  }
  if(std::abs(diff) < FORWARD_PRECISION) { // If we are closer than our chosen precision, stop
    *rightWheelSpeed = 1.0*BRAKE_SPEED; // Drive the robot at the braking speed
    *leftWheelSpeed = 1.0*BRAKE_SPEED;
    return 1;
  }
  *rightWheelSpeed = speed; // Otherwise move ahead
  *leftWheelSpeed = speed;
  if (diff < 0) { // If we need to be going backwards, make motor setpoints negative
    *rightWheelSpeed *= -1;
    *leftWheelSpeed *= -1;
  }
  return 0;
}

// Sets motor values, returns true if at destination
bool pivotOnWheel(double *leftWheelSpeed, double *rightWheelSpeed, double target, double current) {
  double diff = target - current; // How much we need to turn
  if(abs(diff) < ANGLE_PRECISION || (diff>0 && turningLeft) || diff<0 && !turningLeft) { // If we are closer than our precision, brake
    *rightWheelSpeed = 0.82*BRAKE_SPEED; // Lesser braking speed due to less momentum
    *leftWheelSpeed = 0.82*BRAKE_SPEED;
    return 1;
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

// Sets motor values, returns true if at destination
bool turnInPlace(double *leftWheelSpeed, double *rightWheelSpeed, double target, double current) {
  double diff = target - current; // How much we need to turn
  if(diff>0){
    diff *= 1.0;
  }
  if(abs(diff) < ANGLE_PRECISION) { // If we are closer than our precision, brake
    *rightWheelSpeed = 0.82*BRAKE_SPEED;
    *leftWheelSpeed = 0.82*BRAKE_SPEED;
    return 1;
  }
  if(diff < 0) { // If we need to turn left
    *rightWheelSpeed = PIVOT_SPEED;
    *leftWheelSpeed = -1 * PIVOT_SPEED;
  } else { // If we need to turn right
    *leftWheelSpeed = PIVOT_SPEED;
    *rightWheelSpeed = -1 * PIVOT_SPEED;
  }
  return 0;
}

// Reset local states, so the robot is in a "starting configuration"
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
 * Robot main control
 */
int main(int argc, char **argv) {


  ROS_INFO("PROGRAM STARTED");
  
  // Initialize ROS, this must be done before using other ROS components
  ros::init(argc, argv, "robot");

  // Main access point to ROS. Makes a ROS node, which will shut down when this is destructed
  ros::NodeHandle n;

  // Publishers, with a message queue of 1000
  ros::Publisher left_motor_pub = n.advertise<std_msgs::Float32>("LeftMotors", 1000); // Value to set right motor at
  ros::Publisher right_motor_pub = n.advertise<std_msgs::Float32>("RightMotors", 1000); // Right motor value
  ros::Publisher freq_div_pub = n.advertise<std_msgs::Int32>("FreqDiv", 1000); // Frequency division topic
  ros::Publisher heartbeatcheck_pub = n.advertise<std_msgs::Int32>("Heartbeat",1000); // Heartbeat ID
  ros::Publisher enc_reset_pub = n.advertise<std_msgs::Bool>("EncReset",1000); // Encoder reset topic
  ros::Publisher retrieval_arm_pub = n.advertise<std_msgs::Bool>("GrabObject",1000); // Topic to signal a retreival grab
  ros::Publisher command_done_pub = n.advertise<std_msgs::Int32>("CommandDone",1000); // Topic to report a command finishing

  // Subscribers, with a message queue of 1000
  ros::Subscriber subLMotor = n.subscribe("LeftReturn", 1000, leftMotorCallback); // Subscriber reporting back left motor sets
  ros::Subscriber subRMotor = n.subscribe("RightReturn", 1000, rightMotorCallback); // Subscriber reporting back right motor sets
  ros::Subscriber subPaused = n.subscribe("Paused", 1000, pausedCallback); // Reports a software pause is necessary
  ros::Subscriber subLEncoder = n.subscribe("LeftEncoder", 1000, leftEncoderCallback); // Reports left encoder value
  ros::Subscriber subREncoder = n.subscribe("RightEncoder", 1000, rightEncoderCallback); // Reports right encoder value
  ros::Subscriber subHeartbeatConfirm = n.subscribe("ConfHeartbeat", 1000, heartbeatCallback); // The returning heartbeat ID
  ros::Subscriber subControl = n.subscribe("Commands", 1000, commandsCallback); // The incoming commands
  ros::Subscriber subRetrieval = n.subscribe("GrabFinished", 1000, retrievalCallback); // Reports the end of retreival phase

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
  enc_reset_msg.data = false; // Set to true if encoders should be reset
  enc_reset_pub.publish(enc_reset_msg); // Publish this reset value
  ros::spinOnce();

  zero_system(); // Zero out this new instance
  freq_div_msg.data = 1; // Ask for no frequency division
  freq_div_pub.publish(freq_div_msg); // Send the new value
	
   // PID Globals
    // Runs Continueously While(driving == true )
    // Reset when (drive = false)
   double Last_Error; // For Differential 
   double Ei = 0; // Integral, Sum of all errors	 
   double ERROR_TERM; // Sum of all E's

	// Define Max Speed Change
	double Max_Change = 0.4;
	// For Large Error Terms, max speed change if 40%. For PID's less than this number, change is proportional

	// Define Gains
	const double Kp = 0.05; //test to find best Kp
	const double Ki = 0.025;//change Ki, Kd, and Bias after finding Kp 
	const double Kd = 0.0;
	const double Bias = 0;

	// Initialize Error Terms
	double Ep = 0; // Porportional, Difference in Encoders

	double Ed = 0; // Differential, Difference in previous error to current
	
	// ANGLE CORRECTION VARS
	double angle_diff = 0;
	double past_angle = 0;
	bool left_error = false;
	bool right_error = false;
	
	#ifdef DRIVE_REPORT
		// Create a drive report tab separated file (.tsv)
		// CREATES FILE IN THE DIRECTORY YOU ARE IN WHEN "based_motor_control" IS LAUNCED 
		ofstream Drive_Report;
		Drive_Report.open("Drive Report.txt");
		Drive_Report << "Left Motor Value " << "\t" << "Right Motor Value " <<"\t" << "\n";
		// ^ REMOVE header if you want to read csv into another file
		
		// define counter variable and sample rate
		int counter = 0;
		const int COUNT_RATE = 1; // Samples every 'n' motor values 
	#endif
		
  while (ros::ok()) { // Stay in this loop for the length of the program
    // Get the current angle and distances
    current_angle = enc2angle(leftEncoder-left_encoder_zeropoint, rightEncoder - right_encoder_zeropoint);
    angle_diff = current_angle - past_angle; // For PID Derivative
    past_angle = current_angle; 
    current_distance = enc2distance(leftEncoder - left_encoder_zeropoint)/2 + enc2distance(rightEncoder - right_encoder_zeropoint)/2;

#ifdef DEBUG_LITE
    // Debug message
    ROS_INFO("Encoder [left, right]: [%lld, %lld]", leftEncoder - left_encoder_zeropoint, rightEncoder - right_encoder_zeropoint);
#endif

    // Get the newest ROS state
    ros::spinOnce();

    // This code monitors a heartbeat generated by an external source, and pauses the robot if the heatbeat fails
#ifdef HEARBEAT_MONITOR
    // Make sure that the encoders are responding
    if (heartbeatconfirm < heartbeat-15) { // If the heatbeats are out of synch
      paused = true; // Pause the robot
      rightWheelSpeed = 0; // Stop movement
      leftWheelSpeed = 0;
    } else { // Otherwise continue as normal, send next heartbeat
      paused = false;    
      heartbeat_check_msg.data = heartbeat;
      heartbeatcheck_pub.publish(heartbeat_check_msg);
      heartbeat++;
    }
#endif

    // While running (not paused)
    if (!paused) {	 
      if(turning) { // Turning take priority 
		if(turningInPlace){ // Either turn in place or pivot, depending on this flag
	 	 	turning = !(turnInPlace(&leftWheelSpeed, &rightWheelSpeed, target_angle, current_angle)); // Keep moving until we arrive
		} else {
	  		turning = !(pivotOnWheel(&leftWheelSpeed, &rightWheelSpeed, target_angle, current_angle)); // Keep moving until we arrive
		}
		if(!turning) { // Must have just finished the turn
	  	 std_msgs::Int32 msg; 
	  	 msg.data = 0;
	  	 command_done_pub.publish(msg); // Report a finished command
	  	 ramping = false; // Reset ramping to false
		}
		ROS_INFO("Trying to turn: %f %f", leftWheelSpeed, rightWheelSpeed); // Inform the user we are turning
      } else if(driving) { // If we are driving forward right now
			if(!drivingFast){ // Either drive normally or drive fast
	  			driving = !(goXInches(&leftWheelSpeed, &rightWheelSpeed, target_distance, current_distance, DRIVE_SPEED)); 
			} else {
	  			driving = !(goXInches(&leftWheelSpeed, &rightWheelSpeed, target_distance, current_distance, DRIVE_SPEED_FAST)); 
			}
			if(!driving) { // The destination must have been reashed this itteration
	  			std_msgs::Int32 msg; // Defined in msg directory 
	 			msg.data = 0;
	  			command_done_pub.publish(msg); // Report a finished command
	  			ramping = false; // Reset ramping to false now that the movement is done
	  			// zero_system(); // Zero out this new instance
			}
	/* _______________________________________________________________________________________
#ifdef PID17
// These are the distances traveled y left and right wheels, UNITS = INCHES
/*
Ei += current_angle; // Add to Integral Term
ROS_INFO("Ei Term: %f", Ei);
Ed = current_angle - Last_Error;
ROS_INFO("Ed Term: %f", Ed);
ERROR_TERM = (Kp*current_angle) + (Ki*Ei) + (Kd*Ed);

	if (ERROR_TERM < -Max_Change) { ERROR_TERM = -Max_Change;
		ROS_INFO("Error Term Max -> Max Right Speed Decrease)"); 
	} else if (ERROR_TERM > Max_Change) { ERROR_TERM = Max_Change; 
		ROS_INFO("Error Term Min -> Max Left Speed Decrease)"); 
	}
	
	if (ERROR_TERM > 0.01) { 
	  leftWheelSpeed = leftWheelSpeed*(1-ERROR_TERM);
	} else if (ERROR_TERM < -0.01) {
	   rightWheelSpeed = rightWheelSpeed*(1+ERROR_TERM);
	}
	

	
double LeftD = enc2distance(leftEncoder-left_encoder_zeropoint);
double RightD = enc2distance(rightEncoder-right_encoder_zeropoint); 
ROS_INFO("Left Distance: %f ", LeftD); 
ROS_INFO("Right Distance: %f ", RightD);

Ep = LeftD - RightD;
ROS_INFO("Ep Term: %f", Ep);
Ei += Ep; // Add to Integral Term
ROS_INFO("Ei Term: %f", Ei);
Ed = Ep - Last_Error;
ROS_INFO("Ed Term: %f", Ed);
ERROR_TERM = (Kp*Ep) + (Ki*Ei) + (Kd*Ed);

ROS_INFO("Error Term: %f",ERROR_TERM);

// Limit Change
if (ERROR_TERM < -Max_Change) { ERROR_TERM = -Max_Change;
	ROS_INFO("Error Term Max -> Max Right Speed Decrease)"); }
else if (ERROR_TERM > Max_Change) { ERROR_TERM = Max_Change; 
	ROS_INFO("Error Term Max -> Max Left Speed Decrease)"); }

// Adjust Wheel Speed
	if (ERROR_TERM > 0.01) { 
	  leftWheelSpeed = leftWheelSpeed*(1-ERROR_TERM);
	  ROS_INFO("New Left Wheel Speed: %f ", leftWheelSpeed); 
	} else if (ERROR_TERM < -0.01) {
	   rightWheelSpeed = rightWheelSpeed*(1+ERROR_TERM);
	   ROS_INFO("New Right Wheel Speed: %f ", rightWheelSpeed); 
	}


// Update Last Error
Last_Error = current_angle; 
ROS_INFO("Ep Term (Last Error): %f", Last_Error); 

const double MAX_ERROR = 0.15; // maxium value for proportional error term
double error_term = Ki*current_angle + Kd*angle_diff;

if (error_term > 0) {
	if(error_term > MAX_ERROR){
		error_term = MAX_ERROR;
	}
	leftWheelSpeed = leftWheelSpeed*(1-error_term);
	ROS_INFO("%f left speed reduction", (1-error_term));
} else {
	if(error_term < -MAX_ERROR){
		error_term = -MAX_ERROR;
	}
	rightWheelSpeed = rightWheelSpeed*(1+error_term);
	ROS_INFO("%f right speed reduction", (1+error_term));
}

#endif
*/
// END PID17
#ifdef ANGLE_CORRECTION	

// Angle correction, edits wheel speeds in response to being off-angle
	const int MAX_ANGLE = 2; // maxium allowable deviation, if this is exceeded, corrections begin
	const double MAX_ERROR = 0.15; // maxium value for proportional error term
	
	// Check current Angle
	if (current_angle > MAX_ANGLE) { 
	  	// leftWheelSpeed = leftWheelSpeed*0.85;
	  	left_error = true;
	  	right_error = false;
	  	ROS_INFO("LEFT ERROR");
	} else if (current_angle < -MAX_ANGLE) {
	   rightWheelSpeed = rightWheelSpeed*0.85;
	   right_error = true;
	   left_error = false;
	   ROS_INFO("RIGHT ERROR");
	}else if (abs(current_angle) < 0.65){ // do not change speeds, reset L/R errors
		left_error = false;
		right_error = false;
	}
	
	double error_term = 0; // proportional error term for speed change
	
	if (left_error){
		// Check derivative, if positive, do not change
		if (angle_diff < 0 ){
			// If negative, we are heading back on track, reduce speed a little less
			// We want a proportional term, ased of current_angle
			if(current_angle>0){
				error_term = abs(current_angle/(10*MAX_ANGLE));
			
				if(error_term > MAX_ERROR){
					error_term = MAX_ERROR;	  	
				  	#ifdef DRIVE_REPORT
				  		Drive_Report << "#*#*#*#*#*# MAX LEFT speed reduction #*#*#*#*# \n" ;
				  	#endif
				  	
				}

				// NOTE: wheel speed is reset after every pass of while loop
				leftWheelSpeed = leftWheelSpeed*(1-error_term);
				ROS_INFO("%f left speed reduction", (1-error_term));
			}
		}
	}
		
	if (right_error){
		// Check derivative, if negative, do not change
		if (angle_diff > 0 ){
			// If positive, we are heading back on track, reduce speed a little less
			// We want a proportional term, ased of current_angle
			if(current_angle<0){
				error_term = abs(current_angle/(10*MAX_ANGLE));
			
				if(error_term > MAX_ERROR){
					error_term = MAX_ERROR;				   
				   	#ifdef DRIVE_REPORT
				  		Drive_Report << "#*#*#*#*#*# MAX  RIGHT speed reduction #*#*#*#*# \n" ;
				  	#endif
				}
				rightWheelSpeed = rightWheelSpeed*(1-error_term);
				ROS_INFO("%f right speed reduction", (1-error_term));
			}
		}
	}
	
	
	
	/*
	if (current_angle > 15) { // If the deviation is very large, slow down both sides, and with a greater difference 
	  leftWheelSpeed = leftWheelSpeed*0.6;
	  rightWheelSpeed = rightWheelSpeed*0.95;
	  ROS_INFO("!!!!!!!!!!!!!!!!!!!!!BIG CORRECTION TO THE LEFT"); // Report this larger correction for deubugging purposes
	} else if (current_angle < -15) {
	  rightWheelSpeed = rightWheelSpeed*0.6;
	  leftWheelSpeed = leftWheelSpeed*0.95;
	  ROS_INFO("!!!!!!!!!!!!!!!!!!!!!BIG CORRECTION TO THE RIGHT");
	}
	*/
#endif
	ROS_INFO("Trying to drive: %f %f", leftWheelSpeed, rightWheelSpeed); // Let the user know we are trying to drive
      } else if (grabbing) { // If the robot needs to grab the object
	ROS_INFO("Grabbing the object.");
	retrieval_go_msg.data = true;
	retrieval_arm_pub.publish(retrieval_go_msg); // Tell the arm arduino to start grabbing
	retrievalconfirm = false;
	while(!retrievalconfirm){ // Stop everything until the grabbing is finished (this blocks for now)
	  ros::spinOnce();
	}
	grabbing = false;
	retrieval_go_msg.data = false;
	retrieval_arm_pub.publish(retrieval_go_msg); // Tell the arm arduino not to grab anymore
      }

#ifdef DEBUG_LITE 
      //Turning messages (always want to see these)
      ROS_INFO("Current angle: %f", current_angle);
      ROS_INFO("Target angle: %f", target_angle);      
      // Driving messages (only care if driving)
      if(driving) {
	ROS_INFO("Current distance: %f", current_distance);
	ROS_INFO("Target distance: %f", target_distance);
      }
#endif

    } else { // Otherwise the robot is paused
      ROS_INFO("I AM PAUSED");
    }
    
    // Ramping code, used to slowly increase motor power to the dynamically determined minimum each movement
    if(ramping){
      if(driving && rampingStart) { // If this is the beginning of motion
	ramp_factor = 0.3; // Start the ramping factor at 30% (robot moves at 30% of what it would otherwise)
	rampingStart = false;
	ramping_seconds = seconds(); // Start the clock here
	ramping_distance = current_distance; // Start the first checkpoint here
      } else if(driving) {
	ROS_INFO("Drive Checkpoint in : %f / %f", current_distance - ramping_distance, RAMP_STEP); // Report how long to next check
	// If it doesn't make it to the checkpoint in time
	if((current_distance - ramping_distance) < RAMP_STEP && (seconds() - ramping_seconds) > RAMP_TIME) { // Under distance, over time
	  ramp_factor += 0.1; // Ramp 10%
	  ramping_seconds = seconds(); // Reset ramp timer
	  ROS_INFO("RAMPING TO: %f", ramp_factor);
	} else if(current_distance - ramping_distance > RAMP_STEP) { // It made it to the next checkpoint 
	  ramping_seconds = seconds(); // Reset ramp timer
	  ramping_distance = current_distance; // start analyzing the next step
	}
      }

      ROS_INFO("Time to ramp: %f / %f", seconds() - ramping_seconds, RAMP_TIME);

      if(turning && rampingStart) { // If this is the beginning of a turn ramping
	ramp_factor = 0.2; // Start at 20%
	rampingStart = false;
	ramping_seconds = seconds(); // Start timer
	ramping_angle = current_angle; // Zero out the checkpoint
      } else if(turning) {
	ROS_INFO("Turn Checkpoint in : %f / %f", current_angle - ramping_angle, RAMP_ANGLE_STEP);
	// If it didn't make it to the checkpoint in time
	if((current_angle - ramping_angle) < RAMP_ANGLE_STEP && (seconds() - ramping_seconds) > RAMP_TIME) { // Under angle, over time
	  ramp_factor += 0.1; // Ramp 10%
	  ramping_seconds = seconds(); // Reset ramp timer
	  ROS_INFO("RAMPING TO: %f", ramp_factor);
	} else if(current_angle - ramping_angle > RAMP_ANGLE_STEP) { // Made it to the next checkpoint 
	  ramping_seconds = seconds(); // Reset ramp timer
	  ramping_angle = current_angle; // start analyzing the next step
	}
      }
    } else { // If we aren't ramping, set the ramp factor to 100%
      ramp_factor = 1.0;
    }

    if(ramp_factor > 1.0) { // Don't let it ramp above the max
      ramp_factor = 1.0;
    }

    // Values decided, pass to arduinos
    // Pack the motor values into a message object
    std_msgs::Float32 right_msg; // Defined in msg directory
    std_msgs::Float32 left_msg;
    left_msg.data = leftWheelSpeed * ramp_factor * MOTOR_MAX;
    right_msg.data = rightWheelSpeed * ramp_factor * MOTOR_MAX;
	 
    // Publish the motor speed message. Notice that the msg type matches the advertise template <>
    left_motor_pub.publish(left_msg); // Send the new speeds for the arduino to pick up.
    right_motor_pub.publish(right_msg); // Send the new speeds for the arduino to pick up.
#ifdef DEBUG_LITE
    ROS_INFO("Published motor vals: %f,%f", (float)left_msg.data, (float)right_msg.data);
    ROS_INFO("---------------------------------------------------------------------");
#endif

#ifdef DRIVE_REPORT
	counter += 1;
	if (counter % COUNT_RATE == 0){
		Drive_Report << (float)left_msg.data << "\t"<< (float)right_msg.data << "\n";
	}
#endif

    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
  }
  return 0;
}
