#ifndef SIM_CONSTANTS
#define SIM_CONSTANTS

#include <cmath>

// sample characteristics
const double SAMPLE_RADIUS = 10*0.412; // in cm
const double SAMPLE_LENGTH = 10*1.30; // in cm

// detector characteristics
const double DETECTOR_RADIUS = 5; // in cm
const double DETECTOR_DISTANCE = 350; // in cm

// output characteristics
const int NUMBER_ANGLE_BINS = 180;
const double LOW_CS_ANGLE = 0;
const double HIGH_CS_ANGLE = 180;
const double MAX_Z = (DETECTOR_RADIUS + SAMPLE_LENGTH);

const double CONE_ANGLE = atan(MAX_Z/DETECTOR_DISTANCE);
const double TOTAL_SOLID_ANGLE = 4*M_PI*(cos(M_PI/2-CONE_ANGLE))*((HIGH_CS_ANGLE-LOW_CS_ANGLE)/180);

#endif /* SIM_CONSTANTS */
