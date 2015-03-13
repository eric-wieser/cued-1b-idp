Navigation plan
===============

.. graphviz::

	digraph foo {
		graph[rankdir=LR]

		"Q1" -> "Q2" -> "Q3";
		"Q3" -> "Q4";
		"Q4" -> "Q5" -> "Q6";

   		{ rank=same; Q1; Q4 }
	}

.. doxygenfile:: routines/subroutines.h