; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude RightMotor.msg.html

(cl:defclass <RightMotor> (roslisp-msg-protocol:ros-message)
  ((rightMotor
    :reader rightMotor
    :initarg :rightMotor
    :type cl:float
    :initform 0.0))
)

(cl:defclass RightMotor (<RightMotor>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <RightMotor>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'RightMotor)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<RightMotor> is deprecated: use robot-msg:RightMotor instead.")))

(cl:ensure-generic-function 'rightMotor-val :lambda-list '(m))
(cl:defmethod rightMotor-val ((m <RightMotor>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:rightMotor-val is deprecated.  Use robot-msg:rightMotor instead.")
  (rightMotor m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <RightMotor>) ostream)
  "Serializes a message object of type '<RightMotor>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'rightMotor))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <RightMotor>) istream)
  "Deserializes a message object of type '<RightMotor>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'rightMotor) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<RightMotor>)))
  "Returns string type for a message object of type '<RightMotor>"
  "robot/RightMotor")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'RightMotor)))
  "Returns string type for a message object of type 'RightMotor"
  "robot/RightMotor")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<RightMotor>)))
  "Returns md5sum for a message object of type '<RightMotor>"
  "1d15360cc4f3d51c27ed554b188cb822")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'RightMotor)))
  "Returns md5sum for a message object of type 'RightMotor"
  "1d15360cc4f3d51c27ed554b188cb822")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<RightMotor>)))
  "Returns full string definition for message of type '<RightMotor>"
  (cl:format cl:nil "float32 rightMotor~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'RightMotor)))
  "Returns full string definition for message of type 'RightMotor"
  (cl:format cl:nil "float32 rightMotor~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <RightMotor>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <RightMotor>))
  "Converts a ROS message object to a list"
  (cl:list 'RightMotor
    (cl:cons ':rightMotor (rightMotor msg))
))
