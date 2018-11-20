import pandas as pd
import numpy as np

import os

#===================================================================================================================================================
# Program parameters
CORES = 1										# The amount of cores used in the integration
REPACK = False									# Pack all snapshots in 1 file (this should be on for a large number of snapshots and targets)
DUST_GAS = False								# Calculate dust and gas properties for the targets

# Simulation parameters (Gyr)
t = 1E-5										# Total integration time
dt = 1E-9										# The timestep

input_data = "voyager.dat"						# The input data
targets = ["Sun"]								# Targets for the cutoff and dust
r_cut = 0										# If r_cut is 0 every star will be saved, otherwise only the ones in the cut radius from the targets

snapshots = 5000								# The amount of snapshots that will be output
snap_start = 0									# The start time of output snapshots
snap_end = t									# The end time of outputting snapshots
# ===================================================================================================================================================

# Edit the parameter file of the integrator
param_in = open("parameter_files/default_param", "r")
param_data = param_in.read().split("\n")

new_lines = []

for line in param_data:

	line_split = line.split(" ")

	if(line_split[0] == "IC_data"):
		line_split[-1] = str(input_data) 

	if(line_split[0] == "total_time"):
		line_split[-1] = str(t)

	if(line_split[0] == "dt"):
		line_split[-1] = str(dt)

	if(line_split[0] == "repack"):
		line_split[-1] = str(REPACK) 

	if(line_split[0] == "dust_gas"):
		line_split[-1] = str(DUST_GAS) 

	if(line_split[0] == "r_cut"):
		line_split[-1] = str(r_cut)

	new_lines.append(" ".join(line_split))

param_in.close()

# Write the parameter file
param_out = open("parameter_files/default_param", "w")

for line in new_lines:
	param_out.write(line + "\n")

param_out.close()

# Write the target file
target_out = open("parameter_files/targets", "w")

for i in range(len(targets)):
	target_out.write(targets[i] + "\n")

target_out.close()

# Write the snapshot file
snap_out = open("parameter_files/snapshots", "w")
snap_array = np.linspace(snap_start, snap_end, snapshots + 1)

for i in range(len(snap_array)):
	snap_out.write(str(np.round(snap_array[i], 9)) + "\n")

snap_out.close()

# Load the data file to check how many values are in there
data_len = len(pd.read_csv("data/" + input_data, header=None))

# Remove all snapshots
os.system("rm snapshots/*")

# Start the program
os.system("mpirun -np " + str(CORES) + " integrator_mpi")
