#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <cmath>

double toRadians(double degrees)
{
    return degrees*(M_PI/180);
}

double toDegrees(double radians)
{
    return radians*(180/M_PI);
}

#endif /* MATH_FUNCTIONS_H */
