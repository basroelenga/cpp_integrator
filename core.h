#ifndef CORE_H
#define CORE_H

#include <vector>
#include <iostream>

#include "dust_gas.h"
#include "parameters.h"

class Sol_object
{

  private:

    /* Data members */
    std::string name_sol;

    double pos_array[3];
    double vel_array[3];
    double acc_array[3];

    double sim_time;
    float mass;

  public:

    Sol_object(std::string name, double x, double y, double z, double vx, double vy, double vz);

    std::string get_name();

    double* get_positions();
    void set_positions(double pos_in_array[3]);

    double* get_velocities();
    void set_velocities(double vel_in_array[3]);

    void add_accelerations(double acc_in_array[3]);
    void clear_accelerations();

    double* get_accelerations();

    double get_time();
    void set_time(double time_in);
};

class Gas_object
{

  private:

    std::string name;

    std::shared_ptr<std::vector<double>> t;

    std::shared_ptr<std::vector<double>> x_pos;
    std::shared_ptr<std::vector<double>> y_pos;
    std::shared_ptr<std::vector<double>> z_pos;

    std::shared_ptr<std::vector<double>> rho_dust;
    std::shared_ptr<std::vector<double>> rho_gas_H2;
    std::shared_ptr<std::vector<double>> rho_gas_HI;
    std::shared_ptr<std::vector<double>> em_stars;

  public:

    Gas_object(std::string name);
    void add_gas(const double ti, const double x, const double y, const double z);

    std::shared_ptr<std::vector<double>> get_t();

    std::shared_ptr<std::vector<double>> get_x();
    std::shared_ptr<std::vector<double>> get_y();
    std::shared_ptr<std::vector<double>> get_z();

    std::shared_ptr<std::vector<double>> get_rho_dust();
    std::shared_ptr<std::vector<double>> get_rho_gas_H2();
    std::shared_ptr<std::vector<double>> get_rho_gas_HI();
    std::shared_ptr<std::vector<double>> get_em_stars();

    std::string get_name();
};

#endif
