#include "force.h"

double* force_halo_nfw(const double x, const double y, const double z)
{

  r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
  acc = -((phi_0 * r_s) / r) * (((log(1 + r / r_s) / r)) - 1 / (r_s + r));

  acc_array_halo[0] = acc * (x / r);
  acc_array_halo[1] = acc * (y / r);
  acc_array_halo[2] = acc * (z / r);

  return acc_array_halo;
}

double* force_disk(const double x, const double y, const double z)
{

  r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

  sqd = sqrt(pow(z, 2) + pow(b, 2));
  sqden = sqrt(pow(x, 2) + pow(y, 2) + pow(a + sqd, 2));

  sqd1 = pow(sqden, -3);
  acc = -GMd * sqd1;

  acc_array_disk[0] = acc * x;
  acc_array_disk[1] = acc * y;
  acc_array_disk[2] = acc * z * (a / sqd + 1);

  return acc_array_disk;
}

double* force_bulge(const double x, const double y, const double z)
{

    r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    acc = -GMb / (pow(r + c, 2));

    acc_array_bulge[0] = acc * (x / r);
    acc_array_bulge[1] = acc * (y / r);
    acc_array_bulge[2] = acc * (z / r);

    return acc_array_bulge;
}

double* force_total(const double x, const double y, const double z)
{

  acc_bulge = force_bulge(x, y, z);
  acc_disk = force_disk(x, y, z);
  acc_halo = force_halo_nfw(x, y, z);

  acc_total[0] = acc_bulge[0] + acc_disk[0] + acc_halo[0];
  acc_total[1] = acc_bulge[1] + acc_disk[1] + acc_halo[1];
  acc_total[2] = acc_bulge[2] + acc_disk[2] + acc_halo[2];

  return acc_total;
}

/* Debug stuff */
double* force_sun(const double x, const double y, const double z)
{

  float M_sun = 1.989E30;
  float G_sun = 6.62E-11;

  r = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
  acc = -(G_sun * M_sun) / pow(r, 2);

  acc_sun[0] = acc * (x / r);
  acc_sun[1] = acc * (y / r);
  acc_sun[2] = acc * (z / r);

  return acc_sun;
}

// Update the gravitational interaction between the centauri system.
// Can improve this function by moving some stuff around.
std::tuple<double*, double*> update_centuari_system(std::shared_ptr<std::vector<Sol_object>> centauri, std::shared_ptr<Parameters> param)
{

  std::vector<int> indx = param->get_centuari_indx();

  Sol_object obj_1 = (*centauri)[indx[0]];
  Sol_object obj_2 = (*centauri)[indx[1]];

  double* x1 = obj_1.get_positions();
  double* x2 = obj_2.get_positions();

  double* v1 = obj_1.get_velocities();
  double* v2 = obj_2.get_velocities();

  // Distance between the objects
  double r = sqrt(pow((x1[0] - x2[0]), 2) + pow((x1[1] - x2[1]), 2) + pow((x1[2] - x2[2]), 2));

  // To Cartesian
  double* acc_vec_1 = calc_acc(M_proxima, x1, r);
  double* acc_vec_2 = calc_acc(M_alpha, x2, r);

  return std::make_tuple(acc_vec_1, acc_vec_2);
}

double* calc_acc(const double M, const double* x, const double r)
{

  double acc_vec[3];
  double acc = (G * M) / pow(r, 2);

  acc_vec[0] = acc * (x[0] / r);
  acc_vec[1] = acc * (x[1] / r);
  acc_vec[2] = acc * (x[2] / r);

  return acc_vec;
}
