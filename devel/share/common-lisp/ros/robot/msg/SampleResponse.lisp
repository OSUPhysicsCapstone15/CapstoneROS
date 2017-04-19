; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude SampleResponse.msg.html

(cl:defclass <SampleResponse> (roslisp-msg-protocol:ros-message)
  ((angle_from_robot
    :reader angle_from_robot
    :initarg :angle_from_robot
    :type cl:float
    :initform 0.0)
   (distance
    :reader distance
    :initarg :distance
    :type cl:float
    :initform 0.0)
   (sample_not_found
    :reader sample_not_found
    :initarg :sample_not_found
    :type cl:boolean
    :initform cl:nil)
   (sample_angle_conf
    :reader sample_angle_conf
    :initarg :sample_angle_conf
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass SampleResponse (<SampleResponse>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SampleResponse>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SampleResponse)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<SampleResponse> is deprecated: use robot-msg:SampleResponse instead.")))

(cl:ensure-generic-function 'angle_from_robot-val :lambda-list '(m))
(cl:defmethod angle_from_robot-val ((m <SampleResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_from_robot-val is deprecated.  Use robot-msg:angle_from_robot instead.")
  (angle_from_robot m))

(cl:ensure-generic-function 'distance-val :lambda-list '(m))
(cl:defmethod distance-val ((m <SampleResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:distance-val is deprecated.  Use robot-msg:distance instead.")
  (distance m))

(cl:ensure-generic-function 'sample_not_found-val :lambda-list '(m))
(cl:defmethod sample_not_found-val ((m <SampleResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:sample_not_found-val is deprecated.  Use robot-msg:sample_not_found instead.")
  (sample_not_found m))

(cl:ensure-generic-function 'sample_angle_conf-val :lambda-list '(m))
(cl:defmethod sample_angle_conf-val ((m <SampleResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:sample_angle_conf-val is deprecated.  Use robot-msg:sample_angle_conf instead.")
  (sample_angle_conf m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SampleResponse>) ostream)
  "Serializes a message object of type '<SampleResponse>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle_from_robot))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'distance))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'sample_not_found) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'sample_angle_conf) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SampleResponse>) istream)
  "Deserializes a message object of type '<SampleResponse>"
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
    (cl:setf (cl:slot-value msg 'distance) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:slot-value msg 'sample_not_found) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'sample_angle_conf) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SampleResponse>)))
  "Returns string type for a message object of type '<SampleResponse>"
  "robot/SampleResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SampleResponse)))
  "Returns string type for a message object of type 'SampleResponse"
  "robot/SampleResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SampleResponse>)))
  "Returns md5sum for a message object of type '<SampleResponse>"
  "f47f20f3085b0c4c42ff4a174efe2a9d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SampleResponse)))
  "Returns md5sum for a message object of type 'SampleResponse"
  "f47f20f3085b0c4c42ff4a174efe2a9d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SampleResponse>)))
  "Returns full string definition for message of type '<SampleResponse>"
  (cl:format cl:nil "float32 angle_from_robot~%float32 distance~%bool sample_not_found~%bool sample_angle_conf~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SampleResponse)))
  "Returns full string definition for message of type 'SampleResponse"
  (cl:format cl:nil "float32 angle_from_robot~%float32 distance~%bool sample_not_found~%bool sample_angle_conf~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SampleResponse>))
  (cl:+ 0
     4
     4
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SampleResponse>))
  "Converts a ROS message object to a list"
  (cl:list 'SampleResponse
    (cl:cons ':angle_from_robot (angle_from_robot msg))
    (cl:cons ':distance (distance msg))
    (cl:cons ':sample_not_found (sample_not_found msg))
    (cl:cons ':sample_angle_conf (sample_angle_conf msg))
))
