from __future__ import  print_function
import re
import numpy as np 
from xml.dom import minidom


def parse_svg(svg_name):
	"""
	takes an inkscape svg file as an input and outputs a dict where each key is the name of a shape, e,g, 'ellipse_vals'
	and each value is a list of tuples where each tuple contains the parameters of said shape
	e.g.:
	{'line_vals': [(1,2,3,4),(2,4,5,1)]}
	"""

	parsed_svg = {}

	doc = minidom.parse(svg_name)  

	#a list containing control point values for bezier
	path_vals = [path.getAttribute('d') for path in doc.getElementsByTagName('path')]
	bezier_vals = [(re.findall(r"[-+]?\d*\.\d+|\d+", str(path))) for path in path_vals if ('c' or 'C') in path]
	line_vals = [(re.findall(r"[-+]?\d*\.\d+|\d+", str(path))) for path in path_vals if ('c') not in path and 'C' not in path]


	if bezier_vals:
		parsed_svg['bezier_vals'] = bezier_vals
	if line_vals:
		parsed_svg['line_vals'] = line_vals

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

	doc.unlink()

	return parsed_svg

if __name__ == '__main__':
	parse_svg('drawing-12.svg')

