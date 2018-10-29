#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include "TRandom3.h"

class RandomGenerator
{
    public:
        RandomGenerator();
        RandomGenerator(double low, double high)
        {
            engine = TRandom3(0);

            lowLimit = low;
            highLimit = high;

            //generator = std::mt19937(randomDevice());
            //distribution = std::uniform_real_distribution<double>(lowLimit, highLimit);
        }

        double getValue()
        {
            return lowLimit + (highLimit-lowLimit)*engine.Rndm();
        }

    private:
        TRandom3 engine;

        double lowLimit;
        double highLimit;

        //std::mt19937 generator;
        //std::uniform_real_distribution<double> distribution;
};

#endif /* RANDOM_GENERATOR_H */
