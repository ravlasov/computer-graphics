#include "toaster.h"
#include <iostream>
#include <fstream>

Toaster::Toaster()
{

}

std::vector<std::vector<double>> Toaster::getToastHoles(int density, int side)
{
    double bottomZ = -0.6;
    double x, y, z, nx, ny, nz, tx = 0, ty = 0;
    unsigned long it = 0;
    std::vector<std::vector<double>> res;
    for (z = 1; z >= bottomZ; z -= 1.0 / density) {
        res.push_back(std::vector<double>());
        for (double t = 0; t <= 1; t += 1.0 / density) {
            x = side * 0.2;
            y = side * 1.6 * (t - 0.5);
            nx = side;
            ny = 0;
            nz = 0;
            res.at(it).push_back(x);
            res.at(it).push_back(y);
            res.at(it).push_back(z);
            res.at(it).push_back(nx);
            res.at(it).push_back(ny);
            res.at(it).push_back(nz);
            res.at(it).push_back(tx);
            res.at(it).push_back(ty);
        }
        for (double t = 0; t <= 1; t += 1.0 / density) {
            x = side * (0.2 + 0.6 * t);
            y = side * 0.8;
            nx = 0;
            ny = -side;
            nz = 0;
            res.at(it).push_back(x);
            res.at(it).push_back(y);
            res.at(it).push_back(z);
            res.at(it).push_back(nx);
            res.at(it).push_back(ny);
            res.at(it).push_back(nz);
            res.at(it).push_back(tx);
            res.at(it).push_back(ty);
        }
        for (double t = 1; t >= 0; t -= 1.0 / density) {
            x = side * 0.8;
            y = side * 1.6 * (t - 0.5);
            nx = -side;
            ny = 0;
            nz = 0;
            res.at(it).push_back(x);
            res.at(it).push_back(y);
            res.at(it).push_back(z);
            res.at(it).push_back(nx);
            res.at(it).push_back(ny);
            res.at(it).push_back(nz);
            res.at(it).push_back(tx);
            res.at(it).push_back(ty);
        }
        for (double t = 0; t <= 1; t += 1.0 / density) {
            x = side * (0.2 + 0.6 * t);
            y = -0.8 * side;
            nx = 0;
            ny = side;
            nz = 0;
            res.at(it).push_back(x);
            res.at(it).push_back(y);
            res.at(it).push_back(z);
            res.at(it).push_back(nx);
            res.at(it).push_back(ny);
            res.at(it).push_back(nz);
            res.at(it).push_back(tx);
            res.at(it).push_back(ty);
        }
        it++;
    }
    res.push_back(std::vector<double>());
    for (double x = (side > 0 ? 0.2 : -0.8); x <= (side > 0 ? 0.8 : -0.2); x += 1.0 / density) {
        res.push_back(std::vector<double>());
        for (double y = -0.8; y <= 0.8; y += 1.0 / density) {
            z = bottomZ;
            nx = 0;
            ny = 0;
            nz = 1;
            res.at(it).push_back(x);
            res.at(it).push_back(y);
            res.at(it).push_back(z);
            res.at(it).push_back(nx);
            res.at(it).push_back(ny);
            res.at(it).push_back(nz);
            res.at(it).push_back(tx);
            res.at(it).push_back(ty);
        }
        it++;
    }

    return res;
}

std::vector<std::vector<double>> Toaster::getButtons(int density, int btn)
{
    double bottomZ = 1.2;
    double x, y, z, nx, ny, nz, tx = 0, ty = 0;
    unsigned long it = 0;
    std::vector<std::vector<double>> res;

    for (x = 1; x <= bottomZ; x += 1.0 / density) {
        res.push_back(std::vector<double>());
        for (double t = 0; t <= 2 * M_PI; t += 2 * M_PI / density) {
            y = 0.2 + btn / 2.0 + sin(t) / (4.0 + btn);
            z = -0.5 + cos(t) / (4.0 + btn);
            nx = 0;
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
        it++;
    }
    for (double rad = 1.0 / (4.0 + btn); rad > 0.0; rad -= 1.0 / density) {
        res.push_back(std::vector<double>());
        for (double t = 0; t <= 2 * M_PI; t += 2 * M_PI / density) {
            z = bottomZ;
            y = 0.2 + btn / 2.0 + sin(t) * rad;
            z = -0.5 + cos(t) * rad;
            nx = 1;
            ny = 0;
            nz = 0;
            res.at(it).push_back(x);
            res.at(it).push_back(y);
            res.at(it).push_back(z);
            res.at(it).push_back(nx);
            res.at(it).push_back(ny);
            res.at(it).push_back(nz);
            res.at(it).push_back(tx);
            res.at(it).push_back(ty);
        }
        it++;
    }
    return res;

}
