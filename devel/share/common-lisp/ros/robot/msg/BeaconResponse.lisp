; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude BeaconResponse.msg.html

(cl:defclass <BeaconResponse> (roslisp-msg-protocol:ros-message)
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
    :initform 0.0)
   (only_bottom
    :reader only_bottom
    :initarg :only_bottom
    :type cl:boolean
    :initform cl:nil)
   (beacon_not_found
    :reader beacon_not_found
    :initarg :beacon_not_found
    :type cl:boolean
    :initform cl:nil)
   (beacon_angle_conf
    :reader beacon_angle_conf
    :initarg :beacon_angle_conf
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass BeaconResponse (<BeaconResponse>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <BeaconResponse>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'BeaconResponse)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<BeaconResponse> is deprecated: use robot-msg:BeaconResponse instead.")))

(cl:ensure-generic-function 'angle_from_robot-val :lambda-list '(m))
(cl:defmethod angle_from_robot-val ((m <BeaconResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_from_robot-val is deprecated.  Use robot-msg:angle_from_robot instead.")
  (angle_from_robot m))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <BeaconResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:x-val is deprecated.  Use robot-msg:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <BeaconResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:y-val is deprecated.  Use robot-msg:y instead.")
  (y m))

(cl:ensure-generic-function 'only_bottom-val :lambda-list '(m))
(cl:defmethod only_bottom-val ((m <BeaconResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:only_bottom-val is deprecated.  Use robot-msg:only_bottom instead.")
  (only_bottom m))

(cl:ensure-generic-function 'beacon_not_found-val :lambda-list '(m))
(cl:defmethod beacon_not_found-val ((m <BeaconResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:beacon_not_found-val is deprecated.  Use robot-msg:beacon_not_found instead.")
  (beacon_not_found m))

(cl:ensure-generic-function 'beacon_angle_conf-val :lambda-list '(m))
(cl:defmethod beacon_angle_conf-val ((m <BeaconResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:beacon_angle_conf-val is deprecated.  Use robot-msg:beacon_angle_conf instead.")
  (beacon_angle_conf m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <BeaconResponse>) ostream)
  "Serializes a message object of type '<BeaconResponse>"
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
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'only_bottom) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'beacon_not_found) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'beacon_angle_conf) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <BeaconResponse>) istream)
  "Deserializes a message object of type '<BeaconResponse>"
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
    (cl:setf (cl:slot-value msg 'only_bottom) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'beacon_not_found) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'beacon_angle_conf) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<BeaconResponse>)))
  "Returns string type for a message object of type '<BeaconResponse>"
  "robot/BeaconResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'BeaconResponse)))
  "Returns string type for a message object of type 'BeaconResponse"
  "robot/BeaconResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<BeaconResponse>)))
  "Returns md5sum for a message object of type '<BeaconResponse>"
  "b0418911b827747ed56cc16bc2cb6af6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'BeaconResponse)))
  "Returns md5sum for a message object of type 'BeaconResponse"
  "b0418911b827747ed56cc16bc2cb6af6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<BeaconResponse>)))
  "Returns full string definition for message of type '<BeaconResponse>"
  (cl:format cl:nil "float32 angle_from_robot~%float32 x~%float32 y~%bool only_bottom~%bool beacon_not_found~%bool beacon_angle_conf~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'BeaconResponse)))
  "Returns full string definition for message of type 'BeaconResponse"
  (cl:format cl:nil "float32 angle_from_robot~%float32 x~%float32 y~%bool only_bottom~%bool beacon_not_found~%bool beacon_angle_conf~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <BeaconResponse>))
  (cl:+ 0
     4
     4
     4
     1
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <BeaconResponse>))
  "Converts a ROS message object to a list"
  (cl:list 'BeaconResponse
    (cl:cons ':angle_from_robot (angle_from_robot msg))
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':only_bottom (only_bottom msg))
    (cl:cons ':beacon_not_found (beacon_not_found msg))
    (cl:cons ':beacon_angle_conf (beacon_angle_conf msg))
))
