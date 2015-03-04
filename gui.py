# This file is part of systemetric-student-robotics.

# systemetric-student-robotics is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# systemetric-student-robotics is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with systemetric-student-robotics.  If not, see <http://www.gnu.org/licenses/>.


import gtk, gobject, cairo, math, threading, time

class Screen(gtk.DrawingArea):
	""" This class is a Drawing Area"""
	def __init__(self):
		super(Screen,self).__init__()
		self.connect("expose_event", self.do_expose_event)
		gobject.timeout_add(50, self.tick)

	def tick(self):
		## This invalidates the screen, causing the expose event to fire.
		self.queue_draw()
		return True # Causes timeout to tick again.

	## When expose event fires, this is run
	def do_expose_event(self, widget, event):
		self.cr = self.window.cairo_create()
		## Call our draw function to do stuff.
		self.draw(*self.window.get_size())

import table
from odometry import Movement, LineSpot, total_seconds, load_from
from datetime import datetime, timedelta
from contextlib import contextmanager

@contextmanager
def cairo_scoped(cr):
	cr.save()
	try:
		yield
	finally:
		cr.restore()

start = datetime.now()
# with open('tracker.dat') as f:
# 	movement = list(load_from(f))

# print movement
movement = [
	Movement(left=0.4, right=1, at=start),
	Movement(left=1, right=1, at=start + timedelta(seconds=0.5)),
	LineSpot(at=start + timedelta(seconds=3)),
	Movement(left=0.4, right=1, at=start + timedelta(seconds=5)),
	Movement(left=0.4, right=0.4, at=start + timedelta(seconds=6)),
	Movement(left=1, right=-1, at=start + timedelta(seconds=7)),
	Movement(left=-1, right=-1, at=start + timedelta(seconds=10)),
	Movement(left=0, right=0, at=start + timedelta(seconds=12))
]

class TableRenderer(Screen):
	def __init__(self):
		super(TableRenderer,self).__init__()

		self.time = None

	def draw(self, w, h):
		self.cr.save()

		self.initScaling(self.cr, w, h)
		self.drawBackground()
		self.drawPath(self.cr)

		self.cr.restore()

	def drawBackground(self):
		cr = self.cr

		cr.save()

		self.cr.set_source_rgb(0.1, 0.1, 0.1) #gray
		self.cr.rectangle(
			-table.width/2, -table.height/2,
			table.width, table.height
		)
		self.cr.fill()

		for (x1, y1), (x2, y2) in table.lines:
			cr.move_to(x1, y1)
			cr.line_to(x2, y2)

		cr.set_line_width(table.tape_width)
		cr.set_source_rgb(1, 1, 1)
		cr.stroke()

		for (x1, y1), (x2, y2) in table.drops:
			cr.move_to(x1, y1)
			cr.line_to(x2, y2)

		cr.set_line_width(max(cr.device_to_user_distance(1,1)))
		cr.set_source_rgb(0.5, 0.5, 0.5)
		cr.stroke()


		for (x, y) in table.boxes:
			cr.rectangle(x-table.box_size/2, y-table.box_size/2, table.box_size, table.box_size)

		cr.set_source_rgb(1, 0.75, 0.75)
		self.cr.fill()

		cr.restore()


	def initScaling(self, cr, w, h):
		cr.rectangle(0, 0, w, h)
		cr.set_source_rgb(0.25, 0.25, 0.25)
		cr.fill()

		#Move to the middle of the screen
		cr.translate(w/2, h/2)

		margin = 0 #px
		actualWidth, actualHeight = table.width, table.height
		scaleFactor = min( (w - 2*margin) /actualWidth, (h - 2*margin)/actualHeight)

		#Scale so that a measurement of 1 corresponds to one meter. Flip Y axis to give normal coords
		cr.scale(scaleFactor, -scaleFactor)

	def drawPath(self, cr):
		with cairo_scoped(cr):
			origin = cr.get_matrix()
			origin.invert()

			cr.translate(-1.2 + 0.36 - 0.15, -0.6)
			at = self.time

			with cairo_scoped(cr):
				cr.set_line_width(max(cr.device_to_user_distance(3,3)))
				last_m = movement[0]
				for m in movement + [None]:
					if at < last_m.at:
						break

					elif not m or at <= m.at:
						last_m.draw(cr, at)
						break

					last_m.draw(cr, m.at)
					cr.set_source_rgb(1, 0, 0)
					cr.stroke()

					if isinstance(m, Movement):
						last_m = m

					elif isinstance(m, LineSpot):
						currmatrix = cr.get_matrix().multiply(origin)
						loc = currmatrix.transform_point(0, 0)
						new_loc = table.nearest_line(loc)
						inv = cairo.Matrix(*currmatrix)
						inv.invert()

						cr.move_to(0, 0)

						cr.transform(inv)
						cr.translate(-loc[0], -loc[1])
						cr.translate(*new_loc)
						cr.transform(currmatrix)
						cr.line_to(0, 0)
						cr.set_source_rgb(1, 0.5, 0)
						cr.stroke()

						last_m = Movement(last_m.l, last_m.r, at=m.at)


				final = cr.get_matrix()

			loc = final.multiply(origin)

			loc_p = loc.transform_point(0, 0)

			cr.set_line_width(max(cr.device_to_user_distance(3,3)))
			cr.set_source_rgb(1, 0, 0)
			cr.stroke()

		line_p = table.nearest_line(loc_p)

		cr.move_to(*loc_p)
		cr.line_to(*line_p)
		cr.set_line_width(max(cr.device_to_user_distance(1,1)))
		cr.set_source_rgb(1, 0, 1)
		cr.stroke()


		with cairo_scoped(cr):
			cr.transform(loc)
			self.drawRobot(cr)

	def drawRobot(self, cr):
		cr.save()
		cr.rectangle(
			-0.05, -0.15,
			0.4, 0.3
		)
		cr.set_source_rgba(0.5, 0.5, 0.5, 0.5)
		cr.fill()

		cr.set_source_rgb(1, 1, 1)
		cr.set_line_width(max(cr.device_to_user_distance(1,1)))
		cr.stroke()
		cr.restore()


	def drawTokens(self, cr):
		cr.set_font_size(max(cr.device_to_user_distance(10, 10)))
		cr.select_font_face("Arial", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD)
		x, y, width, height = cr.text_extents(text)[:4]
		cr.move_to(0.1, height/2)
		cr.show_text(text)


	def startInNewWindow(self):
		self.show()

		window = gtk.Window()
		window.connect("delete-event", gtk.main_quit)
		window.set_title("IDP table map")
		window.set_size_request(500, 500)
		window.add(self)
		window.present()

		gtk.gdk.threads_init()
		t = threading.Thread(target=gtk.main)
		t.start()

def main():
	gui_start = datetime.now()
	round_start = movement[0].at


	tr = TableRenderer()
	tr.time = round_start

	vbox = gtk.VBox(spacing=0)
	vbox.pack_start(tr, expand=True, fill=True)

	scale = gtk.HScale()
	scale.set_range(0, 300)
	scale.add_mark(60, gtk.POS_TOP, None)
	scale.add_mark(120, gtk.POS_TOP, None)
	scale.add_mark(180, gtk.POS_TOP, None)
	scale.add_mark(240, gtk.POS_TOP, None)
	scale.set_value(0)
	vbox.pack_end(scale, expand=False)

	tr.offset = timedelta(0)

	def gui_time():
		return datetime.now() - gui_start

	def update_time():
		t = gui_time() - tr.offset
		tr.time = round_start + t
		scale.set_value(total_seconds(t))
		return True


	def scale_changed(range):
		tr.offset = gui_time() - timedelta(seconds=range.get_value())

	scale.connect('value-changed', scale_changed)



	gobject.timeout_add(50, update_time)

	window = gtk.Window()
	window.connect("delete-event", gtk.main_quit)
	window.set_title("IDP table map")
	window.set_size_request(500, 500)
	window.add(vbox)
	window.show_all()

	gtk.main()

main()
