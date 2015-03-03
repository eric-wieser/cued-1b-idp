width = 2.4
height = 2.4

tape_width = 0.022

def hline(xs, y):
	return ((xs[0], y), (xs[1], y))

def vline(x, ys):
	return ((x, ys[0]), (x, ys[1]))

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
	(-0.6, 1.2 - 0.36*2 - box_size/2),
	(0, 1.2 - 0.36*2 - box_size/2),
	(-1.2 + box_size/2, 1.2 - 0.36)
]