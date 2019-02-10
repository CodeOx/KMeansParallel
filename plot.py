import matplotlib.pyplot as plt
import numpy as np
from matplotlib.pyplot import figure

kl = [4,6,8]
th = [2,4,8,16]

with open("log_pthread.txt") as f:
    content = f.readlines()
# you may also want to remove whitespace characters like `\n` at the end of each line
content = [x.strip() for x in content]
data = [c.split(":") for c in content]

# for k in kl:
# 	x_seq = [(int(d[1]),float(d[5])) for d in data if d[0] == '1' and int(d[2]) == k]
# 	x = [a[0] for a in x_seq]
# 	y = [a[1] for a in x_seq]
# 	order = np.argsort(x)
# 	xs = np.array(x)[order]
# 	ys = np.array(y)[order]
# 	figure(num=None, figsize=(10, 6), dpi=80, facecolor='w', edgecolor='k')
# 	plt.plot(xs, ys)
# 	for t in th:
# 	 	x_par = [(int(d[1]),float(d[5])) for d in data if int(d[0]) == t and int(d[2]) == k]
# 	 	x = [a[0] for a in x_par]
# 		y = [a[1] for a in x_par]
# 		order = np.argsort(x)
# 		xs = np.array(x)[order]
# 		ys = np.array(y)[order]
# 		plt.plot(xs, ys)
# 	plt.title('Sequential vs pthreads : k=' + str(k))
# 	plt.xlabel('N')
# 	plt.ylabel('time')
# 	plt.legend(['sequential', '2 threads', '4 threads', '8 threads', '16 threads'], loc='upper left')
# 	plt.savefig('plots/seq_vs_pth_k_' + str(k), bbox_inches='tight')
# 	plt.show()

with open("log_omp.txt") as f:
    content = f.readlines()
# you may also want to remove whitespace characters like `\n` at the end of each line
content = [x.strip() for x in content]
data1 = [c.split(":") for c in content]

# for k in kl:
# 	x_seq = [(int(d[1]),float(d[5])) for d in data1 if d[0] == '1' and int(d[2]) == k]
# 	x = [a[0] for a in x_seq]
# 	y = [a[1] for a in x_seq]
# 	order = np.argsort(x)
# 	xs = np.array(x)[order]
# 	ys = np.array(y)[order]
# 	figure(num=None, figsize=(10, 6), dpi=80, facecolor='w', edgecolor='k')
# 	plt.plot(xs, ys)
# 	for t in th:
# 	 	x_par = [(int(d[1]),float(d[5])) for d in data1 if int(d[0]) == t and int(d[2]) == k]
# 	 	x = [a[0] for a in x_par]
# 		y = [a[1] for a in x_par]
# 		order = np.argsort(x)
# 		xs = np.array(x)[order]
# 		ys = np.array(y)[order]
# 		plt.plot(xs, ys)
# 	plt.title('Sequential vs OpenMP : k=' + str(k))
# 	plt.xlabel('N')
# 	plt.ylabel('time')
# 	plt.legend(['sequential', '2 threads', '4 threads', '8 threads', '16 threads'], loc='upper left')
# 	plt.savefig('plots/seq_vs_omp_k_' + str(k), bbox_inches='tight')
# 	plt.show()

# for t in th:
# 	x_seq = [(int(d[1]),float(d[5])) for d in data1 if d[0] == '1' and int(d[2]) == 8]
# 	x = [a[0] for a in x_seq]
# 	y = [a[1] for a in x_seq]
# 	order = np.argsort(x)
# 	xs = np.array(x)[order]
# 	ys = np.array(y)[order]
# 	figure(num=None, figsize=(10, 6), dpi=80, facecolor='w', edgecolor='k')
# 	plt.plot(xs, ys)
# 	#pthread
#  	x_par = [(int(d[1]),float(d[5])) for d in data if int(d[0]) == t and int(d[2]) == 8]
#  	x = [a[0] for a in x_par]
# 	y = [a[1] for a in x_par]
# 	order = np.argsort(x)
# 	xs = np.array(x)[order]
# 	ys = np.array(y)[order]
# 	plt.plot(xs, ys)
# 	#openmp
#  	x_par = [(int(d[1]),float(d[5])) for d in data1 if int(d[0]) == t and int(d[2]) == 8]
#  	x = [a[0] for a in x_par]
# 	y = [a[1] for a in x_par]
# 	order = np.argsort(x)
# 	xs = np.array(x)[order]
# 	ys = np.array(y)[order]
# 	plt.plot(xs, ys)
# 	##done
# 	plt.title('Sequential vs OpenMP vs pthreads: k=8, threads=' + str(t))
# 	plt.xlabel('N')
# 	plt.ylabel('time')
# 	plt.legend(['sequential', 'pthread', 'openMP'], loc='upper left')
# 	plt.savefig('plots/seq_vs_pth_vs_omp_th_' + str(t), bbox_inches='tight')
# 	plt.show()

# speedup_pth = []
# speedup_omp = []
# x_seq = 1.423491
# x_pth = [(int(d[0]),float(d[5])) for d in data if int(d[1]) == 80000 and int(d[2]) == 8 and int(d[0]) != 1]
# x = [a[0] for a in x_pth]
# y = [a[1] for a in x_pth]
# order = np.argsort(x)
# xs = np.array(x)[order]
# ys = np.array(y)[order]
# speedup_pth = [x_seq/i for i in ys]
# figure(num=None, figsize=(10, 6), dpi=80, facecolor='w', edgecolor='k')
# plt.plot(th, speedup_pth)
# x_omp = [(int(d[0]),float(d[5])) for d in data1 if int(d[1]) == 80000 and int(d[2]) == 8  and int(d[0]) != 1]
# x = [a[0] for a in x_omp]
# y = [a[1] for a in x_omp]
# order = np.argsort(x)
# xs = np.array(x)[order]
# ys = np.array(y)[order]
# speedup_omp = [x_seq/i for i in ys]
# plt.plot(th, speedup_omp)
# plt.title('Speedup OpenMP vs pthreads: k=8, n=80000')
# plt.xlabel('threads')
# plt.ylabel('speedup')
# plt.legend(['pthread', 'openMP'], loc='upper left')
# plt.savefig('plots/speedup', bbox_inches='tight')
# plt.show()

eff_pth = []
eff_omp = []
x_seq = 1.423491
x_pth = [(int(d[0]),float(d[5])) for d in data if int(d[1]) == 80000 and int(d[2]) == 8 and int(d[0]) != 1]
x = [a[0] for a in x_pth]
y = [a[1] for a in x_pth]
order = np.argsort(x)
xs = np.array(x)[order]
ys = np.array(y)[order]
eff_pth = [x_seq/(th[i]*ys[i]) for i in [0,1,2,3]]
figure(num=None, figsize=(10, 6), dpi=80, facecolor='w', edgecolor='k')
plt.plot(th, eff_pth)
x_omp = [(int(d[0]),float(d[5])) for d in data1 if int(d[1]) == 80000 and int(d[2]) == 8  and int(d[0]) != 1]
x = [a[0] for a in x_omp]
y = [a[1] for a in x_omp]
order = np.argsort(x)
xs = np.array(x)[order]
ys = np.array(y)[order]
eff_omp = [x_seq/(th[i]*ys[i]) for i in [0,1,2,3]]
plt.plot(th, eff_omp)
plt.title('Efficiency OpenMP vs pthreads: k=8, n=80000')
plt.xlabel('threads')
plt.ylabel('efficiency')
plt.legend(['pthread', 'openMP'], loc='upper left')
plt.savefig('plots/efficiency', bbox_inches='tight')
plt.show()