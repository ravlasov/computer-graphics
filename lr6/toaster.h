#ifndef TOASTER_H
#define TOASTER_H
#include <vector>
#include <math.h>
class Toaster
{

public:
    Toaster();

    std::vector<std::vector<double>> getToastHoles(int density = 100, int side = 1);

    std::vector<std::vector<double>> getButtons(int density = 100, int btn = 1);
};

#endif // TOASTER_H
