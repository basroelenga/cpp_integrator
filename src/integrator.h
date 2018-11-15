#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <vector>
#include <chrono>
#include <tuple>
#include <stdio.h>
#include <mpi.h>

#include "core.h"
#include "force.h"
#include "parser.h"
#include "snapshot.h"
#include "parameters.h"

/* Variable for local storage */
inline double* k1_v;
inline double* k2_v;
inline double* k3_v;
inline double* k4_v;

inline double k1_r[3];
inline double k2_r[3];
inline double k3_r[3];
inline double k4_r[3];

inline double v_new[3];
inline double r_new[3];

void integrate(std::shared_ptr<std::vector<Sol_object>> object_array, std::shared_ptr<Parameters> param);

std::tuple<double*, double*> rk4(const double x, const double y, const double z, const double vx, const double vy, const double vz, const double dt);
void add_gas(std::shared_ptr<std::vector<Sol_object>> object_array, std::shared_ptr<Parameters> param);

#endif
