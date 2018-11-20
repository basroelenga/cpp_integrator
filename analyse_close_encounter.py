import matplotlib.pyplot as plt

import pandas as pd
import numpy as np

class Data_obj:

	def __init__(self, oid):

		self.oid = oid
		self.t = []

		self.x = []
		self.y = []
		self.z = []

	def get_oid(self):
		return self.oid

	def add_snap(self, t, x, y, z):

		self.t.append(t)

		self.x.append(x)
		self.y.append(y)
		self.z.append(z)

	def add_dist(self, r):
		self.r = r

	def get_dist(self):
		return self.r

	def get_time(self):
		return self.t

	def get_positions(self):
		return [self.x, self.y, self.z]

data = pd.read_csv("snapshots/all_snapshots", header=None, delimiter=" ")

oid = data[1].values
t = data[2].values
s = data[0].values

x = data[3].values
y = data[4].values
z = data[5].values

obj_list = []
c = 0
prev_s = 0
t_indx = 0

# Add all snapshots back
for i in range(len(oid)):
	
	if(s[i] != prev_s): 
		print(s[i])
		prev_s = s[i]
		c = 0

	# First object, otherwise cannot loop through list
	if(s[i] == 0): 

		# Create object and add snap
		temp_obj = Data_obj(oid[i])

		if(oid[i] == "Voyager1"): t_indx = i

		temp_obj.add_snap(t[i], x[i], y[i], z[i])
		obj_list.append(temp_obj)

	else:
	
		obj_list[c].add_snap(t[i], x[i], y[i], z[i])
		c += 1

xt_pos, yt_pos, zt_pos = obj_list[t_indx].get_positions()
t = np.asarray(obj_list[t_indx].get_time())

plt.figure()

r_min_list = []
t_min_list = []

# Calculate distance to target
for i in range(len(obj_list)):

	temp_obj = obj_list[i]
	if(temp_obj.get_oid() == "Voyager1" or temp_obj.get_oid() == "Voyager2" or temp_obj.get_oid() == "Sun"): continue

	x, y, z = temp_obj.get_positions()

	xs = np.asarray(xt_pos) - np.asarray(x)
	ys = np.asarray(yt_pos) - np.asarray(y)
	zs = np.asarray(zt_pos) - np.asarray(z)

	r = np.sqrt(xs**2 + ys**2 + zs**2)
	r_min_list.append(np.min(r) * 1E3)

	r_arg = np.argmin(r)
	t_min_list.append(t[r_arg] * 1E3)

	plt.plot(t * 1E3, r * 1E3, c="b", alpha=0.2)

plt.ylim(0, 2)

plt.xlabel("t (Myr)")
plt.ylabel("r (pc)")

plt.figure()
plt.hist(r_min_list, bins=40)

plt.xlabel("Minimum distance to Voyager1 (pc)")
plt.ylabel("Counts")

plt.title("1000 samples for Gaia DR2 " + str(oid[i]).split("_")[0])

plt.figure()
plt.hist(t_min_list, bins=40)

plt.xlabel("Encounter time with Voyager 1 (Myr)")
plt.ylabel("Counts")

plt.title("1000 samples for Gaia DR2 " + str(oid[i]).split("_")[0])


'''
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 7))

for i in range(len(obj_list)):

	x, y, z = obj_list[i].get_positions()

	if(obj_list[i].get_oid() == "Voyager1"):
		ax1.plot(x, y, c="r")
		ax2.plot(x, z, c="r")
	else:
		ax1.plot(x, y, c="b", alpha=0.1)
		ax2.plot(x, z, c="b", alpha=0.1)
'''
plt.show()
		





