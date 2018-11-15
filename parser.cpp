#include "parser.h"

/* Load the objects */
/* ================================================================================ */
std::tuple<std::shared_ptr<std::vector<Sol_object>>, std::shared_ptr<std::vector<Sol_object>>> parse_file(std::shared_ptr<Parameters> param)
{

  /* Create an input stream */
  std::string input_name_obj = "data/";
  input_name_obj.append(param->get_input_data());

  std::ifstream in_stream;
  in_stream.open(input_name_obj.c_str());

  /* Output storage */
  std::string output;

  /* Create two vectors, one for all objects and one for the target objects */
  std::shared_ptr<std::vector<Sol_object>> object_array(new std::vector<Sol_object>());
  std::shared_ptr<std::vector<Sol_object>> target_array(new std::vector<Sol_object>());

  /* Load every solar object */
  while(!in_stream.eof())
  {

    /* Get the line */
    std::getline(in_stream, output);
    if(output.empty()) {break;}

    /* Parse the line */
    std::vector<std::string> line_elements;
    boost::split(line_elements, output, boost::is_any_of(" "));

    /* Convert the string elements to floats where necessary */
    double x = std::stod(line_elements[1]);
    double y = std::stod(line_elements[2]);
    double z = std::stod(line_elements[3]);

    double vx = std::stod(line_elements[4]);
    double vy = std::stod(line_elements[5]);
    double vz = std::stod(line_elements[6]);

    /* Check for target */
    int is_target = 0;

    /* Check for a target */
    for(uint i = 0; i < param->get_targets().size(); i++)
    {
      if(line_elements[0] == param->get_targets()[i])
      {
        target_array->push_back(Sol_object(line_elements[0], x, y, z, vx, vy, vz));
        is_target = 1;
      }
    }

    /* Do not add the target to the list */
    if(is_target == 1) {continue;}

    /* Push the object in the list */
    object_array->push_back(Sol_object(line_elements[0], x, y, z, vx, vy, vz));
  }

  /* Information for the parameter file */
  param->set_num_objects(object_array->size());

  /* Close the stream */
  in_stream.close();

	/* Error check */
  if(object_array->size() == 0)
  {
    std::cout << "ERROR: object array is empty \n";
    exit(EXIT_FAILURE);
  }

  return std::make_tuple(object_array, target_array);
}

std::shared_ptr<std::vector<Sol_object>> get_object_data(std::shared_ptr<std::vector<Sol_object>> object_data_all, std::shared_ptr<std::vector<Sol_object>> target_data, std::shared_ptr<Parameters> param)
{

  int world_size = param->get_world_size();
  int world_rank = param->get_world_rank();

  /* Initialize the data lists */
  std::shared_ptr<std::vector<Sol_object>> object_data(new std::vector<Sol_object>());

  /* Get the object data required for the current processor */
  int start_indx;
  int end_indx;

  /* Special case for root */
  if(world_rank == 0) {start_indx = world_rank * (object_data_all->size() / world_size);}
  else {start_indx = world_rank * (object_data_all->size() / world_size) + 1;}

  if(world_rank == world_size - 1) {end_indx = (world_rank + 1) * (object_data_all->size() / world_size) - 1;}
  else {end_indx = (world_rank + 1) * (object_data_all->size() / world_size);}

  /* Get the objects and put them in the data array */
  for(int i = start_indx; i <= end_indx; i++)
  {
    object_data->push_back((*object_data_all)[i]);
  }

  /* Finally add the target data and create gas objects if needed */
  for(uint i = 0; i < target_data->size(); i++)
  {

    object_data->push_back((*target_data)[i]);
    param->set_target_indx(object_data->size() - 1);

    /* This will add the gas object */
    if(param->get_world_rank() == 0) {param->add_gas_object(Gas_object((*target_data)[i].get_name()));}
  }

  return object_data;
}
