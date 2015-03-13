Color classification
====================

The egg sensor reads four variables describing the egg. A range of similar eggs
can be sampled to calibrate the sensor. From these readings, a normal
distribution can be fitted to each variable and egg. Considering covariances,
we can then generalize to a 4-variable normal distribution for each egg.

To identify the egg, the find the normal distribution with the highest
probability density at a given point in variable-space.

The following steppable animation shows how this works for just two variables
(red and blue).

.. raw:: html

	<iframe src="http://eric-wieser.github.io/idp-presentation" allowtransparency="true" style="border: 0; width: 100%; height: 50vh"></iframe>

The conversion of the raw recorded data to a set of normal distribution
parameters is done with a small python script, that leverages the `numpy`
numeric toolkit, and generates the `egg_stats.cc` file.

Api documentation
-----------------
.. doxygenenum:: EggType
	:outline:

.. doxygenvariable:: egg_stats::expectations

.. doxygenstruct:: MultivariateNormal
	:members:
	:undoc-members:
