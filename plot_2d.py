import time
import matplotlib.pyplot as plt
import numpy as np
import sys
from sklearn.cluster import AgglomerativeClustering
from sklearn.neighbors import kneighbors_graph
import parse
"""
n_samples = 1500
np.random.seed(0)
t = 1478022128 * np.random.rand(1,n_samples)
x = t
y = 1478022128 * np.random.rand(1,n_samples)
X = np.concatenate((x, y))
X = X.T
"""

snapshot_path = "anon-lnfs-fs4.txt"
file_dict = parse.parser(snapshot_path)
n_samples = len(file_dict)
x = []
y = []
z = []
for fileid ,filedata in file_dict.items():
    x.append(filedata['creation_time'])
    y.append(filedata['modification_time'])
    z.append(filedata['user_id'])
X = np.concatenate(([x], [y],[z]))
X = X.T

knn_graph = kneighbors_graph(X, 50, include_self=False)

linkage = 'ward'
n_clusters = 4
st = time.time()
model = AgglomerativeClustering(n_clusters = n_clusters, linkage=linkage).fit(X)
elapsed_time = time.time() - st
label = model.labels_

t0 = time.time()
model.fit(X)
elapsed_time = time.time() - t0
plt.scatter(X[:, 0], X[:, 1], c=model.labels_,
            cmap=plt.cm.spectral)
plt.title('%s' % (linkage),
          fontdict=dict(verticalalignment='top'))
plt.axis('equal')
plt.axis('on')

plt.xlabel('creation',fontsize = 16)
plt.ylabel('modification',fontsize = 16 )
#plt.zlabel('uid')

plt.subplots_adjust(bottom=0, top=0.9, wspace=0,
                    left=0, right=1)
plt.suptitle('num_cluster: %i' % (n_clusters))
plt.show()
"""
for connectivity in (None, knn_graph):
    for n_clusters in (10, 30):
        plt.figure(figsize=(10, 4))
        for index, linkage in enumerate(('average', 'complete', 'ward')):
            plt.subplot(1, 3, index + 1)
            model = AgglomerativeClustering(linkage=linkage,
                                            connectivity=connectivity,
                                            n_clusters=n_clusters)
            t0 = time.time()
            model.fit(X)
            elapsed_time = time.time() - t0
            plt.scatter(X[:, 0], X[:, 1], c=model.labels_,
                        cmap=plt.cm.spectral)
            plt.title('%s' % (linkage),
                      fontdict=dict(verticalalignment='top'))
            plt.axis('equal')
            plt.axis('off')

            plt.subplots_adjust(bottom=0, top=.89, wspace=0,
                                left=0, right=1)
            plt.suptitle('num_cluster: %i, connectivity=%r' %
                         (n_clusters, connectivity is not None))

plt.show()
"""
