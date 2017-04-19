// Generated by gencpp from file robot/Motors.msg
// DO NOT EDIT!


#ifndef ROBOT_MESSAGE_MOTORS_H
#define ROBOT_MESSAGE_MOTORS_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace robot
{
template <class ContainerAllocator>
struct Motors_
{
  typedef Motors_<ContainerAllocator> Type;

  Motors_()
    : leftMotor(0.0)
    , rightMotor(0.0)  {
    }
  Motors_(const ContainerAllocator& _alloc)
    : leftMotor(0.0)
    , rightMotor(0.0)  {
    }



   typedef float _leftMotor_type;
  _leftMotor_type leftMotor;

   typedef float _rightMotor_type;
  _rightMotor_type rightMotor;




  typedef boost::shared_ptr< ::robot::Motors_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::robot::Motors_<ContainerAllocator> const> ConstPtr;

}; // struct Motors_

typedef ::robot::Motors_<std::allocator<void> > Motors;

typedef boost::shared_ptr< ::robot::Motors > MotorsPtr;
typedef boost::shared_ptr< ::robot::Motors const> MotorsConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::robot::Motors_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::robot::Motors_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace robot

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'robot': ['/home/buckeye/catkin_ws/src/CapstoneROS/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::robot::Motors_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::robot::Motors_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::robot::Motors_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::robot::Motors_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::robot::Motors_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::robot::Motors_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::robot::Motors_<ContainerAllocator> >
{
  static const char* value()
  {
    return "9540d02400fbf39c0861158d46f5ba23";
  }

  static const char* value(const ::robot::Motors_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x9540d02400fbf39cULL;
  static const uint64_t static_value2 = 0x0861158d46f5ba23ULL;
};

template<class ContainerAllocator>
struct DataType< ::robot::Motors_<ContainerAllocator> >
{
  static const char* value()
  {
    return "robot/Motors";
  }

  static const char* value(const ::robot::Motors_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::robot::Motors_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float32 leftMotor\n\
float32 rightMotor\n\
";
  }

  static const char* value(const ::robot::Motors_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::robot::Motors_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.leftMotor);
      stream.next(m.rightMotor);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct Motors_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::robot::Motors_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::robot::Motors_<ContainerAllocator>& v)
  {
    s << indent << "leftMotor: ";
    Printer<float>::stream(s, indent + "  ", v.leftMotor);
    s << indent << "rightMotor: ";
    Printer<float>::stream(s, indent + "  ", v.rightMotor);
  }
};

} // namespace message_operations
} // namespace ros

#endif // ROBOT_MESSAGE_MOTORS_H
