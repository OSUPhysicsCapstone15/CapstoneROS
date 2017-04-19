
(cl:in-package :asdf)

(defsystem "robot-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "EncoderRequest" :depends-on ("_package_EncoderRequest"))
    (:file "_package_EncoderRequest" :depends-on ("_package"))
  ))