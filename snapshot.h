#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <iomanip>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <mpi.h>

#include "core.h"
#include "parameters.h"

std::chrono::duration<double> save_snapshot(std::shared_ptr<std::vector<Sol_object>> object_array, std::shared_ptr<Parameters> param, int snap_num);
void save_gast_dust(std::shared_ptr<Parameters> param);

void repack_snapshots(std::shared_ptr<Parameters> param);
std::string get_snap_name(int snap_num);

#endif
