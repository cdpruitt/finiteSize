#ifndef CROSS_SECTION_H
#define CROSS_SECTION_H

#include <string>
#include "dataSet.h"

class CrossSection
{
    public:
        CrossSection();
        CrossSection(std::string inputFileName);
        double getValue(double angle);
        double getIntegral();

        DataSet data;
        double max;
};

#endif /* CROSS_SECTION_H */
