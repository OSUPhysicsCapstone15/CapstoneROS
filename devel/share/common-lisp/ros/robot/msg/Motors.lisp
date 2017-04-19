; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude Motors.msg.html

(cl:defclass <Motors> (roslisp-msg-protocol:ros-message)
  ((leftMotor
    :reader leftMotor
    :initarg :leftMotor
    :type cl:float
    :initform 0.0)
   (rightMotor
    :reader rightMotor
    :initarg :rightMotor
    :type cl:float
    :initform 0.0))
)

(cl:defclass Motors (<Motors>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Motors>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Motors)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<Motors> is deprecated: use robot-msg:Motors instead.")))

(cl:ensure-generic-function 'leftMotor-val :lambda-list '(m))
(cl:defmethod leftMotor-val ((m <Motors>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:leftMotor-val is deprecated.  Use robot-msg:leftMotor instead.")
  (leftMotor m))

(cl:ensure-generic-function 'rightMotor-val :lambda-list '(m))
(cl:defmethod rightMotor-val ((m <Motors>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:rightMotor-val is deprecated.  Use robot-msg:rightMotor instead.")
  (rightMotor m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Motors>) ostream)
  "Serializes a message object of type '<Motors>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'leftMotor))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'rightMotor))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Motors>) istream)
  "Deserializes a message object of type '<Motors>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'leftMotor) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'rightMotor) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Motors>)))
  "Returns string type for a message object of type '<Motors>"
  "robot/Motors")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Motors)))
  "Returns string type for a message object of type 'Motors"
  "robot/Motors")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Motors>)))
  "Returns md5sum for a message object of type '<Motors>"
  "9540d02400fbf39c0861158d46f5ba23")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Motors)))
  "Returns md5sum for a message object of type 'Motors"
  "9540d02400fbf39c0861158d46f5ba23")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Motors>)))
  "Returns full string definition for message of type '<Motors>"
  (cl:format cl:nil "float32 leftMotor~%float32 rightMotor~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Motors)))
  "Returns full string definition for message of type 'Motors"
  (cl:format cl:nil "float32 leftMotor~%float32 rightMotor~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Motors>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Motors>))
  "Converts a ROS message object to a list"
  (cl:list 'Motors
    (cl:cons ':leftMotor (leftMotor msg))
    (cl:cons ':rightMotor (rightMotor msg))
))
