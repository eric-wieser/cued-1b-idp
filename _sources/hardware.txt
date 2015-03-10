.. _hardware:

Hardware access layer
=====================

To ensure hardware is operated correctly, each independant electronic subsystem
is encapsulated in a class. This also allows us to make meaningful constructors
and destructors, such as: setting up ports for inputs; turning off LEDs at
shutdown; driving a motor slowly to hold the deliverer at startup.

Low level
---------
A bunch of utility classes to help implement the higher-level abstractions.

.. doxygenclass:: RLink
	:members:

.. doxygenclass:: Device
	:protected-members:

.. doxygenclass:: Port
	:members:


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


Actuators
---------
Things which cause parts of the robot to move

.. doxygenclass:: Drive
	:members:
.. doxygenclass:: Arm
	:members:
.. doxygenclass:: Courier
	:members:


Sensors
-------
Things which give the robot information about its surroundings

.. doxygenclass:: EggSensor
	:members:
.. doxygenclass:: LineSensors
	:members:
