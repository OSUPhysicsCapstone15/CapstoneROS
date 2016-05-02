/* Encoder Reader
 * Adapted from the Phidget Encoder's example client, this program reports encoder values.
 * It updates the ros publishers each time a step is detected in the encoder.
*/

#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Int64.h"
#include "std_msgs/Bool.h"
#include <stdio.h>
#include <phidget21.h>

// Set up ROS publishing
std_msgs::Int64 leftencoder_msg;
std_msgs::Int64 rightencoder_msg;
std_msgs::Int32 confirmheartbeat_msg;
ros::Publisher pub_LeftEncoder;
ros::Publisher pub_RightEncoder;
ros::Publisher pub_confirmHeartbeat;

// Static constants
static int freqDiv = 10; // Only a count that is a multiple of this is reported, to prevent saturation.
static int heartbeat = 0; // Used to detect whether or not the system remains attached (to calling ROS program)

// Gobals
long long countR, countL; // The counts of each encoder
bool resetFlag = false; // Whether or not there should be a reset

// ROS callback, sets the frequency division value
void FreqDivCallback(const std_msgs::Int32& msg) {
  freqDiv = msg.data;
}

// ROS callback, begins a heartbeat check
void HeartbeatCheckCallback(const std_msgs::Int32& msg) {
  heartbeat = msg.data;
}

// ROS callback, resets the encoder values
void RunResetCallback(const std_msgs::Bool& msg) {
  if (msg.data) {
    resetFlag = true; // Tells the system to run a reset
  }
}

// Attach to new device - Unchanged from example
int CCONV AttachHandler(CPhidgetHandle ENC, void *userptr)
{
  int serialNo;
  CPhidget_DeviceID deviceID;
  int i, inputcount;
  
  CPhidget_getSerialNumber(ENC, &serialNo);
  
  //Retrieve the device ID and number of encoders so that we can set the enables if needed
  CPhidget_getDeviceID(ENC, &deviceID);
  CPhidgetEncoder_getEncoderCount((CPhidgetEncoderHandle)ENC, &inputcount);
  printf("Encoder %10d attached! \n", serialNo);
  
  //the 1047 requires enabling of the encoder inputs, so enable them if this is a 1047    
  if (deviceID == PHIDID_ENCODER_HS_4ENCODER_4INPUT)
    {
      printf("Encoder requires Enable. Enabling inputs....\n");
      for (i = 0 ; i < inputcount ; i++)
	CPhidgetEncoder_setEnabled((CPhidgetEncoderHandle)ENC, i, 1);
    }
  return 0;
}

// Detach from the board - Unchanged from example
int CCONV DetachHandler(CPhidgetHandle ENC, void *userptr)
{
  int serialNo;
  CPhidget_getSerialNumber(ENC, &serialNo);
  printf("Encoder %10d detached! \n", serialNo);
  
  return 0;
}

// Error handling - Unchanged from example
int CCONV ErrorHandler(CPhidgetHandle ENC, void *userptr, int ErrorCode, const char *Description)
{
  printf("Error handled. %d - %s \n", ErrorCode, Description);
  
  return 0;
}

// Responds to a change on an input pin of the board
int CCONV InputChangeHandler(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int State)
{
  printf("Input #%i - State: %i \n", Index, State);
  
  return 0;
}

// Responds to position changes from the attached encoders. Rewritten to update the new counts over ROS
int CCONV PositionChangeHandler(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int Time, int RelativePosition)
{
  int Position;
  CPhidgetEncoder_getPosition(ENC, Index, &Position); // Get the updated position

  // Update the correct side with the new count, if it is a multiple of freqDiv
  if(Index == 0) { // Left
    leftencoder_msg.data = Position;
    if(Position % freqDiv == 0){
      pub_LeftEncoder.publish(leftencoder_msg);
    }
    ros::spinOnce();
  } else if(Index == 1) { // Right
    rightencoder_msg.data = Position;
    if(Position % freqDiv == 0){
      pub_RightEncoder.publish(rightencoder_msg);
    } 
    ros::spinOnce();
  }
  // Print the update
  printf("Encoder #%i - Position: %5d -- Relative Change %2d -- Elapsed Time: %5d \n", Index, Position, RelativePosition, Time);
  
  return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
//Will also display the number of inputs and encoders on this device
int display_properties(CPhidgetEncoderHandle phid)
{
  int serialNo, version, num_inputs, num_encoders;
  const char* ptr;
  
  CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
  CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
  CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);
  
  CPhidgetEncoder_getInputCount(phid, &num_inputs);
  CPhidgetEncoder_getEncoderCount(phid, &num_encoders);
  
  printf("%s\n", ptr);
  printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
  printf("Num Encoders: %d\nNum Inputs: %d\n", num_encoders, num_inputs);
  
  return 0;
}

// Runs the encoder program until ROS is told to exit
int encoder_simple()
{
  int result;
  const char *err;
  
  //Declare an encoder handle
  CPhidgetEncoderHandle encoder = 0;
  
  //create the encoder object
  CPhidgetEncoder_create(&encoder);
  
  //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
  CPhidget_set_OnAttach_Handler((CPhidgetHandle)encoder, AttachHandler, NULL);
  CPhidget_set_OnDetach_Handler((CPhidgetHandle)encoder, DetachHandler, NULL);
  CPhidget_set_OnError_Handler((CPhidgetHandle)encoder, ErrorHandler, NULL);
  
  //Registers a callback that will run if an input changes.
  //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
  CPhidgetEncoder_set_OnInputChange_Handler(encoder, InputChangeHandler, NULL);
  
  //Registers a callback that will run if the encoder changes.
  //Requires the handle for the Encoder, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
  CPhidgetEncoder_set_OnPositionChange_Handler (encoder, PositionChangeHandler, NULL);
  
  CPhidget_open((CPhidgetHandle)encoder, -1);
  
  //get the program to wait for an encoder device to be attached
  printf("Waiting for encoder to be attached....");
  if((result = CPhidget_waitForAttachment((CPhidgetHandle)encoder, 10000)))
    {
      CPhidget_getErrorDescription(result, &err);
      printf("Problem waiting for attachment: %s\n", err);
      return 0;
    }
  
  //Display the properties of the attached encoder device
  display_properties(encoder);
  
  //read encoder event data
  printf("Reading.....\n");
  
  // Keep running until ROS kills the node
  while(ros::ok()) {
    if(heartbeat != 0) {
      confirmheartbeat_msg.data = heartbeat;
      pub_confirmHeartbeat.publish(confirmheartbeat_msg);
      heartbeat = 0;
    }
    if(resetFlag) {
      
      CPhidgetEncoder_setPosition(encoder, 0, 0);
      CPhidgetEncoder_setPosition(encoder, 1, 0);
      resetFlag = false;
    }
    ros::spinOnce();
  }
  
  //since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
  printf("Closing...\n");
  CPhidget_close((CPhidgetHandle)encoder);
  CPhidget_delete((CPhidgetHandle)encoder);
  
  //all done, exit
  return 0;
}

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "phidgetEncoder"); // Initialize this ROS node
  
  ros::NodeHandle  n;  // Setup ROS handler

  // Subscribe to the publishers repsonsible for giving commands to this node
  ros::Subscriber fd = n.subscribe("FreqDiv", 1000, FreqDivCallback);
  ros::Subscriber checkHeartbeat = n.subscribe("Heartbeat", 1000, HeartbeatCheckCallback);
  ros::Subscriber encReset = n.subscribe("EncReset", 1000, RunResetCallback);

  //Set up the publishers
  pub_LeftEncoder = n.advertise<std_msgs::Int64>("LeftEncoder", 1000); // The left encoder's position
  pub_RightEncoder = n.advertise<std_msgs::Int64>("RightEncoder", 1000); // The right encoder's position
  pub_confirmHeartbeat = n.advertise<std_msgs::Int32>("ConfHeartbeat", 1000); // The hearbeat ID responder
  
  ros::spinOnce(); // First report to ROS
  
  encoder_simple(); // This will run encoder monitoring until ROS node is shut down
  return 0;
}

