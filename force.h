#ifndef FORCE_H
#define FORCE_H

#include <iostream>
#include <vector>
#include <cmath>

/* Constants used in functions, maybe put in a file to change without compiling */
const double G = 4.49708E-06;

/* Bulge parameters */
const double GMb = 3.0E10 * G;

const double c = 0.7;

/* Disk parameters */
const double GMd = 9.3E10 * G;

const double a = 6.5;
const double b = 0.26;

/* NFW halo parameters */
const double M200 = 1E12;
const double r_s = 21.5;

const double Rvir = 258;
const double conc = Rvir / r_s;

const double phi_0 = G * (M200 / Rvir) * conc * pow(std::log(1 + conc) - conc / (1 + conc), -1);

/* Some temporary variables */
inline double acc_array_halo[3];
inline double acc_array_disk[3];
inline double acc_array_bulge[3];

inline double r;
inline double acc;

inline double sqd;
inline double sqden;
inline double sqd1;

inline double* acc_halo;
inline double* acc_disk;
inline double* acc_bulge;

inline double acc_total[3];

/* Force calculation methods */
double* force_halo_nfw(const double x, const double y, const double z);
double* force_bulge(const double x, const double y, const double z);
double* force_disk(const double x, const double y, const double z);

double* force_total(const double x, const double y, const double z);

/* Test for the sun force */
inline double acc_sun[3];
double* force_sun(const double x, const double y, const double z);

#endif
