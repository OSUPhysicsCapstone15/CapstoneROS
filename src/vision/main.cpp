#include "blob.h"
#include "beacon.h"
#include "ros/ros.h"

int main(){
/*
  to test beacon.cpp */
	
	beacon_loc bloc;
	bloc.x=2.9;
	bloc.y=9.3;
	bloc.angle_from_robot=0;
	beacon_main(bloc);

/* to test blob.cpp
 
	sample_loc sloc;
	blob_main(sloc);
*/

return 0;
}
