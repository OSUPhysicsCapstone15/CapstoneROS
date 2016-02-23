#ifndef beacon_H_INCLUDED
#define beacon_H_INCLUDED


struct beacon_loc {
	float angle_from_robot=0; //degrees for all angles
	float distance=0; //meters
	float angle_from_beacon=0;
	bool only_bottom=0;
	bool beacon_not_found=0;
	bool beacon_angle_conf=0;
};


beacon_loc beacon_main(float min, float max);

#endif
