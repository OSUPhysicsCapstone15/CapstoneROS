#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/UInt16.h>

// Set up ROS node
ros::NodeHandle  nh;
// Set up subscriber
ros::Subscriber<std_msgs::UInt16> sub("stepper", stepper_cb);
// Set up stepper
Stepper cam(200, 5, 4, 3, 2);

// Get the angle from the console
void stepper_cb(const std_msgs::UInt16& cmd_msg){
	// Convert angle to stpes
	int steps = cmd_msg.data/1.8;
	cam.step(steps); 
}

void setup() {
	nh.initNode();
	nh.suscribe(sub); 
	
	cam.setSpeed(60);
	Serial.begin(9600);
}

void loop(){
	nh.spinOnce();
	delay(1);
}
