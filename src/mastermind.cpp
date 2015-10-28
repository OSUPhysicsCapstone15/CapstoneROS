#include "ros/ros.h"
#include "std_msgs/String.h"
#include "robot/EncoderRequest.h"
#include "robot/Motors.h"
#include <cstdlib> 
#include <sstream>
#include <ctime>

// Some constants
static const double MOTOR_MAX = 350;

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

  // Publish a topic "Motors" of the "Motors" msg variety, with a message queue of 1000
  ros::Publisher motor_pub = n.advertise<robot::Motors>("Motors", 1000);
  
  // Start a service client that will call the service "encoder_request"
  ros::ServiceClient encoderClient = n.serviceClient<robot::EncoderRequest>("encoder_request");
  
  // Defines a maximum loop rate (10 Hz)
  ros::Rate loop_rate(10); // This should be fast enough for us, since at 2 m/s this would be .2 meters an update at worst.

  
  int count = 0;// Count the iterations, because why not
  double leftWheelSpeed = 0; // Keep track of the wheel speeds
  double rightWheelSpeed = 0;
  int leftEncoderCount = 0;
  int rightEncoderCount = 0;
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
    robot::Motors msg; // Defined in msg directory
    msg.leftMotor = leftWheelSpeed * MOTOR_MAX;
    msg.rightMotor = rightWheelSpeed * MOTOR_MAX;
    
    // Publish the motor speed message. Notice that the msg type matches the advertise template <>
    motor_pub.publish(msg); // Send the new speeds for the arduino to pick up.
    ROS_INFO("Published motor vals: %f,%f", (float)msg.leftMotor, (float)msg.rightMotor);
    
    // Make a service request. Defined in srv directory
    robot::EncoderRequest srv;

    //srv.request.left = 677; /* This is what a request arugment WOULD look like, but there are none */

    // Make a call to a service. It returns true if it succeeds, and updates src with the right values
    if (encoderClient.call(srv)) {
      leftEncoderCount = (long)srv.response.leftCount;
      rightEncoderCount = (int)srv.response.rightCount;
      ROS_INFO("Encoders read: (%d,%d)", leftEncoderCount, rightEncoderCount);
    } else { // If it returned false, something went wrong
      ROS_ERROR("Failed to call service encoder_request");
    }
    
    ros::spinOnce(); // Checks for ros update
    loop_rate.sleep(); // Sleep for the period corresponding to the given frequency
    count++; // Increment count
  }

  return 0;
}

