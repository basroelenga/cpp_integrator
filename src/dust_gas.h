#ifndef DUST_GAS_H
#define DUST_GAS_H

#include <iostream>
#include <tuple>
#include <cmath>

/* Parameters */
/* Warm dust */
const double d_d1 = 1.22E-27;
const double h_d1 = 3.3;
const double ze_d1 = 0.09;

/* Cold dust */
const double d_d2 = 1.51E-25;
const double h_d2 = 5.0;
const double ze_d2 = 0.1;

/* H2 gas */
const double d_g1 = 4.06;
const double h_g1 = 2.57;
const double ze_g1 = 0.08;

/* HI gas */
const double d_g2 = 0.32;
const double h_g2 = 18.24;
const double ze_g2 = 0.52;

/* Stars */
const double etadisk = 5.49E-38;
const double h_stars = 2.5;
const double ze_stars = 0.16;
const double etabulge = 2.07E-34;

const double Re = 0.68;
const double ab = 0.61;
const double Rt = 2.75;

/* Functions */
double disk_dustorH2(const double R, const double z, const double rho, const double h, const double ze);
double disk_HI(const double R, const double z, const double rho, const double h, const double ze);

double stars(const double R, const double z, const double etadisk, const double h, const double ze, const double etabulge, const  double ab, const double Re);
std::tuple<double, double, double, double> dust_gas(const double x, const double y, const double z);

#endif
