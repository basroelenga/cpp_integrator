#include "snapshot.h"

/* Save a snapshot */
/* ================================================================================ */
std::chrono::duration<double> save_snapshot(std::shared_ptr<std::vector<Sol_object>> object_array, std::shared_ptr<Parameters> param, int snap_num)
{

  auto save_snap_start = std::chrono::system_clock::now();
  std::string snap_name = get_snap_name(snap_num);

  /* This is for the cutoff targets */
  std::vector<int> target_indx = param->get_target_indx_list();
  std::vector<std::string> target_names = param->get_targets();

  std::string directory = "snapshots/";

  /* Store local data */
  std::string name;

  double* pos_array;
  double* vel_array;

  double t;
  double r;

  /* Do the special case */
  if(param->get_r_cut() != 0)
  {

    /* Loop over each target */
    for(uint i = 0; i < target_names.size(); i++)
    {

      /* Create an output stream */
      std::string thread_name = "_" + target_names[i] + "_" + std::to_string(param->get_world_rank());
      std::string output_name = directory + snap_name + thread_name;

      /* Open a stream for this target */
      std::ofstream output_file;
      output_file.open(output_name.c_str());

      /* The output string, also set the precision of the output */
      std::stringstream out_stream;
      out_stream.precision(15);

      /* Obtain positions of the target */
      double* pos_cut = (*object_array)[target_indx[i]].get_positions();

      /* Save every particle to a snapshot */
      for(uint j = 0; j < object_array->size(); j++)
      {

        out_stream.str(std::string());
        name = (*object_array)[j].get_name();

        pos_array = (*object_array)[j].get_positions();
        vel_array = (*object_array)[j].get_velocities();

        t = (*object_array)[j].get_time();
        r = sqrt(pow(pos_array[0] - pos_cut[0], 2) + pow(pos_array[1] - pos_cut[1], 2) + pow(pos_array[2] - pos_cut[2], 2));

        /* Continue if larger than cutoff radii */
        if(r > param->get_r_cut()) {continue;}

        /* Only save the targets on the first thread */
        int should_continue = 0;

        for(uint k = 0; k < target_indx.size(); k++) {if(target_indx[k] == j and param->get_world_rank() != 0) {should_continue = 1;}}
        if(should_continue == 1) {continue;}

        /* Output to file */
        out_stream << name << " ";
        out_stream << std::setprecision(9) << t << " ";
        out_stream << std::setprecision(15) << pos_array[0] << " " << pos_array[1] << " " << pos_array[2] << " ";
        out_stream << std::setprecision(15) << vel_array[0] << " " << vel_array[1] << " " << vel_array[2] << "\n";

        output_file << out_stream.str();
      }

      thread_name.clear();
      output_file.close();
    }
  }
  /* This happens when integration normally */
  else
  {

    std::string thread_name = "_" + std::to_string(param->get_world_rank());
    std::string output_name = directory + snap_name + thread_name;

    /* The stream */
    std::ofstream output_file;
    output_file.open(output_name.c_str());

    /* The output string, also set the precision of the output */
    std::stringstream out_stream;
    out_stream.precision(15);

    for(uint i = 0; i < object_array->size(); i++)
    {

      out_stream.str(std::string());
      name = (*object_array)[i].get_name();

      pos_array = (*object_array)[i].get_positions();
      vel_array = (*object_array)[i].get_velocities();

      t = (*object_array)[i].get_time();

      /* Only save the targets on the first thread */
      int should_continue = 0;

      for(uint j = 0; j < target_indx.size(); j++) {if(target_indx[j] == i and param->get_world_rank() != 0) {should_continue = 1;}}
      if(should_continue == 1) {continue;}

      /* Output to file */
      out_stream << name << " ";
      out_stream << std::setprecision(9) << t << " ";
      out_stream << std::setprecision(15) << pos_array[0] << " " << pos_array[1] << " " << pos_array[2] << " ";
      out_stream << std::setprecision(15) << vel_array[0] << " " << vel_array[1] << " " << vel_array[2] << "\n";

      output_file << out_stream.str();
    }

    output_file.close();
  }

  /* Now that the snapshots are saved, combine them */
  MPI_Barrier(MPI_COMM_WORLD);

  /* Combine on the root */
  if(param->get_world_rank() == 0)
  {

    if(param->get_r_cut() != 0)
    {

      /* Loop over the targets to combine them */
      for(uint i = 0; i < target_names.size(); i++)
      {

        /* Create an output stream */
        std::string target_name = "_" + target_names[i];
        std::string output_name = directory + snap_name + target_name;

        /* The stream */
        std::ofstream output_file;
        output_file.open(output_name.c_str());

        /* The output string, also set the precision of the output */
        std::stringstream out_stream;
        out_stream.precision(15);

        /* Loop over all processors */
        for(uint j = 0; j < param->get_world_size(); j++)
        {

          /* Clear the string stream */
          out_stream.str("");

          /* Create an input stream */
          std::string input_name_obj = "snapshots/";
          std::string input_name = input_name_obj + snap_name + target_name + "_" + std::to_string(j);

          std::ifstream in_stream;
          in_stream.open(input_name.c_str());

          /* Output storage */
          std::string output;

          /* Load every object */
          while(!in_stream.eof())
          {

            /* Get the line */
            std::getline(in_stream, output);
            if(output.empty()) {break;}

            /* Write the line to the output file */
            out_stream << output << "\n";
          }

          output_file << out_stream.str();
          in_stream.close();

          /* Delete the file */
          remove(input_name.c_str());
        }

        output_file.close();
      }
    }
    else
    {

      /* Create the output file */
      std::string output_name = directory + snap_name;

      /* The stream */
      std::ofstream output_file;
      output_file.open(output_name.c_str());

      /* The output string, also set the precision of the output */
      std::stringstream out_stream;
      out_stream.precision(15);

      for(int i = 0; i < param->get_world_size(); i++)
      {

        /* Clear the string stream */
        out_stream.str("");

        /* Create an input stream */
        std::string input_name_obj = "snapshots/";
        std::string input_name = input_name_obj + snap_name + "_" + std::to_string(i);

        std::ifstream in_stream;
        in_stream.open(input_name.c_str());

        /* Output storage */
        std::string output;

        /* Load every object */
        while(!in_stream.eof())
        {

          /* Get the line */
          std::getline(in_stream, output);
          if(output.empty()) {break;}

          /* Write the line to the output file */
          out_stream << output << "\n";
        }

        output_file << out_stream.str();
        in_stream.close();

        /* Delete the file */
        remove(input_name.c_str());
      }

      output_file.close();
    }
  }

  /* Make the other processors wait for the combining */
  MPI_Barrier(MPI_COMM_WORLD);

  /* Some timing stuff */
  auto save_snap_end = std::chrono::system_clock::now();
  std::chrono::duration<double> io_time = save_snap_end - save_snap_start;

  return io_time;
}

void repack_snapshots(std::shared_ptr<Parameters> param)
{

  std::cout << "\n" << "Repacking snapshots. \n";
  std::string directory = "snapshots/";

  if(param->get_r_cut() != 0)
  {

    std::vector<std::string> target_names = param->get_targets();

    /* Loop over every target */
    for(uint i = 0; i < target_names.size(); i++)
    {

      /* Create the output stream */
      std::string output_name = directory + "all_snapshots_" + target_names[i];

      /* The stream */
      std::ofstream output_file;
      output_file.open(output_name.c_str());

      /* The output string, also set the precision of the output */
      std::stringstream out_stream;
      out_stream.precision(15);

      /* Loop over all target snapshots */
      for(int j = 0; j < param->get_snapshots_num(); j++)
      {

        /* Reconstruct the input name */
        std::string input_name = directory + get_snap_name(j) + "_" + target_names[i];

        /* Clear the string stream */
        out_stream.str("");

        /* Open the input stream */
        std::ifstream in_stream;
        in_stream.open(input_name.c_str());

        /* Output storage */
        std::string output;

        /* Load every object */
        while(!in_stream.eof())
        {

          /* Get the line */
          std::getline(in_stream, output);
          if(output.empty()) {break;}

          /* Write the line to the output file */
          out_stream << j << " " << output << "\n";
        }

        output_file << out_stream.str();
        in_stream.close();

        /* Delete the file */
        remove(input_name.c_str());
      }

      output_file.close();
    }
  }
  else
  {

    /* Create the output stream */
    std::string output_name = directory + "all_snapshots";

    /* The stream */
    std::ofstream output_file;
    output_file.open(output_name.c_str());

    /* The output string, also set the precision of the output */
    std::stringstream out_stream;
    out_stream.precision(15);

    /* Load all the files in the snapshot folder */
    for(int i = 0; i < param->get_snapshots_num(); i++)
    {

      /* Reconstruct the input name */
      std::string input_name = directory + get_snap_name(i);

      /* Clear the string stream */
      out_stream.str("");

      /* Open the input stream */
      std::ifstream in_stream;
      in_stream.open(input_name.c_str());

      /* Output storage */
      std::string output;

      /* Load every object */
      while(!in_stream.eof())
      {

        /* Get the line */
        std::getline(in_stream, output);
        if(output.empty()) {break;}

        /* Write the line to the output file */
        out_stream << i << " " << output << "\n";
      }

      output_file << out_stream.str();
      in_stream.close();

      /* Delete the file */
      remove(input_name.c_str());
    }

    output_file.close();
  }
}

std::string get_snap_name(int snap_num)
{

  std::string snap_name = "snapshot";

  std::stringstream sstream;
  sstream << snap_num;

  /* Get the correct snapshot name */
  if(snap_num >= 0 and snap_num < 10)
  {

    snap_name.append("_0000");
    snap_name.append(sstream.str());
  }
  else if(snap_num >= 10 and snap_num < 100)
  {

    snap_name.append("_000");
    snap_name.append(sstream.str());
  }
  else if(snap_num >= 100 and snap_num < 1000)
  {

    snap_name.append("_00");
    snap_name.append(sstream.str());
  }
  else if(snap_num >= 1000 and snap_num < 10000)
  {

    snap_name.append("_0");
    snap_name.append(sstream.str());
  }
  else if(snap_num >= 10000 and snap_num < 100000)
  {

    snap_name.append("_");
    snap_name.append(sstream.str());
  }
  else
  {

    std::cout << "ERROR: Snapshot number out of range \n";
    exit(EXIT_FAILURE);
  }

  return snap_name;
}

void save_gast_dust(std::shared_ptr<Parameters> param)
{

  std::shared_ptr<std::vector<Gas_object>> g_obj_ptr = param->get_gas_objects();
  std::string directory = "snapshots/";

  for(uint i = 0; i < g_obj_ptr->size(); i++)
  {

    /* Dereference */
    Gas_object g_obj = (*g_obj_ptr)[i];
    std::string output_name = directory + "gas_dust_" + g_obj.get_name();

    /* The stream */
    std::ofstream output_file;
    output_file.open(output_name.c_str());

    /* The output string, also set the precision of the output */
    std::stringstream out_stream;
    out_stream.precision(15);

    /* Get the positions and gas stuff */
    std::shared_ptr<std::vector<double>> t = g_obj.get_t();

    std::shared_ptr<std::vector<double>> x = g_obj.get_x();
    std::shared_ptr<std::vector<double>> y = g_obj.get_y();
    std::shared_ptr<std::vector<double>> z = g_obj.get_z();

    std::shared_ptr<std::vector<double>> rho_dust = g_obj.get_rho_dust();
    std::shared_ptr<std::vector<double>> rho_gas_H2 = g_obj.get_rho_gas_H2();
    std::shared_ptr<std::vector<double>> rho_gas_HI = g_obj.get_rho_gas_HI();
    std::shared_ptr<std::vector<double>> em_stars = g_obj.get_em_stars();

    for(uint j = 0; j < t->size(); j++)
    {
      out_stream << std::setprecision(9) << (*t)[j] << " ";
      out_stream << std::setprecision(15) << (*x)[j] << " " << (*y)[j] << " " << (*z)[j] << " " << (*rho_dust)[j] << " " << (*rho_gas_H2)[j] << " " << (*rho_gas_HI)[j] << " " << (*em_stars)[j] << " \n";
    }

    output_file << out_stream.str();
    output_file.close();
  }
}
