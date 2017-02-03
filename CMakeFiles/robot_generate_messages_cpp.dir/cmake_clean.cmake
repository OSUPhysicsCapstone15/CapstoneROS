FILE(REMOVE_RECURSE
  "CMakeFiles/robot_generate_messages_cpp"
  "devel/include/robot/SampleResponse.h"
  "devel/include/robot/Commands.h"
  "devel/include/robot/BeaconResponse.h"
  "devel/include/robot/RightMotor.h"
  "devel/include/robot/LeftMotor.h"
  "devel/include/robot/BeaconRequest.h"
  "devel/include/robot/Motors.h"
  "devel/include/robot/TargetResponse.h"
  "devel/include/robot/SampleRequest.h"
  "devel/include/robot/TargetRequest.h"
  "devel/include/robot/EncoderRequest.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/robot_generate_messages_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
