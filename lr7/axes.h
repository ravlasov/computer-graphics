#ifndef AXES_H
#define AXES_H
#include <vector>
#include <math.h>

class Axes
{
public:
    std::vector<std::vector<double>> cone(int density = 100, double distFromZero = 1, double height = 1, double baseRadius = 1, int ax = 0);
};

#endif // AXES_H
