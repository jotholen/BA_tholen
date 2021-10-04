import numpy as np
import matplotlib.pyplot as plt
import csv

actual_x = []
actual_y = []
actual_z = []

actual_xaxisx = []
actual_xaxisy = []
actual_xaxisz = []

actual_yaxisx = []
actual_yaxisy = []
actual_yaxisz = []

actual_alpha = []
actual_beta = []
actual_gamma = []
times = []

estimated_x = []
estimated_y = []
estimated_z = []

estimated_alpha = []
estimated_beta = []
estimated_gamma = []

estimated_yaxisx = []
estimated_yaxisy = []
estimated_yaxisz = []

estimated_zaxisx = []
estimated_zaxisy = []
estimated_zaxisz = []
times2 = []

estimated_x3 = []
estimated_y3 = []
estimated_z3 = []

estimated_alpha3 = []
estimated_beta3 = []
estimated_gamma3 = []

estimated_yaxisx3 = []
estimated_yaxisy3 = []
estimated_yaxisz3 = []

estimated_zaxisx3 = []
estimated_zaxisy3 = []
estimated_zaxisz3 = []
times3 = []


with open('aprilpose7richtigemessung.csv') as csv_file:
	reader = csv.reader(csv_file ,delimiter=';')
	counter = 0
	for row in reader:
		if counter != 0:
			if (float(row[6])-2775050) < float(3.5):
				actual_x.append(float("{:.3f}".format(float(row[0]))))
				actual_y.append(float("{:.3f}".format(float(row[1]))))
				actual_z.append(float("{:.3f}".format(float(row[2]))))

				actual_alpha.append(float("{:.3f}".format(float(row[3]))))
				actual_beta.append(float("{:.3f}".format(float(row[4]))))
				actual_gamma.append(float("{:.3f}".format(float(row[5]))))
				times.append(float("{:.2f}".format(float(row[6])))-2775050)
		counter += 1

with open('lichter012345pose7sehrkurze2.csv') as csv_file:
	reader = csv.reader(csv_file ,delimiter=';')
	counter = 0
	for row in reader:
		if counter != 0:
			if (float(row[6])-2812504.94) < float(4.5):
				estimated_x.append(float("{:.3f}".format(float(row[0]))))
				estimated_y.append(float("{:.3f}".format(float(row[1]))))
				estimated_z.append(float("{:.3f}".format(float(row[2]))))

				estimated_alpha.append(float("{:.3f}".format(float(row[3]))))
				estimated_beta.append(float("{:.3f}".format(float(row[4]))))
				estimated_gamma.append(float("{:.3f}".format(float(row[5]))))
				times2.append(float("{:.2f}".format(float(row[6])-2812504.94)))
		counter += 1

with open('lichter1256pose3sehrkurze.csv') as csv_file:
	reader = csv.reader(csv_file ,delimiter=';')
	counter = 0
	for row in reader:
		if counter != 0:
			if (float(row[6])-281228) < float(20) and float(row[0]) < 10 and float(row[1]) < 10 and float(row[2]) < 10:
				estimated_x3.append(float("{:.3f}".format(float(row[0]))))
				estimated_y3.append(float("{:.3f}".format(float(row[1]))))
				estimated_z3.append(float("{:.3f}".format(float(row[2]))))

				estimated_alpha3.append(float("{:.3f}".format(float(row[3]))))
				estimated_beta3.append(float("{:.3f}".format(float(row[4]))))
				estimated_gamma3.append(float("{:.3f}".format(float(row[5]))))
				times3.append(float("{:.2f}".format(float(row[6])-2812255.15)))
		counter += 1

plt.figure(figsize = (18,18))
#plot = fig.add_subplot(111)
#plt.plot(times,actual_x, 'r-', label = 'Ground Truth: x', linewidth = 1.5)
#plt.plot(times,actual_y, 'b-', label = 'Ground Truth: y', linewidth = 1.5)
#plt.plot(times,actual_z, 'g-', label = 'Ground Truth: z', linewidth = 1.5)

plt.plot(times,actual_alpha, 'r-', label = 'Ground Truth: alpha (Drehung um z-Achse)', linewidth = 1.5)
plt.plot(times,actual_beta, 'b-', label = 'Ground Truth: beta (Drehung um y-Achse)', linewidth = 1.5)
plt.plot(times,actual_gamma, 'g-', label = 'Ground Truth: gamma (Drehung um x-Achse)', linewidth = 1.5)
#,uncontrolled_x, uncontrolled_y, 'b-')
#plt.plot(times2,estimated_x, '^-', label = 'Geschätzt: x', linewidth = 1.5)
#plt.plot(times2,estimated_y, 'p-', label = 'Geschätzt: y', linewidth = 1.5)#
#plt.plot(times2,estimated_z, 'o-', label = 'Geschätzt: z', linewidth = 1.5)

plt.plot(times2,estimated_alpha, '^-', label = 'Geschätzt: alpha (Drehung um z-Achse)', linewidth = 1.5)
plt.plot(times2,estimated_beta, 'p-', label = 'Geschätzt: beta (Drehung um y-Achse)', linewidth = 1.5)
plt.plot(times2,estimated_gamma, 'o-', label = 'Geschätzt: gamma (Drehung um z-Achse)', linewidth = 1.5)

#plt.plot(times3,estimated_x3, 'r-', label = 'lights: x', linewidth = 1.5)
#plt.plot(times3,estimated_y3, 'b-', label = 'lights: y', linewidth = 1.5)
#plt.plot(times3,estimated_z3, 'g-', label = 'lights: z', linewidth = 1.5)

#plt.plot(times2,estimated_alpha, 'r-', label = 'lights: alpha (z)', linewidth = 1.5)
#plt.plot(times2,estimated_beta, 'b-', label = 'lights: beta (y)', linewidth = 1.5)
#plt.plot(times2,estimated_gamma, 'g--', label = 'lights: gamma (z)', linewidth = 1.5)
plt.locator_params(nbins=5)
plt.xlim(xmin=0, xmax=2.8)
#pyplot.locator_params(axis='y', nbins=6)
#pyplot.locator_params(axis='x', nbins=10)
#plot.xlabel("X")
#plot.ylabel("Y")
#plt.ylimit(30)
plt.grid(axis='y',alpha=0.3,zorder=0,linestyle='--')
plt.grid(axis='x',alpha=0.3,zorder=0,linestyle='--')
#plt.xlabel('X',fontsize=30)
#plt.ylabel('Y',fontsize=30)
#plt.axis([-0.2,12.1,-2.,32.])
#plt.xticks(fontsize=20)
#plt.yticks(fontsize=20)
plt.legend(fontsize=25)
plt.xlabel("Zeit (s)", fontsize=20)
plt.ylabel("Winkel (radian)", fontsize=20)
plt.yticks(fontsize=30)
plt.xticks(fontsize=30)
#plt.xaxis.set_major_locator(plt.MaxNLocator(3))
plt.savefig('pos7eulert' + '.pdf')
plt.show()

#plt.axis('equal')
#plt.plot(actual_x, actual_y, 'r-', desired_x, desired_y, 'g--',uncontrolled_x, uncontrolled_y, 'b-')
#plt.set_figheight(6)
#plt.set_figwidth(8)
#plt.xlabel("X")
#plt.ylabel("Y")
#plt.show()


#    axes = plt. gca()
#    axes. xaxis. label. set_size(20)
#    axes. yaxis. label. set_size(20)
#https://tex.stackexchange.com/questions/196481/problem-on-subfigure-2x2
