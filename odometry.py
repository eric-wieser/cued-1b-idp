from __future__ import division
import math

radius = 0.05
spacing = 0.28
rpm = 40

linear_speed = (2 * math.pi * radius) * (rpm/60)
rotary_speed = linear_speed / (spacing / 2)

# path for py 2.6 -> 2.7
def total_seconds(td):
	return (td.microseconds + (td.seconds + td.days * 24 * 3600) * 10**6) / 10**6

class Event(object):
	def __init__(self, at):
		self.at = at

class Movement(Event):
	def __init__(self, left, right, at):
		"""left and right are motor speeds, at is a timestamp"""
		self.l = left
		self.r = right
		super(Movement, self).__init__(at)

	def __repr__(self):
		return "Movement({s.l}, {s.r}, {s.at!r})".format(s=self)

	@property
	def arc_radius(self):
		"""
		> 0: turning counterclockwise
		= 0: straight
		< 0: turning clockwise
		"""
		assert not self.is_straight
		return (spacing / 2) * (self.r + self.l) / (self.r - self.l)


	@property
	def is_straight(self):
		return self.l == self.r

	@property
	def omega(self):
		r = self.arc_radius
		return rotary_speed * (self.r - self.l) / 2

	@property
	def path_speed(self):
		return linear_speed * (self.l + self.r) / 2


	def draw(self, cr, to):
		dt = total_seconds(to - self.at)

		if self.is_straight:
			cr.move_to(0, 0)
			cr.line_to(self.path_speed * dt, 0)
			cr.translate(self.path_speed * dt, 0)
		else:
			r = self.arc_radius
			if r > 0:
				cr.move_to(0, 0)
				cr.arc( #xc=
					0, #yc=
					self.arc_radius, #radius=
					self.arc_radius, #angle1=
					-math.pi / 2, #angle2=
					-math.pi / 2 + self.omega * dt
				)
			else:
				cr.move_to(0, 0)
				cr.arc_negative( #xc=
					0, #yc=
					self.arc_radius, #radius=
					abs(self.arc_radius), #angle1=
					math.pi / 2, #angle2=
					math.pi / 2 + self.omega * dt
				)
			cr.translate(0, self.arc_radius)
			cr.rotate(self.omega * dt)
			cr.translate(0, -self.arc_radius)

class LineSpot(Event):
	pass

class Pose(Event):
	def __init__(x, y, theta, at):
		"""left and right are motor speeds, at is a timestamp"""
		self.x = x
		self.y = y
		self.theta = theta
		super(Pose, self).__init__(at)


def load_from(f):
	from struct import Struct
	from datetime import datetime

	structs = {
		'M': Struct('Qff'),
		'P': Struct('Qfff'),
		'L': Struct('Q')
	}

	while True:
		header = f.read(1)
		fmt = structs.get(header)
		assert fmt

		packet = f.read(fmt.size)
		if len(packet) != fmt.size:
			break

		args = fmt.unpack(packet)
		t_i, args = args[0], args[1:]
		t = datetime.fromtimestamp(t_i / 1000.0)

		if header == 'M':
			l, r = args

			if l > 1: l = 1
			if l < -1: l = -1

			if r > 1: r = 1
			if r < -1: r = -1

			yield Movement(l, r, t)

		elif header == 'P':
			x, y, theta = args

			yield Pose(x, y, theta, t)

		elif header == 'L':
			yield LineSpot(t)