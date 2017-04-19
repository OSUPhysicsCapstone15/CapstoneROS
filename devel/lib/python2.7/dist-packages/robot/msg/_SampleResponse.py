# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from robot/SampleResponse.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class SampleResponse(genpy.Message):
  _md5sum = "f47f20f3085b0c4c42ff4a174efe2a9d"
  _type = "robot/SampleResponse"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """float32 angle_from_robot
float32 distance
bool sample_not_found
bool sample_angle_conf

"""
  __slots__ = ['angle_from_robot','distance','sample_not_found','sample_angle_conf']
  _slot_types = ['float32','float32','bool','bool']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       angle_from_robot,distance,sample_not_found,sample_angle_conf

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(SampleResponse, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.angle_from_robot is None:
        self.angle_from_robot = 0.
      if self.distance is None:
        self.distance = 0.
      if self.sample_not_found is None:
        self.sample_not_found = False
      if self.sample_angle_conf is None:
        self.sample_angle_conf = False
    else:
      self.angle_from_robot = 0.
      self.distance = 0.
      self.sample_not_found = False
      self.sample_angle_conf = False

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_2f2B.pack(_x.angle_from_robot, _x.distance, _x.sample_not_found, _x.sample_angle_conf))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 10
      (_x.angle_from_robot, _x.distance, _x.sample_not_found, _x.sample_angle_conf,) = _struct_2f2B.unpack(str[start:end])
      self.sample_not_found = bool(self.sample_not_found)
      self.sample_angle_conf = bool(self.sample_angle_conf)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_2f2B.pack(_x.angle_from_robot, _x.distance, _x.sample_not_found, _x.sample_angle_conf))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 10
      (_x.angle_from_robot, _x.distance, _x.sample_not_found, _x.sample_angle_conf,) = _struct_2f2B.unpack(str[start:end])
      self.sample_not_found = bool(self.sample_not_found)
      self.sample_angle_conf = bool(self.sample_angle_conf)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_2f2B = struct.Struct("<2f2B")
