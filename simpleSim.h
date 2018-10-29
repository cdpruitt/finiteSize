#ifndef SIM_CONSTANTS
#define SIM_CONSTANTS

#include <cmath>

// sample characteristics
const double SAMPLE_RADIUS = 0.412; // in cm
const double SAMPLE_LENGTH = 1.3; // in cm

// detector characteristics
const double DETECTOR_RADIUS = 5; // in cm
const double DETECTOR_DISTANCE = 350; // in cm

// output characteristics
const int NUMBER_ANGLE_BINS = 180;
const double LOW_THETA = 0;
const double HIGH_THETA = 180;
const double TOTAL_SOLID_ANGLE = 4*M_PI*((HIGH_THETA-LOW_THETA)/180);

#endif /* SIM_CONSTANTS */
