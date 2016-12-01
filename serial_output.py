import serial
from svg_parser import parse_svg
port = '/dev/ttyACM0'
ser = serial.Serial(port, 9600)

def serial_out(svg):

	svg_dict = parse_svg(svg)

	for shapes in svg_dict:
		if shapes == 'circle_vals':
			for circle in svg_dict[shapes]:
				ser.write('circle')
				ser.write(circle)

		if shapes == 'ellipse_vals':
			for ellipse in svg_dict[shapes]:
				ser.write('ellipse')
				ser.write(ellipse)

		if shapes == 'rect_vals':
			for rect in svg_dict[shapes]:
				ser.write('rect')
				ser.write(rect)

		if shapes == 'line_vals':
			for line in svg_dict[shapes]:
				ser.write('line')
				ser.write(line)

		if shapes == 'bezier_vals':
			for bezier in svg_dict[shapes]:
				ser.write('bezier')
				ser.write(bezier)


