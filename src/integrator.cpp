#include "integrator.h"

void integrate(std::shared_ptr<std::vector<Sol_object>> object_array, std::shared_ptr<Parameters> param)
{

  /* A start up message */
  if(param->get_world_rank() == 0)
  {
    std::cout << "================================================ \n";
    std::cout << "Integrator started on " << param->get_world_size() << " processors. \n";
    std::cout << "Total number of objects in integration: " << param->get_num_objects() << " \n";
  }

  /* Set the internal starting time */
  double t = 0;
  int s = 0;

  /* Get the start time */
  auto initial_time = std::chrono::system_clock::now();
  auto prev_time = std::chrono::system_clock::now();

  while(t <= (param->get_total_time()) + param->get_dt())
  {

    /* Check if a snapshot should be written, cant do explicit comparison */
    if(param->get_snap_time(s) > t - (param->get_dt() / 2) && param->get_snap_time(s) < t + (param->get_dt() / 2))
    {

      /* Some time stuff */
      auto now_time = std::chrono::system_clock::now();
      std::chrono::duration<double> diff_total = now_time - initial_time;
      std::chrono::duration<double> diff_snap = now_time - prev_time;

      /* Save the snapshot */
      std::chrono::duration<double> io_time = save_snapshot(object_array, param, s);
      prev_time = now_time;

      /* Output the gast dus at the current positions for the targets */
      if(param->get_world_rank() == 0 && param->should_output_dust_gas()) {add_gas(object_array, param);}

      /* Output some information */
      if(param->get_world_rank() == 0)
      {

        /*
        std::cout << "\n";
        std::cout << "Writing snapshot " << s + 1 << ". \n";
        std::cout << "Total time: " << diff_total.count() << " seconds. \n";
        std::cout << "Snapshot time: " << diff_snap.count() << " seconds. \n";
        std::cout << "Write time: " << io_time.count() << " seconds. \n";
        std::cout << param->get_snap_time(s) << " " << t << " and " << t - param->get_dt() << "\n";
        */

        progress(param->get_snapshots_num(), s, "Output");
      }
      s += 1;
    }

    /* Needs to be done here */
    t += param->get_dt();

    /* Update every object */
    for(uint i = 0; i < object_array->size(); i++)
    {

      double* x = (*object_array)[i].get_positions();
      double* v = (*object_array)[i].get_velocities();

      std::tuple<double*, double*> new_rv = rk4(x[0], x[1], x[2], v[0], v[1], v[2], param->get_dt());

      (*object_array)[i].set_positions(std::get<1>(new_rv));
      (*object_array)[i].set_velocities(std::get<0>(new_rv));

      (*object_array)[i].set_time(t);
    }
  }

  /* Repack the snapshots if requested */
  if(param->get_world_rank() == 0 && param->should_repack()) {repack_snapshots(param);}

  /* Save the gas dust if needed */
  if(param->get_world_rank() == 0 && param->should_output_dust_gas()) {save_gast_dust(param);}
}

/* Calculate the force */
std::tuple<double*, double*> rk4(const double x, const double y, const double z, const double vx, const double vy, const double vz, const double dt)
{

  /* Calculate the components, first component */
  k1_v = force_total(x, y, z);

  k1_r[0] = vx;
  k1_r[1] = vy;
  k1_r[2] = vz;

  /* Second component */
  k2_v = force_total(x + k1_r[0] * (dt / 2), y + k1_r[1] * (dt / 2), z + k1_r[2] * (dt / 2));

  k2_r[0] = vx + k1_v[0] * (dt / 2);
  k2_r[1] = vy + k1_v[1] * (dt / 2);
  k2_r[2] = vz + k1_v[2] * (dt / 2);

  /* Third component */
  k3_v = force_total(x + k2_r[0] * (dt / 2), y + k2_r[1] * (dt / 2), z + k2_r[2] * (dt / 2));

  k3_r[0] = vx + k2_v[0] * (dt / 2);
  k3_r[1] = vy + k2_v[1] * (dt / 2);
  k3_r[2] = vz + k2_v[2] * (dt / 2);

  /* Fourth component */
  k4_v = force_total(x + k3_r[0] * dt, y + k3_r[1] * dt, z + k3_r[2] * dt);

  k4_r[0] = vx + k3_v[0] * dt;
  k4_r[1] = vy + k3_v[1] * dt;
  k4_r[2] = vz + k3_v[2] * dt;

  /* Do the Runge Kutta */
  v_new[0] = vx + (k1_v[0] + 2 * k2_v[0] + 2 * k3_v[0] + k4_v[0]) * (dt / 6);
  v_new[1] = vy + (k1_v[1] + 2 * k2_v[1] + 2 * k3_v[1] + k4_v[1]) * (dt / 6);
  v_new[2] = vz + (k1_v[2] + 2 * k2_v[2] + 2 * k3_v[2] + k4_v[2]) * (dt / 6);

  r_new[0] = x + (k1_r[0] + 2 * k2_r[0] + 2 * k3_r[0] + k4_r[0]) * (dt / 6);
  r_new[1] = y + (k1_r[1] + 2 * k2_r[1] + 2 * k3_r[1] + k4_r[1]) * (dt / 6);
  r_new[2] = z + (k1_r[2] + 2 * k2_r[2] + 2 * k3_r[2] + k4_r[2]) * (dt / 6);

  return std::make_tuple(v_new, r_new);
}

void add_gas(std::shared_ptr<std::vector<Sol_object>> object_array, std::shared_ptr<Parameters> param)
{

  std::shared_ptr g_obj_ptr = param->get_gas_objects();

  for(uint i = 0; i < g_obj_ptr->size(); i++)
  {

    double t = (*object_array)[param->get_target_indx_list()[i]].get_time();
    double* x = (*object_array)[param->get_target_indx_list()[i]].get_positions();
    (*g_obj_ptr)[i].add_gas(t, x[0], x[1], x[2]);
  }
}

void progress(int total, int current, std::string output_message)
{

  // Hardcode bars for now
  int total_bars = 50;
  float p = (float)current / (float)total;

  int progress_bars = p * total_bars;
  std::string output_bars;

  for(int i = 0; i < progress_bars; i++)
  {
    output_bars += "=";
  }

  for(int i = 0; i < total_bars - progress_bars - 1; i++)
  {
    output_bars += " ";
  }

  std::cout << "\r" << "Integrating : [" << output_bars << "] (snapshots : " << current + 1<< "/" << total << ")" << std::flush;
}
