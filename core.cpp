#include "core.h"

/* ===========================================================================*/
/* Constructor */
Sol_object::Sol_object(std::string name, double x, double y, double z, double vx, double vy, double vz)
{

  name_sol = name;

  pos_array[0] = x;
  pos_array[1] = y;
  pos_array[2] = z;

  vel_array[0] = vx;
  vel_array[1] = vy;
  vel_array[2] = vz;

  sim_time = 0;
}

/* Member functions */
double* Sol_object::get_positions()
{
  return pos_array;
}

void Sol_object::set_positions(double pos_in_array[3])
{

  pos_array[0] = pos_in_array[0];
  pos_array[1] = pos_in_array[1];
  pos_array[2] = pos_in_array[2];
}

double* Sol_object::get_velocities()
{
  return vel_array;
}

void Sol_object::set_velocities(double vel_in_array[3])
{

  vel_array[0] = vel_in_array[0];
  vel_array[1] = vel_in_array[1];
  vel_array[2] = vel_in_array[2];
}

void Sol_object::clear_accelerations()
{

  acc_array[0] = 0;
  acc_array[1] = 0;
  acc_array[2] = 0;
}

void Sol_object::add_accelerations(double acc_in_array[3])
{

  acc_array[0] += acc_in_array[0];
  acc_array[1] += acc_in_array[1];
  acc_array[2] += acc_in_array[2];
}

double* Sol_object::get_accelerations()
{
  return acc_array;
}

double Sol_object::get_time()
{
  return sim_time;
}

void Sol_object::set_time(double time_in)
{
  sim_time = time_in;
}

std::string Sol_object::get_name()
{
  return name_sol;
}

/* ===========================================================================*/
/* Constructor */
Gas_object::Gas_object(std::string name)
{

  this->name = name;

  /* Save the positions and time */
  t = std::make_shared<std::vector<double>>();

  x_pos = std::make_shared<std::vector<double>>();
  y_pos = std::make_shared<std::vector<double>>();
  z_pos = std::make_shared<std::vector<double>>();

  /* Save the gas values */
  rho_dust = std::make_shared<std::vector<double>>();
  rho_gas_H2 = std::make_shared<std::vector<double>>();
  rho_gas_HI = std::make_shared<std::vector<double>>();
  em_stars = std::make_shared<std::vector<double>>();
}

void Gas_object::add_gas(const double ti, const double x, const double y, const double z)
{

  t->push_back(ti);

  /* Add the positions */
  x_pos->push_back(x);
  y_pos->push_back(y);
  z_pos->push_back(z);

  std::tuple<double, double, double, double> gas_values = dust_gas(x, y, z);

  rho_dust->push_back(std::get<0>(gas_values));
  rho_gas_H2->push_back(std::get<1>(gas_values));
  rho_gas_HI->push_back(std::get<2>(gas_values));
  em_stars->push_back(std::get<3>(gas_values));
}

std::shared_ptr<std::vector<double>> Gas_object::get_t()
{
  return t;
}

std::shared_ptr<std::vector<double>> Gas_object::get_x()
{
  return x_pos;
}

std::shared_ptr<std::vector<double>> Gas_object::get_y()
{
  return y_pos;
}

std::shared_ptr<std::vector<double>> Gas_object::get_z()
{
  return z_pos;
}

std::shared_ptr<std::vector<double>> Gas_object::get_rho_dust()
{
  return rho_dust;
}

std::shared_ptr<std::vector<double>> Gas_object::get_rho_gas_H2()
{
  return rho_gas_H2;
}

std::shared_ptr<std::vector<double>> Gas_object::get_rho_gas_HI()
{
  return rho_gas_HI;
}

std::shared_ptr<std::vector<double>> Gas_object::get_em_stars()
{
  return em_stars;
}

std::string Gas_object::get_name()
{
  return name;
}
