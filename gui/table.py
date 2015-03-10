width = 2.4
height = 2.4

class LineSegment(object):
	def __init__(self, (x1, y1), (x2, y2)):
		self.p1 = x1, y1
		self.p2 = x2, y2

	def nearest(self, p):
		import math

		def dist(p2, p1):
			return p2[0] - p1[0], p2[1] - p1[1]

		def dot(p1, p2):
			return p2[0] * p1[0] + p2[1] * p1[1]

		# test vector
		t = dist(p, self.p1)

		# normal vector
		n = dist(self.p2, self.p1)

		f = dot(n, t) / dot(n, n)

		if f < 0:
			f = 0
		elif f > 1:
			f = 1

		return self.p1[0] + n[0] * f, self.p1[1] + n[1] * f

	def __iter__(self):
		yield self.p1
		yield self.p2

def nearest_line(p):
	line_ps = [l.nearest(p) for l in lines]
	return min(line_ps, key=lambda lp: (lp[0] - p[0])**2 + (lp[1] - p[1])**2)

tape_width = 0.022

def hline(xs, y):
	return LineSegment((xs[0], y), (xs[1], y))

def vline(x, ys):
	return LineSegment((x, ys[0]), (x, ys[1]))

lines = [
	# outer square
	hline([-1.2, 1.2], -1.2 + 0.36),
	hline([-1.2, 1.2], 1.2 - 0.36),
	vline(-1.2 + 0.36, [-1.2, 1.2]),
	vline(1.2 - 0.36, [-1.2, 1.2]),

	# starting line
	hline([-1.2, 1.2], -0.6),

	# center lines
	hline([-1.2, 1.2 - 2*0.36], 0),
	vline(0, [-1.2, 1.2]),

	# starting box
	hline([
		-1.2 + 0.36 + 0.20,
		-1.2 + 0.36 - 0.20
	], -0.6 + (0.15 - tape_width/2)),
	hline([
		-1.2 + 0.36 + 0.20,
		-1.2 + 0.36 - 0.20
	], -0.6 - (0.15 - tape_width/2)),
	vline(
		-1.2 + 0.36 + (0.20 - tape_width/2),
		[
			-0.6 + 0.15,
			-0.6 - 0.15
		]
	),
	vline(
		-1.2 + 0.36 - (0.20 - tape_width/2),
		[
			-0.6 + 0.15,
			-0.6 - 0.15
		]
	),

	# conveyor lines
	vline(-0.4, [-0.6, -1.2]),
	vline(-0.2, [-0.6, -1.2]),
	vline( 0.2, [-0.6, -1.2]),
	vline( 0.4, [-0.6, -1.2])
]

drops = [
	hline([-1.2, 1.2 - 0.36*2], 1.2 - 0.36*2),
	vline(1.2 - 0.36*2, [1.2 - 0.36*2, 1.2 - 0.36*4])
]

box_size = 0.13

boxes = [
	(-1.2 + 0.36, 1.2 - 0.36*2 - box_size/2),
	(0, 1.2 - 0.36*2 - box_size/2),
	(-1.2 + box_size/2, 1.2 - 0.36)
]