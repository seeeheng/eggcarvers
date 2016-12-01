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

	x_range = np.linspace(P0.real,P3.real,1000)
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

		if (abs(x_range[x] - last) >= .1):
			last = x_range[x]
			new_x_range.append(int(x_range[x]))
			new_y_range.append(int(y_range[x]))

	plt.plot(new_x_range,new_y_range)


def plot_line(line):
	start = line.start
	end = line.end
	plt.plot([start.real,end.real],[-start.imag,-end.imag])



def parse_svg(svg_name):

	parsed_svg = {}

	doc = minidom.parse(svg_name)  

	# ??
	path_vals = [path.getAttribute('d') for path in doc.getElementsByTagName('path')]

	paths =  parse_path(str(path_vals)) 
	print(paths)
	# a list of tuples containing rectangle parameters
	# (x, y, width, height)
	rect_vals = [(path.getAttribute('x'), path.getAttribute('y'), path.getAttribute('width'),path.getAttribute('height')) 
			for path in doc.getElementsByTagName('rect')]
	if len(rect_vals):
		parsed_svg['rect_vals'] = rect_vals

	# a list of tuples containing ellipse parameters
	# (cx, cy, rx, ry)
	ellipse_vals = [(path.getAttribute('cx'), path.getAttribute('cy'), path.getAttribute('rx'),path.getAttribute('ry')) 
			for path in doc.getElementsByTagName('ellipse')]
	if len(ellipse_vals):
		parsed_svg['ellipse_vals'] = ellipse_vals

	# a list of tuples containing circle parameters
	# (cx, cy, r)
	circle_vals = [(path.getAttribute('cx'), path.getAttribute('cy'), path.getAttribute('r')) 
			for path in doc.getElementsByTagName('circle')]
	if len(circle_vals):
		parsed_svg['circle_vals'] = circle_vals



	# print(rect_vals)
	# print(ellipse_vals)
	# print(circle_vals)
	# print(path_vals)



	print(paths)
	doc.unlink()
	# for path in paths:

	# 	if type(path) is CubicBezier:
	# 		continue
	# 		# plot_bezier_para(path)
		

	# 	elif type(path) is Line:
	# 		print(path)
	# 		# plot_line(path)

parse_svg('drawing-11.svg')
plt.show()

