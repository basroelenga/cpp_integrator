#include "dust_gas.h"

double disk_dustorH2(const double R, const double z, const double rho, const double h, const double ze)
{

  return rho * exp(-R / h - std::abs(z) / ze);
}

double disk_HI(const double R, const double z, const double rho, const double h, const double ze)
{
  if(R > Rt)
  {
    return rho * exp(-R / h - std::abs(z) / ze);
  }
  else
  {
    return 0;
  }
}

double stars(const double R, const double z, const double etadisk, const double h, const double ze, const double etabulge, const  double ab, const double Re)
{

  double B = sqrt(pow(R, 2) + pow(z, 2) * pow(ab, 2)) / Re;
  return etadisk * exp(-R / h - std::abs(z) / ze) + etabulge * exp(-7.67 * pow(B, 0.25)) * pow(B, (-7/8));
}

std::tuple<double, double, double, double> dust_gas(const double x, const double y, const double z)
{

  double R = sqrt(pow(x, 2) + pow(y, 2));
  double rho_dust = disk_dustorH2(R, z, d_d1, h_d1, ze_d1) + disk_dustorH2(R, z, d_d2, h_d2, ze_d2);
  double rho_gas_H2 = disk_dustorH2(R, z, d_g1, h_g1, ze_g1);
  double rho_gas_HI = disk_HI(R, z, d_g2, h_g2, ze_g2);
  double em_stars = stars(R, z, etadisk, h_stars, ze_stars, etabulge, ab, Re);

  return std::make_tuple(rho_dust, rho_gas_H2, rho_gas_HI, em_stars);
}
