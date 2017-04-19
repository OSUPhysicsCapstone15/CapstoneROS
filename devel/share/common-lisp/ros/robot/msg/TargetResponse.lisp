; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude TargetResponse.msg.html

(cl:defclass <TargetResponse> (roslisp-msg-protocol:ros-message)
  ((angle_from_target
    :reader angle_from_target
    :initarg :angle_from_target
    :type cl:float
    :initform 0.0)
   (distance
    :reader distance
    :initarg :distance
    :type cl:float
    :initform 0.0)
   (target_not_found
    :reader target_not_found
    :initarg :target_not_found
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass TargetResponse (<TargetResponse>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <TargetResponse>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'TargetResponse)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<TargetResponse> is deprecated: use robot-msg:TargetResponse instead.")))

(cl:ensure-generic-function 'angle_from_target-val :lambda-list '(m))
(cl:defmethod angle_from_target-val ((m <TargetResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_from_target-val is deprecated.  Use robot-msg:angle_from_target instead.")
  (angle_from_target m))

(cl:ensure-generic-function 'distance-val :lambda-list '(m))
(cl:defmethod distance-val ((m <TargetResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:distance-val is deprecated.  Use robot-msg:distance instead.")
  (distance m))

(cl:ensure-generic-function 'target_not_found-val :lambda-list '(m))
(cl:defmethod target_not_found-val ((m <TargetResponse>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:target_not_found-val is deprecated.  Use robot-msg:target_not_found instead.")
  (target_not_found m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <TargetResponse>) ostream)
  "Serializes a message object of type '<TargetResponse>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle_from_target))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'distance))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'target_not_found) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <TargetResponse>) istream)
  "Deserializes a message object of type '<TargetResponse>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle_from_target) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'distance) (roslisp-utils:decode-single-float-bits bits)))
    (cl:setf (cl:slot-value msg 'target_not_found) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<TargetResponse>)))
  "Returns string type for a message object of type '<TargetResponse>"
  "robot/TargetResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'TargetResponse)))
  "Returns string type for a message object of type 'TargetResponse"
  "robot/TargetResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<TargetResponse>)))
  "Returns md5sum for a message object of type '<TargetResponse>"
  "13f2edab508c01f49b1491e6283fff1b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'TargetResponse)))
  "Returns md5sum for a message object of type 'TargetResponse"
  "13f2edab508c01f49b1491e6283fff1b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<TargetResponse>)))
  "Returns full string definition for message of type '<TargetResponse>"
  (cl:format cl:nil "float32 angle_from_target ~%float32 distance~%bool target_not_found~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'TargetResponse)))
  "Returns full string definition for message of type 'TargetResponse"
  (cl:format cl:nil "float32 angle_from_target ~%float32 distance~%bool target_not_found~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <TargetResponse>))
  (cl:+ 0
     4
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <TargetResponse>))
  "Converts a ROS message object to a list"
  (cl:list 'TargetResponse
    (cl:cons ':angle_from_target (angle_from_target msg))
    (cl:cons ':distance (distance msg))
    (cl:cons ':target_not_found (target_not_found msg))
))
