from __future__ import division, print_function
import numpy as np 
import matplotlib.pyplot as plt
from math import floor
from xml.dom import minidom
from svgpathtools import Path, Line, QuadraticBezier, CubicBezier, Arc, parse_path



def para_cubic_bezier(P0,P1,P2,P3,t):
	"""takes in start and end point as well as two control points and a t value and spits out the x and y
	vals of a bezier curve"""

	x = (1-t)**3*P0.real + 3*t*(1-t)**2*P1.real + 3*t**2*(1-t)*P2.real + t**3*P3.real
	y = (1-t)**3*P0.imag + 3*t*(1-t)**2*P1.imag + 3*t**2*(1-t)*P2.imag + t**3*P3.imag
	
	return x,y


def cubic_bezier(P0,P1,P2,P3,x):
	"""
	spits out y-val as a function of x, where 0,0 is the start of the curve
	"""

	t3 = -P0.real + 3*P1.real - 3*P2.real + P3.real
	t2 = 3*P0.real -6*P1.real + 3*P2.real
	t1 = -3*P0.real + 3*P1.real
	t0 = P0.real - x

	roots = [t3,t2,t1,t0]
	zeroes = np.roots(roots)

	zeroes = [val for val in zeroes if val >= 0 and val <= 1]

	t = float(zeroes[0])

	y = (1-t)**3 * P0.imag+ 3*(1-t)**2 * t * P1.imag + 3*(1-t) * t**2 * P2.imag + t**3 * P3.imag

	return y


def plot_bezier(bezier):

	P0 = (bezier.start)
	P1 = (bezier.control1)
	P2 = (bezier.control2)
	P3 = (bezier.end)

	x_range = np.linspace(P0.real,P3.real,100)
	y_range = []

	for x in x_range:
		y_range.append(-cubic_bezier(P0, P1, P2, P3, x))

	plt.plot(x_range,y_range)


def plot_bezier_para(bezier):

	P0 = (bezier.start)
	P1 = (bezier.control1)
	P2 = (bezier.control2)
	P3 = (bezier.end)

	t_range = np.linspace(0,1,10000)
	x_range = []
	y_range = []

	print(para_cubic_bezier(P0, P1, P2, P3, .999999))

	for t in t_range:
	
		x = para_cubic_bezier(P0, P1, P2, P3, t)[0]

		y = -para_cubic_bezier(P0, P1, P2, P3, t)[1]

		x_range.append(x)
		y_range.append(y)

	new_x_range = []
	new_y_range = []
	last = x_range[0]


	new_x_range.append(round(last))
	new_y_range.append(y_range[0])

	for x in range(len(x_range)):

		if (abs(x_range[x] - last) >= 3):
			last = x_range[x]
			new_x_range.append(int(x_range[x]))
			new_y_range.append(int(y_range[x]))

	# print(new_x_range)
	# print(new_y_range)
	# print(len(new_y_range))

	#print(x_range)
	#print(y_range)
	plt.plot(new_x_range,new_y_range)

def parse_svg(svg_name):

	doc = minidom.parse(svg_name)  
	path_strings = [path.getAttribute('d') for path
                in doc.getElementsByTagName('path')]
	doc.unlink()

	# print(str(path_strings))

	paths =  parse_path(str(path_strings)) 

	# print(paths) 

	for path in paths:

		if type(path) is CubicBezier:
			print('yooo')
			print(path)
			# plot_bezier_para(path)
			#plot_bezier(path)

		elif type(path) is Line:
			print('booo')
			continue

parse_svg('drawing_5.svg')
plt.show()