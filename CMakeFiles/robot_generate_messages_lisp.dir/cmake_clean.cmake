FILE(REMOVE_RECURSE
  "CMakeFiles/robot_generate_messages_lisp"
  "devel/share/common-lisp/ros/robot/msg/SampleResponse.lisp"
  "devel/share/common-lisp/ros/robot/msg/Commands.lisp"
  "devel/share/common-lisp/ros/robot/msg/BeaconResponse.lisp"
  "devel/share/common-lisp/ros/robot/msg/RightMotor.lisp"
  "devel/share/common-lisp/ros/robot/msg/LeftMotor.lisp"
  "devel/share/common-lisp/ros/robot/msg/BeaconRequest.lisp"
  "devel/share/common-lisp/ros/robot/msg/Motors.lisp"
  "devel/share/common-lisp/ros/robot/msg/TargetResponse.lisp"
  "devel/share/common-lisp/ros/robot/msg/SampleRequest.lisp"
  "devel/share/common-lisp/ros/robot/msg/TargetRequest.lisp"
  "devel/share/common-lisp/ros/robot/srv/EncoderRequest.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/robot_generate_messages_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
