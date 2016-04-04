#ifndef beacon_H_INCLUDED
#define beacon_H_INCLUDED


struct beacon_loc {
	float angle_from_robot=0; //degrees for all angles
	float x=0; //meters
	float y=0;
	bool only_bottom=0;
	bool beacon_not_found=0;
	bool beacon_angle_conf=0;
};


void beacon_main(beacon_loc &b_loc);

#endif
