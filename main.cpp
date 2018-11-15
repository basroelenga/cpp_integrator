#include <iostream>
#include <mpi.h>

#include "parser.h"
#include "integrator.h"
#include "parameters.h"

int main(int argc, char* argv[])
{

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  int world_size;
  int world_rank;

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  /* Load the parameter file on every processor */
  std::shared_ptr<Parameters> param(new Parameters(world_size, world_rank));

  /* Load the data on every processor */
  std::tuple<std::shared_ptr<std::vector<Sol_object>>, std::shared_ptr<std::vector<Sol_object>>> obj_data = parse_file(param);

  /* Get the data for the current processor */
  std::shared_ptr<std::vector<Sol_object>> object_data = get_object_data(std::get<0>(obj_data), std::get<1>(obj_data), param);

  /* Wait until the loading is done on all processors */
  MPI_Barrier(MPI_COMM_WORLD);

  /* Start the integration */
  integrate(object_data, param);

  /* Exit */
  MPI_Finalize();
  return 0;
}
