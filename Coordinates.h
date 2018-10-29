#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>

struct Coordinates
{
    Coordinates() {};
    Coordinates(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
    double dot(const Coordinates& otherPoint)
    {
        return x*otherPoint.x + y*otherPoint.y + z*otherPoint.z;
    }
    
    Coordinates scale(double scalar)
    {
        return Coordinates(x*scalar, y*scalar, z*scalar);
    }

    double distance(const Coordinates& otherPoint)
    {
        return pow(
                pow(x-otherPoint.x,2) +
                pow(y-otherPoint.y,2) +
                pow(z-otherPoint.z,2),
                0.5);
    }

    friend Coordinates operator+(const Coordinates& augend, const Coordinates& addend)
    {
        return Coordinates(augend.x+addend.x, augend.y+addend.y, augend.z+addend.z);
    }

    friend Coordinates operator-(const Coordinates& minuend, const Coordinates& subtrahend)
    {
        return Coordinates(minuend.x-subtrahend.x, minuend.y-subtrahend.y, minuend.z-subtrahend.z);
    }

    friend std::ostream& operator<<(std::ostream& os, const Coordinates& coordinates)
    {
        os << "(" << coordinates.x << ", " << coordinates.y << ", " << coordinates.z << ")";
        return os;
    }

    double x;
    double y;
    double z;
};

#endif /* COORDINATES_H */
