import numpy
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import scipy.cluster.hierarchy as hcluster
import parse

x_axis = 'creation_time'
y_axis = 'modification_time'
z_data = 'file_id'

snapshot_path = "anon-lnfs-fs4.txt"
file_dict = parse.parser(snapshot_path)
n_samples = len(file_dict)

x = []
y = []
z = []
for file_id, file_data in file_dict.items():
    x.append(file_data[x_axis])
    y.append(file_data[y_axis])
    z.append(file_data[z_data])
data = numpy.concatenate(([x], [y]))
data = data.T
# clustering
thresh = 10000000
clusters = hcluster.fclusterdata(data, thresh, criterion="distance")

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x,y,z, c = clusters, s= 5, linewidth = 0)

title = "threshold: %f, number of clusters: %d" % (thresh, len(set(clusters)))
plt.title(title)
plt.show()