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
