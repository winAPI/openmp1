from matplotlib import pyplot as plt
import matplotlib.patches as mpatches

# ms
#n1000 = [10978.08, 5115.15, 5124.29, 5124.52, ]
#n2000 = [103282.98, 50296.77, 50901.25, 49623.42, ]

n500 = [0.69, 0.59, 0.59, 0.60, ]
n700 = [3.86, 1.73, 1.72, 1.72, ]
n900 = [8.49, 3.71, 3.72, 3.71, ]
n1000 = [11.04, 5.14, 5.15, 5.15, ]
n1500 = [40.03, 18.95, 18.93, 18.94, ]
n2000 = [103.02, 50.26, 50.10, 53.24, ]


sizes_matrix = [500, 700, 900, 1000, 1500, 2000]

time_single_thread = [n500[0], n700[0], n900[0], n1000[0], n1500[0], n2000[0]]
time_parallel5     = [n500[1], n700[1], n900[1], n1000[1], n1500[1], n2000[1]]
time_parallel10    = [n500[2], n700[2], n900[2], n1000[2], n1500[2], n2000[2]]
time_parallel20    = [n500[3], n700[3], n900[3], n1000[3], n1500[3], n2000[3]]


#plt.semilogx(basex=2,subsx=sizes_matrix, nonposx='clip')
#plt.xscale('log') #plt.semilogx(sizes_matrix)
#plt.semilogx()
#plt.ticklabel_format(style='plain', useOffset=True, axis='x')

plt.legend(handles=[
    mpatches.Patch(color='r', label='Single thread'),
    mpatches.Patch(color='g', label='5 threads'),
    mpatches.Patch(color='b', label='10 threads'),
    mpatches.Patch(color='k', label='20 threads')
], loc=2,  borderaxespad=0.) #bbox_to_anchor=(1.05, 1),

plt.title('Benchmark (Single thread and parallel)')
plt.xlabel('Size matrix, N (NxN)')
plt.ylabel('Time computing, Seconds')

plt.plot(sizes_matrix, time_single_thread, 'ro-')
plt.plot(sizes_matrix, time_parallel5, 'go-')
plt.plot(sizes_matrix, time_parallel10, 'bo-')
plt.plot(sizes_matrix, time_parallel20, 'ko-')

for x, y in zip(sizes_matrix, time_single_thread):
    plt.annotate(s="({},{})".format(x, y), xy=(x, y), xytext=(x - 10, y + 4))

plt.axis([500, 2000, 0, 105])
plt.xticks(sizes_matrix)
plt.show()