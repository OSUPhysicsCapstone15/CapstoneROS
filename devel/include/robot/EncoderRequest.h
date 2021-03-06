// Generated by gencpp from file robot/EncoderRequest.msg
// DO NOT EDIT!


#ifndef ROBOT_MESSAGE_ENCODERREQUEST_H
#define ROBOT_MESSAGE_ENCODERREQUEST_H

#include <ros/service_traits.h>


#include <robot/EncoderRequestRequest.h>
#include <robot/EncoderRequestResponse.h>


namespace robot
{

struct EncoderRequest
{

typedef EncoderRequestRequest Request;
typedef EncoderRequestResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct EncoderRequest
} // namespace robot


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::robot::EncoderRequest > {
  static const char* value()
  {
    return "da965117f83be82d823cfdc1c3990723";
  }

  static const char* value(const ::robot::EncoderRequest&) { return value(); }
};

template<>
struct DataType< ::robot::EncoderRequest > {
  static const char* value()
  {
    return "robot/EncoderRequest";
  }

  static const char* value(const ::robot::EncoderRequest&) { return value(); }
};


// service_traits::MD5Sum< ::robot::EncoderRequestRequest> should match 
// service_traits::MD5Sum< ::robot::EncoderRequest > 
template<>
struct MD5Sum< ::robot::EncoderRequestRequest>
{
  static const char* value()
  {
    return MD5Sum< ::robot::EncoderRequest >::value();
  }
  static const char* value(const ::robot::EncoderRequestRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::robot::EncoderRequestRequest> should match 
// service_traits::DataType< ::robot::EncoderRequest > 
template<>
struct DataType< ::robot::EncoderRequestRequest>
{
  static const char* value()
  {
    return DataType< ::robot::EncoderRequest >::value();
  }
  static const char* value(const ::robot::EncoderRequestRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::robot::EncoderRequestResponse> should match 
// service_traits::MD5Sum< ::robot::EncoderRequest > 
template<>
struct MD5Sum< ::robot::EncoderRequestResponse>
{
  static const char* value()
  {
    return MD5Sum< ::robot::EncoderRequest >::value();
  }
  static const char* value(const ::robot::EncoderRequestResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::robot::EncoderRequestResponse> should match 
// service_traits::DataType< ::robot::EncoderRequest > 
template<>
struct DataType< ::robot::EncoderRequestResponse>
{
  static const char* value()
  {
    return DataType< ::robot::EncoderRequest >::value();
  }
  static const char* value(const ::robot::EncoderRequestResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // ROBOT_MESSAGE_ENCODERREQUEST_H
