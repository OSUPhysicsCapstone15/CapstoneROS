FILE(REMOVE_RECURSE
  "CMakeFiles/robot_generate_messages_py"
  "devel/lib/python2.7/dist-packages/robot/msg/_SampleResponse.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_Commands.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_BeaconResponse.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_RightMotor.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_LeftMotor.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_BeaconRequest.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_Motors.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_TargetResponse.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_SampleRequest.py"
  "devel/lib/python2.7/dist-packages/robot/msg/_TargetRequest.py"
  "devel/lib/python2.7/dist-packages/robot/srv/_EncoderRequest.py"
  "devel/lib/python2.7/dist-packages/robot/msg/__init__.py"
  "devel/lib/python2.7/dist-packages/robot/srv/__init__.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/robot_generate_messages_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
