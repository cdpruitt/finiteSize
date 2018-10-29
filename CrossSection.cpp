#include "CrossSection.h"
#include "io.h"
#include <iostream>

using namespace std;

CrossSection::CrossSection(string inputFileName)
{
    vector<string> buffer;
    readDataFile(inputFileName, buffer);

    for(auto& line : buffer)
    {
        vector<string> tokens;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        if(tokens.size()!=2)
        {
            cerr << "Error: expected 2 whitespace-delimited tokens from parsing line:"
                << endl << line << " in file " << inputFileName << ", but parsed "
                << tokens.size() << "tokens instead. Exiting..." << endl;
            exit(1);
        }

        double angle = stod(tokens[0]);
        double CS = stod(tokens[1]);

        DataPoint point;
        point.xValue = angle;
        point.yValue = CS;

        data.addPoint(point);
    }

    max = 0;

    for(auto& point : data.getPoints())
    {
        if(point.yValue>max)
        {
            max = point.yValue;
        }
    }
}

double CrossSection::getValue(double angle)
{
    // linear interpolation to find cross section at angle
    for(int i=1; i<data.getNumberOfPoints(); i++)
    {
        if(angle<data.getPoint(i).xValue && angle>data.getPoint(i-1).xValue)
        {
            double deltaY = data.getPoint(i).yValue-data.getPoint(i-1).yValue;
            double deltaX = data.getPoint(i).xValue-data.getPoint(i-1).xValue;
            double stepX = angle-data.getPoint(i-1).xValue;

            return stepX*(deltaY/deltaX)+data.getPoint(i-1).yValue;
        }
    }

    cerr << "Error: could not get cross section value for angle " << angle << endl;
    return -1;
}

double CrossSection::getIntegral()
{
    double sum = 0;
    for(int i=1; i<data.getNumberOfPoints(); i++)
    {
        double averageY = (data.getPoint(i).yValue+data.getPoint(i-1).yValue)/2;
        double deltaX = data.getPoint(i).xValue-data.getPoint(i-1).xValue;

        sum += deltaX*averageY;
    }

    return sum;
}
