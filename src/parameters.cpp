#include "core.h"
#include "parameters.h"

Parameters::Parameters(int world_size, int world_rank)
{

  /* Set MPI properties */
  this->world_size = world_size;
  this->world_rank = world_rank;
  /* The file names are hardcoded for now, maybe change later */
  /* Load the parameter file */
  load_parameter_file("default_param");

  /* Load the file with targets */
  load_target_file("targets");

  /* Load the snapshot times */
  load_snapshot_file("snapshots");

  /* Should there be gas objects */
  if(should_output_dust_gas()) {gas_obj_list = std::make_shared<std::vector<Gas_object>>();}
}

void Parameters::load_parameter_file(std::string param_name)
{

  std::string output;

  std::string input_name_par = "parameter_files/";
  input_name_par.append(param_name);

  std::ifstream in_stream;
  in_stream.open(input_name_par.c_str());

  while(!in_stream.eof())
  {

    /* Get the line */
    std::getline(in_stream, output);
    if(output.empty()) {continue;}

    /* Parse the line */
    std::vector<std::string> line_elements;
    boost::split(line_elements, output, boost::is_any_of(" "));

    /* Skip comments */
    if(line_elements[0] == "#") {continue;}

    /* ============================================== */
    /* Parse the parameters correctly */
    if(line_elements[0] == "IC_data")
    {
      input_data = line_elements[line_elements.size() - 1];
    }
    else if(line_elements[0] == "total_time")
    {
      total_time = std::stod(line_elements[line_elements.size() - 1]);
    }
    else if(line_elements[0] == "dt")
    {
      dt = std::stod(line_elements[line_elements.size() - 1]);
    }
    else if(line_elements[0] == "repack")
    {
      if(line_elements[line_elements.size() - 1] == "True") {repack = true;}
      else if(line_elements[line_elements.size() - 1] == "False") {repack = false;}
    }
    else if(line_elements[0] == "output_dust_gas")
    {
      if(line_elements[line_elements.size() - 1] == "True") {output_dust_gas = true;}
      else if(line_elements[line_elements.size() - 1] == "False") {output_dust_gas = false;}
    }
    else if(line_elements[0] == "r_cut")
    {
      r_cut = std::stod(line_elements[line_elements.size() - 1]);
    }
    else
    {
      std::cout << "ERROR: failed to recognize parameter " << line_elements[0] << ". \n";
    }
    /* ============================================== */
  }

  in_stream.close();
}

void Parameters::load_target_file(std::string target_name)
{

  std::string output;

  std::string input_name_par = "parameter_files/";
  input_name_par.append(target_name);

  std::ifstream in_stream;
  in_stream.open(input_name_par.c_str());

  while(!in_stream.eof())
  {

    /* Get the line */
    std::getline(in_stream, output);
    if(!output.empty()) {targets.push_back(output);}
  }

  in_stream.close();
}

void Parameters::load_snapshot_file(std::string snap_name)
{

  std::string output;

  std::string input_name_par = "parameter_files/";
  input_name_par.append(snap_name);

  std::ifstream in_stream;
  in_stream.open(input_name_par.c_str());

  while(!in_stream.eof())
  {

    /* Get the line */
    std::getline(in_stream, output);
    if(!output.empty()) {snap_times.push_back(std::stod(output));}
  }

  in_stream.close();
}

/* Member functions */
double Parameters::get_total_time()
{
  return total_time;
}

double Parameters::get_dt()
{
  return dt;
}

uint Parameters::get_snapshots_num()
{
  return snap_times.size();
}

int Parameters::get_world_rank()
{
  return world_rank;
}

int Parameters::get_world_size()
{
  return world_size;
}

double Parameters::get_r_cut()
{
  return r_cut;
}

std::string Parameters::get_input_data()
{
  return input_data;
}

std::vector<std::string> Parameters::get_targets()
{
  return targets;
}

void Parameters::set_target_indx(int indx)
{
  target_indx_list.push_back(indx);
}

std::vector<int> Parameters::get_target_indx_list()
{
  return target_indx_list;
}

double Parameters::get_snap_time(int s)
{
  return snap_times[s];
}

int Parameters::get_num_objects()
{
  return num_objects;
}

void Parameters::set_num_objects(int num_objects)
{
  this->num_objects = num_objects;
}

bool Parameters::should_repack()
{
  return repack;
}

bool Parameters::should_output_dust_gas()
{
  return output_dust_gas;
}

void Parameters::add_gas_object(Gas_object g_obj)
{
  gas_obj_list->push_back(g_obj);
}

std::shared_ptr<std::vector<Gas_object>> Parameters::get_gas_objects()
{
  return gas_obj_list;
}
