; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude TargetRequest.msg.html

(cl:defclass <TargetRequest> (roslisp-msg-protocol:ros-message)
  ((angle_min
    :reader angle_min
    :initarg :angle_min
    :type cl:float
    :initform 0.0)
   (angle_max
    :reader angle_max
    :initarg :angle_max
    :type cl:float
    :initform 0.0))
)

(cl:defclass TargetRequest (<TargetRequest>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <TargetRequest>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'TargetRequest)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<TargetRequest> is deprecated: use robot-msg:TargetRequest instead.")))

(cl:ensure-generic-function 'angle_min-val :lambda-list '(m))
(cl:defmethod angle_min-val ((m <TargetRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_min-val is deprecated.  Use robot-msg:angle_min instead.")
  (angle_min m))

(cl:ensure-generic-function 'angle_max-val :lambda-list '(m))
(cl:defmethod angle_max-val ((m <TargetRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_max-val is deprecated.  Use robot-msg:angle_max instead.")
  (angle_max m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <TargetRequest>) ostream)
  "Serializes a message object of type '<TargetRequest>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle_min))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angle_max))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <TargetRequest>) istream)
  "Deserializes a message object of type '<TargetRequest>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle_min) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angle_max) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<TargetRequest>)))
  "Returns string type for a message object of type '<TargetRequest>"
  "robot/TargetRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'TargetRequest)))
  "Returns string type for a message object of type 'TargetRequest"
  "robot/TargetRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<TargetRequest>)))
  "Returns md5sum for a message object of type '<TargetRequest>"
  "49e833ef41cffc32cbca68e3428713bf")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'TargetRequest)))
  "Returns md5sum for a message object of type 'TargetRequest"
  "49e833ef41cffc32cbca68e3428713bf")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<TargetRequest>)))
  "Returns full string definition for message of type '<TargetRequest>"
  (cl:format cl:nil "float32 angle_min~%float32 angle_max~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'TargetRequest)))
  "Returns full string definition for message of type 'TargetRequest"
  (cl:format cl:nil "float32 angle_min~%float32 angle_max~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <TargetRequest>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <TargetRequest>))
  "Converts a ROS message object to a list"
  (cl:list 'TargetRequest
    (cl:cons ':angle_min (angle_min msg))
    (cl:cons ':angle_max (angle_max msg))
))
