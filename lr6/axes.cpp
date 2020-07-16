#include "axes.h"
#include <iostream>
#include <fstream>

std::vector<std::vector<double>> Axes::cone(int density, double distFromZero, double height, double baseRadius, int ax)
{
    std::vector<std::vector<double>> res;
    double stepHeight = height / density;
    double stepRad = baseRadius / density;
    double radius = baseRadius;
    double x,y,z,nx,ny,nz,tx=0,ty=0;
    unsigned int it = 0;
    switch (ax) {
        case 0: // ox
            for (double h = 0; h <= height; h += stepHeight) {
                res.push_back(std::vector<double>());
                for (double t = 0; t <= 2 * M_PI; t += 2 * M_PI / density) {
                    x = distFromZero + h;
                    y = radius * sin(t);
                    z = radius * cos(t);
                    nx = radius / h;
                    ny = sin(t);
                    nz = cos(t);
                    res.at(it).push_back(x);
                    res.at(it).push_back(y);
                    res.at(it).push_back(z);
                    res.at(it).push_back(nx);
                    res.at(it).push_back(ny);
                    res.at(it).push_back(nz);
                    res.at(it).push_back(tx);
                    res.at(it).push_back(ty);
                }
                radius -= stepRad;
                it++;
            }
            for (double t = 0; t <= 2 * M_PI; t += 2 * M_PI / density) {
                res.push_back(std::vector<double>());
		x = -distFromZero;
                y = baseRadius/20 * sin(t);
                z = baseRadius/20 * cos(t);
                nx = 0;
                ny = sin(t);
                nz = cos(t);
                res.back().push_back(x);
                res.back().push_back(y);
                res.back().push_back(z);
                res.back().push_back(nx);
                res.back().push_back(ny);
                res.back().push_back(nz);
                res.back().push_back(tx);
                res.back().push_back(ty);

                res.back().push_back(-x);
                res.back().push_back(y);
                res.back().push_back(z);
                res.back().push_back(nx);
                res.back().push_back(ny);
                res.back().push_back(nz);
                res.back().push_back(tx);
                res.back().push_back(ty);
	    }

            break;
        case 1: //oy
            for (double h = 0; h <= height; h += stepHeight) {
                res.push_back(std::vector<double>());
                for (double t = 0; t <= 2 * M_PI; t += 2 * M_PI / density) {
                    x = radius * sin(t);
                    y = distFromZero + h;
                    z = radius * cos(t);
                    nx = sin(t);
                    ny = radius / h;
                    nz = cos(t);
                    res.at(it).push_back(x);
                    res.at(it).push_back(y);
                    res.at(it).push_back(z);
                    res.at(it).push_back(nx);
                    res.at(it).push_back(ny);
                    res.at(it).push_back(nz);
                    res.at(it).push_back(tx);
                    res.at(it).push_back(ty);
                }
                radius -= stepRad;
                it++;
            }
            break;
        case 2: //oz
            for (double h = 0; h <= height; h += stepHeight) {
                res.push_back(std::vector<double>());
                for (double t = 0; t <= 2 * M_PI; t += 2 * M_PI / density) {
                    x = radius * cos(t);
                    y = radius * sin(t);
                    z = distFromZero + h;
                    nx = cos(t);
                    ny = sin(t);
                    nz = radius / h;
                    res.at(it).push_back(x);
                    res.at(it).push_back(y);
                    res.at(it).push_back(z);
                    res.at(it).push_back(nx);
                    res.at(it).push_back(ny);
                    res.at(it).push_back(nz);
                    res.at(it).push_back(tx);
                    res.at(it).push_back(ty);
                }
                radius -= stepRad;
                it++;
            }
            break;
    }
    return res;
}
