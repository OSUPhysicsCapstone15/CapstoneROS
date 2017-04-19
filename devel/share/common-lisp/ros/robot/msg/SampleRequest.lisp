; Auto-generated. Do not edit!


(cl:in-package robot-msg)


;//! \htmlinclude SampleRequest.msg.html

(cl:defclass <SampleRequest> (roslisp-msg-protocol:ros-message)
  ((angle_min
    :reader angle_min
    :initarg :angle_min
    :type cl:float
    :initform 0.0)
   (angle_max
    :reader angle_max
    :initarg :angle_max
    :type cl:float
    :initform 0.0)
   (whiteSample
    :reader whiteSample
    :initarg :whiteSample
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass SampleRequest (<SampleRequest>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SampleRequest>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SampleRequest)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-msg:<SampleRequest> is deprecated: use robot-msg:SampleRequest instead.")))

(cl:ensure-generic-function 'angle_min-val :lambda-list '(m))
(cl:defmethod angle_min-val ((m <SampleRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_min-val is deprecated.  Use robot-msg:angle_min instead.")
  (angle_min m))

(cl:ensure-generic-function 'angle_max-val :lambda-list '(m))
(cl:defmethod angle_max-val ((m <SampleRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:angle_max-val is deprecated.  Use robot-msg:angle_max instead.")
  (angle_max m))

(cl:ensure-generic-function 'whiteSample-val :lambda-list '(m))
(cl:defmethod whiteSample-val ((m <SampleRequest>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-msg:whiteSample-val is deprecated.  Use robot-msg:whiteSample instead.")
  (whiteSample m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SampleRequest>) ostream)
  "Serializes a message object of type '<SampleRequest>"
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
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'whiteSample) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SampleRequest>) istream)
  "Deserializes a message object of type '<SampleRequest>"
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
    (cl:setf (cl:slot-value msg 'whiteSample) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SampleRequest>)))
  "Returns string type for a message object of type '<SampleRequest>"
  "robot/SampleRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SampleRequest)))
  "Returns string type for a message object of type 'SampleRequest"
  "robot/SampleRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SampleRequest>)))
  "Returns md5sum for a message object of type '<SampleRequest>"
  "fc8527015f1fbcba2ccc6b6bc5e725f3")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SampleRequest)))
  "Returns md5sum for a message object of type 'SampleRequest"
  "fc8527015f1fbcba2ccc6b6bc5e725f3")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SampleRequest>)))
  "Returns full string definition for message of type '<SampleRequest>"
  (cl:format cl:nil "float32 angle_min~%float32 angle_max~%bool whiteSample~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SampleRequest)))
  "Returns full string definition for message of type 'SampleRequest"
  (cl:format cl:nil "float32 angle_min~%float32 angle_max~%bool whiteSample~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SampleRequest>))
  (cl:+ 0
     4
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SampleRequest>))
  "Converts a ROS message object to a list"
  (cl:list 'SampleRequest
    (cl:cons ':angle_min (angle_min msg))
    (cl:cons ':angle_max (angle_max msg))
    (cl:cons ':whiteSample (whiteSample msg))
))
