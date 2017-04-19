; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude LeftMotor.msg.html

(cl:defclass <LeftMotor> (roslisp-msg-protocol:ros-message)
  ((leftMotor
    :reader leftMotor
    :initarg :leftMotor
    :type cl:float
    :initform 0.0))
)

(cl:defclass LeftMotor (<LeftMotor>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <LeftMotor>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'LeftMotor)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<LeftMotor> is deprecated: use robot-msg:LeftMotor instead.")))

(cl:ensure-generic-function 'leftMotor-val :lambda-list '(m))
(cl:defmethod leftMotor-val ((m <LeftMotor>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:leftMotor-val is deprecated.  Use robot-msg:leftMotor instead.")
  (leftMotor m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <LeftMotor>) ostream)
  "Serializes a message object of type '<LeftMotor>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'leftMotor))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <LeftMotor>) istream)
  "Deserializes a message object of type '<LeftMotor>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'leftMotor) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<LeftMotor>)))
  "Returns string type for a message object of type '<LeftMotor>"
  "robot/LeftMotor")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'LeftMotor)))
  "Returns string type for a message object of type 'LeftMotor"
  "robot/LeftMotor")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<LeftMotor>)))
  "Returns md5sum for a message object of type '<LeftMotor>"
  "2b05e1ef5e5f1a1141ed27c07178eedc")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'LeftMotor)))
  "Returns md5sum for a message object of type 'LeftMotor"
  "2b05e1ef5e5f1a1141ed27c07178eedc")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<LeftMotor>)))
  "Returns full string definition for message of type '<LeftMotor>"
  (cl:format cl:nil "float32 leftMotor~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'LeftMotor)))
  "Returns full string definition for message of type 'LeftMotor"
  (cl:format cl:nil "float32 leftMotor~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <LeftMotor>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <LeftMotor>))
  "Converts a ROS message object to a list"
  (cl:list 'LeftMotor
    (cl:cons ':leftMotor (leftMotor msg))
))
