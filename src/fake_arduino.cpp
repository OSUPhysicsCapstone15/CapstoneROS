#include "ros/ros.h"
#include "std_msgs/String.h"
#include "robot/Motors.h"
#include "robot/EncoderRequest.h"

void motorCallback(const robot::Motors::ConstPtr& msg)
{
  ROS_INFO("Setting the motors: [%f,%f]", msg->leftMotor, msg->rightMotor);
}

bool encoders(robot::EncoderRequest::Request  &req,
	   robot::EncoderRequest::Response &res)
{
  res.leftCount = 1;
  res.rightCount = 2;
  ROS_INFO("Request for encoder information received");
  ROS_INFO("sending back response: [%f,%f]", (float)res.leftCount, (float)res.rightCount);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fake_arduino");
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("Motors", 1000, motorCallback);
  ros::ServiceServer service = n.advertiseService("encoder_request", encoders);
  ROS_INFO("Fake Arduino Running");
  ros::spin();

  return 0;
}
