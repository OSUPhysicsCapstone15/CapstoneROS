# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from robot/SampleRequest.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class SampleRequest(genpy.Message):
  _md5sum = "fc8527015f1fbcba2ccc6b6bc5e725f3"
  _type = "robot/SampleRequest"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """float32 angle_min
float32 angle_max
bool whiteSample

"""
  __slots__ = ['angle_min','angle_max','whiteSample']
  _slot_types = ['float32','float32','bool']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       angle_min,angle_max,whiteSample

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(SampleRequest, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.angle_min is None:
        self.angle_min = 0.
      if self.angle_max is None:
        self.angle_max = 0.
      if self.whiteSample is None:
        self.whiteSample = False
    else:
      self.angle_min = 0.
      self.angle_max = 0.
      self.whiteSample = False

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
      buff.write(_struct_2fB.pack(_x.angle_min, _x.angle_max, _x.whiteSample))
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
      end += 9
      (_x.angle_min, _x.angle_max, _x.whiteSample,) = _struct_2fB.unpack(str[start:end])
      self.whiteSample = bool(self.whiteSample)
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
      buff.write(_struct_2fB.pack(_x.angle_min, _x.angle_max, _x.whiteSample))
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
      end += 9
      (_x.angle_min, _x.angle_max, _x.whiteSample,) = _struct_2fB.unpack(str[start:end])
      self.whiteSample = bool(self.whiteSample)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_2fB = struct.Struct("<2fB")
