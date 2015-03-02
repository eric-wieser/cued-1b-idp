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
from odometry import Movement
from datetime import datetime, timedelta

start = datetime.now()
movement = [
	Movement(left=1, right=1, at=start),
	Movement(left=0.4, right=1, at=start + timedelta(seconds=5)),
	Movement(left=0.4, right=0.4, at=start + timedelta(seconds=6)),
	Movement(left=1, right=-1, at=start + timedelta(seconds=8)),
	Movement(left=-1, right=-1, at=start + timedelta(seconds=10))
]

class TableRenderer(Screen):

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

		cr.restore()


	def initScaling(self, cr, w, h):
		#Move to the middle of the screen
		cr.translate(w/2, h/2)

		margin = 10 #px
		actualWidth, actualHeight = table.width, table.height
		scaleFactor = min( (w - 2*margin) /actualWidth, (h - 2*margin)/actualHeight)

		#Scale so that a measurement of 1 corresponds to one meter. Flip Y axis to give normal coords
		cr.scale(scaleFactor, -scaleFactor)

	def drawPath(self, cr):
		cr.save()
		cr.translate(-1.2 + 0.36, -0.6)
		at = datetime.now()


		cr.set_line_width(max(cr.device_to_user_distance(3,3)))
		cr.set_source_rgb(1, 0, 0)
		for m1, m2 in zip(movement, movement[1:] + [None]):
			if at < m1.at:
				break

			elif not m2 or at <= m2.at:
				m1.draw(cr, at)
				break

			else:
				m1.draw(cr, m2.at)

		cr.stroke()

		self.drawRobot(cr)
		cr.restore()

	def drawRobot(self, cr):
		cr.save()
		cr.rectangle(
			-0.05, -0.15,
			0.4, 0.3
		)
		cr.set_source_rgb(0.5, 0.5, 0.5)
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
		window.add(self)
		window.present()

		gtk.gdk.threads_init()
		t = threading.Thread(target=gtk.main)
		t.start()

def main():
	mr = TableRenderer()
	mr.startInNewWindow()

	while True:
		pass

main()