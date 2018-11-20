import matplotlib.pyplot as plt

import pandas as pd
import numpy as np

class Int_obj:

	def __init__(self, oid):
		
		self.oid = oid

		self.s_list = []
		self.t_list = []
		self.r_list = []

		self.x_list = []
		self.y_list = []
		self.z_list = []

	def add_snap(self, s, t, x, y, z):
		
		self.s_list.append(s)
		self.t_list.append(t)

		self.x_list.append(x)
		self.y_list.append(y)
		self.z_list.append(z)

	def add_dist(self, r):
		self.r_list.append(r)

	def get_oid(self):
		return self.oid

	def get_snap_num(self):
		return self.s_list

	def get_time(self):
		return self.t_list

	def get_distance(self):
		return self.r_list

	def get_positions(self):
		return [self.x_list, self.y_list, self.z_list]

# Only reads the repack version
target_id = "Voyager1"
snap_name = "snapshots/all_snapshots_" + target_id

data_in = pd.read_csv(snap_name, delimiter=" ", header=None)

# Extract the data
oid = data_in[1].values

s = data_in[0].values
t = data_in[2].values

x = data_in[3].values
y = data_in[4].values
z = data_in[5].values

# Construct objects
obj_list = []

for i in range(len(oid)):
	
	# First object, otherwise cannot loop through list
	if(i == 0): 

		# Create object and add snap
		temp_obj = Int_obj(oid[i])
		temp_obj.add_snap(s[i], t[i], x[i], y[i], z[i])

		obj_list.append(temp_obj)

	else:
	
		obj_exists = False
		
		# Check if the object already exists
		for j in range(len(obj_list)):
			if(obj_list[j].get_oid() == oid[i]): 
				
				obj_list[j].add_snap(s[i], t[i], x[i], y[i], z[i])
				obj_exists = True
	
		if(obj_exists == False):
			
			# Create object and add snap
			temp_obj = Int_obj(oid[i])
			temp_obj.add_snap(s[i], t[i], x[i], y[i], z[i])

			obj_list.append(temp_obj)

# Find the target index
target_indx = 0

for i in range(len(obj_list)):
	if(obj_list[i].get_oid() == target_id): target_indx = i

# Get the positions from the target index
t_pos = np.transpose(obj_list[target_indx].get_positions())

# Calculate the distances between target and object
for i in range(len(obj_list)):
	
	if(i == target_indx): continue

	temp_obj = obj_list[i]
	
	in_snaps = temp_obj.get_snap_num()
	o_pos = np.transpose(temp_obj.get_positions())
	
	print(temp_obj.get_oid())

	for j, s in enumerate(in_snaps):
		
		x = t_pos[s][0] - o_pos[j][0]
		y = t_pos[s][1] - o_pos[j][1]
		z = t_pos[s][2] - o_pos[j][2]

		r = np.sqrt(x**2 + y**2 + z**2)
		temp_obj.add_dist(r)

# Plot everything
fig = plt.figure(figsize=(15, 8))
closest_approach = []

for i in range(len(obj_list)):
	
	if(i == target_indx): continue
	
	t = np.asarray(obj_list[i].get_time()) * 1E9
	r = np.asarray(obj_list[i].get_distance()) * 1000
	
	r_min = np.argmin(r)
	closest_approach.append([int(t[r_min]), r[r_min], obj_list[i].get_oid()])

	plt.plot(t, r, c="b")
	plt.text(t[r_min], r[r_min], str(obj_list[i].get_oid()))

plt.title(target_id + " close encounters")
plt.ylim(0, 2)

plt.xlabel("t (yr)")
plt.ylabel("r (pc)")

# Closest approaches
app_sort = np.argsort(np.transpose(closest_approach), 1)[1]
save_close = open("/home/bas/Documents/python_integrator/data/close_encounters.dat", "w")

for i in range(len(app_sort)):
	save_close.write(closest_approach[app_sort[i]][2] + "\n")
	print(closest_approach[app_sort[i]])

	if(i > 20):break

save_close.close()

plt.tight_layout()
plt.show()



