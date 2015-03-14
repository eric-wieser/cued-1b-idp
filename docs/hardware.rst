.. default-domain:: cpp

Hardware access layer
=====================

To ensure hardware is operated correctly, each independant electronic subsystem
is encapsulated in a class. This also allows us to make meaningful constructors
and destructors, such as: setting up ports for inputs; turning off LEDs at
shutdown; driving a motor slowly to hold the deliverer at startup.

These all get wrapped in a single :class:`Robot` instance, with members as
follows.

.. doxygenstruct:: Robot
	:members:
	:undoc-members:


Actuators
---------
Things which cause parts of the robot to move

.. doxygenclass:: Drive
	:members:
.. doxygenclass:: Arm
	:members: Arm, up, down, open, close
	:undoc-members:
.. doxygenclass:: Courier
	:members:


Sensors
-------
Things which give the robot information about its surroundings

.. doxygenclass:: EggSensor
	:members: EggSensor,read
	:undoc-members:

.. doxygenstruct:: EggSensor::Reading
	:members:

.. doxygenclass:: LineSensors
	:members: LineSensors, read
	:undoc-members:

.. doxygenstruct:: LineSensors::Reading
	:members:


.. doxygenclass:: Bumper
	:members: Bumper, read
	:undoc-members:

.. doxygenstruct:: Bumper::Reading
	:members:

For ease of debugging, some of these readings have ``ostream <<`` overloads, to
allow::

    std::cout << robot.ls.read() << std::endl


Low level
---------
All of the above classes use the following utility classes to interface with the
hardware.

.. doxygenclass:: RLink
	:members:

.. doxygenclass:: Device
	:protected-members:

.. doxygenclass:: Port
	:members:

.. doxygennamespace:: port

.. enum:: port::Name

	An enum of port names, from ``P0`` to ``P7``, and ``PA0`` to ``PA7``

	.. doxygenfunction:: operator<<(std::ostream &, const port::Name &)


Exceptions
~~~~~~~~~~

To prevent errors silently occuring without being noticed (or worse, error
codes being handled as values), exceptions are used for all critical errors.
These all derive from `std::exception`, and implement the `const char* what()`
member to give a brief summary of the error to the programmer, to allow them to
fix the appropriate electrical/network problem.

.. doxygenclass:: LinkError
	:members:

.. doxygenstruct:: PortError
	:members:

.. doxygenstruct:: PinsDoublyMapped
	:members:

