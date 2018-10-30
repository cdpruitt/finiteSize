#ifndef DETECTOR_H
#define DETECTOR_H

#include "Coordinates.h"
#include "mathFunctions.h"
#include "simpleSim.h"
#include <cmath>

#include <string>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

class Detector
{
    public:
        Detector();
        Detector(double a, double r, double d):
            angle(a), radius(r), distance(d)
    {
        origin.x = distance*cos(toRadians(angle));
        origin.y = distance*sin(toRadians(angle));
        origin.z = 0;

        solidAngle = 2*M_PI*(1-cos(atan(radius/distance)));
        solidAngleFraction = solidAngle/TOTAL_SOLID_ANGLE;

        std::string name = std::to_string(angle);
        std::string XZProjName = name + "XZ";
        std::string YZProjName = name + "YZ";
        std::string hitMapName = name + "hitMap";

        distanceHisto = TH1I(name.c_str(), name.c_str(), 100, 0, radius);
        XZProj = TH2I(XZProjName.c_str(), XZProjName.c_str(),
                100, -2*radius, 2*radius,
                100, -2*radius, 2*radius);

        YZProj = TH2I(YZProjName.c_str(), YZProjName.c_str(),
                100, -2*radius, 2*radius,
                100, -2*radius, 2*radius);

        hitMap = TH3I(hitMapName.c_str(), hitMapName.c_str(),
                100, -2*radius, 2*radius,
                100, -2*radius, 2*radius,
                100, -2*radius, 2*radius);

    }

        double angle;
        double radius;
        double distance;

        double solidAngle;
        double solidAngleFraction;

        TH1I distanceHisto;
        TH2I XZProj;
        TH2I YZProj;
        TH3I hitMap;

        int counts = 0;

        Coordinates origin;
};

#endif /* DETECTOR_H */
