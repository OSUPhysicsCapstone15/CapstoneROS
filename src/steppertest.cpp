#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/UInt16.h>

ros::NodeHandle  nh;

Stepper cam(200, 11, 10, 9, 8);

void stepper_cb(const std_msgs::UInt16& cmd_msg){
	cam.step(cmd_msg.data); 
}

ros::Subscriber<std_msgs::UInt16> sub("stepper", stepper_cb);

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
