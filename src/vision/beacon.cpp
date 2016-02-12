#include "beacon.h"

struct beacon_loc beacon_main(float min, float max) {
	beacon_loc loc;
	loc.angle_from_robot = 45.0;
	loc.distance = 10.0;
	loc.angle_from_beacon = 50.0;
	loc.only_bottom = false;
	loc.beacon_not_found = false;
	loc.beacon_angle_conf = false;
	return loc;
}
