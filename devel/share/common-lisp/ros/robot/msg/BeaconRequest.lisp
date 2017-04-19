; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude BeaconRequest.msg.html

(cl:defclass <BeaconRequest> (roslisp-msg-protocol:ros-message)
  ((angle_from_robot
    :reader angle_from_robot
    :initarg :angle_from_robot
    :type cl:float
    :initform 0.0)
   (x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0))
)

(cl:defclass BeaconRequest (<BeaconRequest>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <BeaconRequest>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'BeaconRequest)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<BeaconRequest> is deprecated: use robot-msg:BeaconRequest instead.")))

(cl:ensure-generic-function 'angle_from_robot-val :lambda-list '(m))
(cl:defmethod angle_from_robot-val ((m <BeaconRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_from_robot-val is deprecated.  Use robot-msg:angle_from_robot instead.")
  (angle_from_robot m))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <BeaconRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:x-val is deprecated.  Use robot-msg:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <BeaconRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:y-val is deprecated.  Use robot-msg:y instead.")
  (y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <BeaconRequest>) ostream)
  "Serializes a message object of type '<BeaconRequest>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle_from_robot))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <BeaconRequest>) istream)
  "Deserializes a message object of type '<BeaconRequest>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle_from_robot) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<BeaconRequest>)))
  "Returns string type for a message object of type '<BeaconRequest>"
  "robot/BeaconRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'BeaconRequest)))
  "Returns string type for a message object of type 'BeaconRequest"
  "robot/BeaconRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<BeaconRequest>)))
  "Returns md5sum for a message object of type '<BeaconRequest>"
  "556aa70db00b20ecb2bb8618209e1b41")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'BeaconRequest)))
  "Returns md5sum for a message object of type 'BeaconRequest"
  "556aa70db00b20ecb2bb8618209e1b41")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<BeaconRequest>)))
  "Returns full string definition for message of type '<BeaconRequest>"
  (cl:format cl:nil "float32 angle_from_robot~%float32 x~%float32 y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'BeaconRequest)))
  "Returns full string definition for message of type 'BeaconRequest"
  (cl:format cl:nil "float32 angle_from_robot~%float32 x~%float32 y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <BeaconRequest>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <BeaconRequest>))
  "Converts a ROS message object to a list"
  (cl:list 'BeaconRequest
    (cl:cons ':angle_from_robot (angle_from_robot msg))
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
))
