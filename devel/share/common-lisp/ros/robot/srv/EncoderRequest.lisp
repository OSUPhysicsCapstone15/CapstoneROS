; Auto-generated. Do not edit!


(cl:in-package robot-srv)


;//! \htmlinclude EncoderRequest-request.msg.html

(cl:defclass <EncoderRequest-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass EncoderRequest-request (<EncoderRequest-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <EncoderRequest-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'EncoderRequest-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-srv:<EncoderRequest-request> is deprecated: use robot-srv:EncoderRequest-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <EncoderRequest-request>) ostream)
  "Serializes a message object of type '<EncoderRequest-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <EncoderRequest-request>) istream)
  "Deserializes a message object of type '<EncoderRequest-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<EncoderRequest-request>)))
  "Returns string type for a service object of type '<EncoderRequest-request>"
  "robot/EncoderRequestRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'EncoderRequest-request)))
  "Returns string type for a service object of type 'EncoderRequest-request"
  "robot/EncoderRequestRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<EncoderRequest-request>)))
  "Returns md5sum for a message object of type '<EncoderRequest-request>"
  "da965117f83be82d823cfdc1c3990723")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'EncoderRequest-request)))
  "Returns md5sum for a message object of type 'EncoderRequest-request"
  "da965117f83be82d823cfdc1c3990723")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<EncoderRequest-request>)))
  "Returns full string definition for message of type '<EncoderRequest-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'EncoderRequest-request)))
  "Returns full string definition for message of type 'EncoderRequest-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <EncoderRequest-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <EncoderRequest-request>))
  "Converts a ROS message object to a list"
  (cl:list 'EncoderRequest-request
))
;//! \htmlinclude EncoderRequest-response.msg.html

(cl:defclass <EncoderRequest-response> (roslisp-msg-protocol:ros-message)
  ((leftCount
    :reader leftCount
    :initarg :leftCount
    :type cl:integer
    :initform 0)
   (rightCount
    :reader rightCount
    :initarg :rightCount
    :type cl:integer
    :initform 0))
)

(cl:defclass EncoderRequest-response (<EncoderRequest-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <EncoderRequest-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'EncoderRequest-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name robot-srv:<EncoderRequest-response> is deprecated: use robot-srv:EncoderRequest-response instead.")))

(cl:ensure-generic-function 'leftCount-val :lambda-list '(m))
(cl:defmethod leftCount-val ((m <EncoderRequest-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-srv:leftCount-val is deprecated.  Use robot-srv:leftCount instead.")
  (leftCount m))

(cl:ensure-generic-function 'rightCount-val :lambda-list '(m))
(cl:defmethod rightCount-val ((m <EncoderRequest-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader robot-srv:rightCount-val is deprecated.  Use robot-srv:rightCount instead.")
  (rightCount m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <EncoderRequest-response>) ostream)
  "Serializes a message object of type '<EncoderRequest-response>"
  (cl:let* ((signed (cl:slot-value msg 'leftCount)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'rightCount)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <EncoderRequest-response>) istream)
  "Deserializes a message object of type '<EncoderRequest-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'leftCount) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'rightCount) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<EncoderRequest-response>)))
  "Returns string type for a service object of type '<EncoderRequest-response>"
  "robot/EncoderRequestResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'EncoderRequest-response)))
  "Returns string type for a service object of type 'EncoderRequest-response"
  "robot/EncoderRequestResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<EncoderRequest-response>)))
  "Returns md5sum for a message object of type '<EncoderRequest-response>"
  "da965117f83be82d823cfdc1c3990723")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'EncoderRequest-response)))
  "Returns md5sum for a message object of type 'EncoderRequest-response"
  "da965117f83be82d823cfdc1c3990723")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<EncoderRequest-response>)))
  "Returns full string definition for message of type '<EncoderRequest-response>"
  (cl:format cl:nil "int64 leftCount~%int64 rightCount~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'EncoderRequest-response)))
  "Returns full string definition for message of type 'EncoderRequest-response"
  (cl:format cl:nil "int64 leftCount~%int64 rightCount~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <EncoderRequest-response>))
  (cl:+ 0
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <EncoderRequest-response>))
  "Converts a ROS message object to a list"
  (cl:list 'EncoderRequest-response
    (cl:cons ':leftCount (leftCount msg))
    (cl:cons ':rightCount (rightCount msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'EncoderRequest)))
  'EncoderRequest-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'EncoderRequest)))
  'EncoderRequest-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'EncoderRequest)))
  "Returns string type for a service object of type '<EncoderRequest>"
  "robot/EncoderRequest")