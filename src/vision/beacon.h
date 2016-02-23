#ifndef beacon_H_INCLUDED
#define beacon_H_INCLUDED

#include "opencv2/highgui/highgui.hpp"

class Beacon {
private:
	float angle_from_robot=0; //degrees for all angles
	float distance=0; //meters
	float angle_from_beacon=0;
	bool only_bottom=false;
	bool beacon_not_found=true;
	bool beacon_angle_conf=false;

public:
	Beacon();
	~Beacon();
	void capture(); //updates known position of beacon
	float getDistance() { return distance; }		
	float getRobAng() { return angle_from_robot; }
	float getBeacAng() { return angle_from_beacon; }
	bool isOnlyBottom() { return only_bottom; }
	bool isNotFound() { return beacon_not_found; }
	bool isConfident() { return beacon_angle_conf; }
	void closeWindows() { cv::destroyAllWindows(); }
};

#endif
