import matplotlib.pyplot as plt
import numpy
import scipy.cluster.hierarchy as hcluster
import parse

x_axis = 'creation_time'
y_axis = 'modification_time'
z_data = 'user_id'

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

# plotting
for i in xrange(len(clusters)):
    clusters[i] *= clusters[i] * clusters[i] * 4
plt.scatter(*numpy.transpose(data), c=z, alpha=0.5, s=clusters, linewidths=0)
plt.xlabel(x_axis)
plt.ylabel(y_axis)

title = "threshold: %f, number of clusters: %d" % (thresh, len(set(clusters)))
plt.title(title)
plt.show()