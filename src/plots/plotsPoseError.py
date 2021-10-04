import numpy as np
import matplotlib.pyplot as plt
import math
import csv
import statistics
import scipy
from scipy.spatial.transform import Rotation as R


pos = ["024","0145", "01245", "012345"]
class plotter:
	def getMedian(file, co):
		actual_x = []
		actual_y = []
		actual_z = []
		print(file)
		m = 0
		with open(file) as csv_file:
			reader = csv.reader(csv_file ,delimiter=';')
			counter = 0
			for row in reader:
				if counter != 0:
					if co == "x":
						m = m + float(row[0])
					if co == "y":
						m = m + float(row[1])
					if co == "z":
						m = m + float(row[2])
					if co == "xo":
						m = m + float(row[5])
					if co == "yo":
						m = m + float(row[4])
					if co == "zo":
						m = m + float(row[3])
					actual_x.append(float("{:.3f}".format(float(row[0]))))
					actual_y.append(float("{:.3f}".format(float(row[1]))))
					actual_z.append(float("{:.3f}".format(float(row[2]))))
				counter += 1
		return m / len(actual_x)
	def getDeviation(file, m, aprilX, aprilY, aprilZ):
		actual_x = []
		actual_y = []
		actual_z = []
		print(file)
		s = 0
		with open(file) as csv_file:
			reader = csv.reader(csv_file ,delimiter=';')
			counter = 0
			for row in reader:
				if counter != 0:
					if ((float(row[0]) - 0)**2 + (float(row[1]) - 0)**2 + (float(row[2])-0)**2)**(1/float(2)) < 30:
						dist = ((float(row[0]) - aprilX)**2 + (float(row[1]) - aprilY)**2 + (float(row[2])-aprilZ)**2)**(1/float(2))
						s = s + (m - dist)**2
						actual_x.append(float("{:.3f}".format(float(row[0]))))
						actual_y.append(float("{:.3f}".format(float(row[1]))))
						actual_z.append(float("{:.3f}".format(float(row[2]))))
				counter += 1
		s = s / len(actual_x)
		return (s)**(1/float(2))
	def getMiddle(file, aprilX, aprilY, aprilZ):
		actual_x = []
		actual_y = []
		actual_z = []
		print(file)
		m = 0
		with open(file) as csv_file:
			reader = csv.reader(csv_file ,delimiter=';')
			counter = 0
			for row in reader:
				if counter != 0:
					if ((float(row[0]) - 0)**2 + (float(row[1]) - 0)**2 + (float(row[2])-0)**2)**(1/float(2)) < 30:
						m = m + ((float(row[0]) - aprilX)**2 + (float(row[1]) - aprilY)**2 + (float(row[2])-aprilZ)**2)**(1/float(2))
						actual_x.append(float("{:.3f}".format(float(row[0]))))
						actual_y.append(float("{:.3f}".format(float(row[1]))))
						actual_z.append(float("{:.3f}".format(float(row[2]))))
				counter += 1
		return m / len(actual_x)
	def getMiddleOrientation(file):
		err = []
		print(file)
		m = 0
		with open(file) as csv_file:
			reader = csv.reader(csv_file ,delimiter=';')
			m = 0
			counter = 0
			for row in reader:
				if counter > 10:
					m = m + float(row[0]) * 180 / math.pi
					err.append(float("{:.3f}".format(float(row[0]))))
				counter += 1
		return m/len(err)
	def getDeviationOrientation(file, m):
		err = []
		print(file)
		s = 0
		with open(file) as csv_file:
			reader = csv.reader(csv_file ,delimiter=';')
			counter = 0
			for row in reader:
				if counter > 10:
					s = s + (m - (float(row[0])* 180 / math.pi))**2
					err.append(float("{:.3f}".format(float(row[0]))))
				counter += 1
		return (s / len(err))**(1/float(2))
	def getMiddleaprilori(file):
		actual_x = []
		actual_y = []
		actual_z = []
		print(file)
		x = 0
		y = 0
		z = 0
		with open(file) as csv_file:
			reader = csv.reader(csv_file ,delimiter=';')
			counter = 0
			for row in reader:
				if counter != 0:
					x = x + float(row[3])
					y = y + float(row[4])
					z = z + float(row[5])
					actual_x.append(float("{:.3f}".format(float(row[3]))))
				counter += 1
			print("z" + str(x/ len(actual_x)))
			print("y" + str(y/ len(actual_x)))
			print("x" + str(z/ len(actual_x)))
		return 0
		
i=3
plt.plot()
plt.figure(figsize = (16,16))
plotter.getMiddleaprilori('aprilpose' + str(3) + 'richtigemessung.csv')
c2 = 0
for x in pos:
	c2 = c2 + 1
	print(x)
	file = ""
	color = 'b<' 
	labelPlus = ""
	if x == "01345":
		color = 'm'
		style = "+"
		labelPlus = "12456"
	if x == "012345":
		color = 'c'
		style = 'x'
		labelPlus = "123456"
	if x == "01245":
		color = 'g'
		style = '^'
		labelPlus = "12356"
	if x == "01234":
		color = 'r'
		style = 'x'
		labelPlus = "12345"
	if x == "0134":
		color = 'g'
		style = '^'
		labelPlus = "1245"
	if x == "0145":
		color = 'm'
		style = '+'
		labelPlus="1256"
	if x == "1235":
		color = 'r'
		style = 'x'
		labelPlus="2346"
	if x == "024":
		style = '^'
		color = 'c'
		labelPlus="135"
	if x == "123":
		style = 'v'
		color = 'r'
		labelPlus="234"
	if x == "135":
		style = 'x'
		color = 'm'
		labelPlus = "246"
	if x == "014":
		style = '+'
		color = 'g'
		labelPlus = "125"
	if x == "april":
		color = 'ko'
	while i < 10:
#		aprilFile = 'aprilpose' + str(i) + 'richtigemessung.csv'##
#		xA = plotter.getMedian(aprilFile, "x")
#		yA = plotter.getMedian(aprilFile, "y")
#		zA = plotter.getMedian(aprilFile, "z")
#		xoA = plotter.getMedian(aprilFile, "xo")
#		yoA = plotter.getMedian(aprilFile, "yo")
#		zoA = plotter.getMedian(aprilFile, "zo")
#		if x == "april":
#			file = 'aprilpose' + str(i) + 'richtigemessung.csv'
#		else:
#			file = 'lichter' + x + 'pose' + str(i) + 'sehrkurze2.csv'
#		m = plotter.getMiddle(file, xA, yA, zA)
#		s = plotter.getDeviation(file, m, xA, yA, zA)
		#plt.scatter(i, m, marker='+', c='k', s=100, label='the data')
		g = i
		if(c2 == 1):
			g = g -0.15
		if(c2 == 2):
			g = g - 0.1
		if(c2 == 3):
			g = g + 0.1
		if(c2 == 4):
			g = g + 0.15

		#plt.errorbar(g ,m, yerr=s, marker=style, c=color, label= ("Lichter: "+labelPlus), markersize=20)
		m = plotter.getMiddleOrientation('orienterrorpose' + str(i) + x + '.csv')
		m = m 
		s = plotter.getDeviationOrientation('orienterrorpose' + str(i) + x + '.csv', m)
		s = s
		plt.errorbar(g ,m, yerr=s, marker=style, c=color, label= ("Lichter: "+labelPlus), markersize=20)
		i=i+1
	i = 3
plt.locator_params(nbins=10)
plt.ylim(ymin=0)
plt.grid(axis='y',alpha=0.5,zorder=0,linestyle='--')
plt.grid(axis='x',alpha=0.5,zorder=0,linestyle='--')
plt.legend(fontsize=40)
l = [3,4,5,6,7,8,9]
labels = ["Pose 3", "Pose 4", "Pose 5", "Pose 6", "Pose 7", "Pose 8", "Pose 9"]
plt.xticks(l, labels, rotation ='vertical', fontsize=30)
plt.xlabel("Posen der Kamera", fontsize=20)
plt.ylabel("Orientierungsfehler (deg)", fontsize=20)
plt.yticks(fontsize=30)
#plt.yticks(fontsize=20)
plt.savefig('oribestelichtertest3' + '.pdf')
plt.show()
