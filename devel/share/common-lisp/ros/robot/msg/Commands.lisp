; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude Commands.msg.html

(cl:defclass <Commands> (roslisp-msg-protocol:ros-message)
  ((commandOrder
    :reader commandOrder
    :initarg :commandOrder
    :type cl:integer
    :initform 0)
   (value
    :reader value
    :initarg :value
    :type cl:float
    :initform 0.0))
)

(cl:defclass Commands (<Commands>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Commands>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Commands)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<Commands> is deprecated: use robot-msg:Commands instead.")))

(cl:ensure-generic-function 'commandOrder-val :lambda-list '(m))
(cl:defmethod commandOrder-val ((m <Commands>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:commandOrder-val is deprecated.  Use robot-msg:commandOrder instead.")
  (commandOrder m))

(cl:ensure-generic-function 'value-val :lambda-list '(m))
(cl:defmethod value-val ((m <Commands>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:value-val is deprecated.  Use robot-msg:value instead.")
  (value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Commands>) ostream)
  "Serializes a message object of type '<Commands>"
  (cl:let* ((signed (cl:slot-value msg 'commandOrder)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'value))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Commands>) istream)
  "Deserializes a message object of type '<Commands>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'commandOrder) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'value) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Commands>)))
  "Returns string type for a message object of type '<Commands>"
  "robot/Commands")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Commands)))
  "Returns string type for a message object of type 'Commands"
  "robot/Commands")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Commands>)))
  "Returns md5sum for a message object of type '<Commands>"
  "eea913a9357e05e448c0c8f8d935dabb")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Commands)))
  "Returns md5sum for a message object of type 'Commands"
  "eea913a9357e05e448c0c8f8d935dabb")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Commands>)))
  "Returns full string definition for message of type '<Commands>"
  (cl:format cl:nil "int32 commandOrder~%float32 value~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Commands)))
  "Returns full string definition for message of type 'Commands"
  (cl:format cl:nil "int32 commandOrder~%float32 value~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Commands>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Commands>))
  "Converts a ROS message object to a list"
  (cl:list 'Commands
    (cl:cons ':commandOrder (commandOrder msg))
    (cl:cons ':value (value msg))
))
