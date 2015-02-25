

#include "signals.h"


/** Delays the rising edge of a boolean signal.

         Original Signal                              Transient shorter than ticks ignored
       /                                             / 
     ..........__________________________          .....
     :        |  \                       |         :   :
     :        |   Delayed rising edge    |         :   :
_____:________|                          |_________:___:_______
      <------>                            \
        ticks                              No delay on falling edge.

*/
bool delay_rising(int& hist, bool value, int ticks) {
	if (value) {
		hist++;
		return( hist >= ticks);
	}

	hist = 0;
	return( false );
}

/** Delays the falling edge of a boolean signal
(as for delay_rising but for falling edge.)
*/
bool delay_falling(int& hist, bool value, int ticks) {
	if (value) {
		hist = 0;
		return( true );
	}

	hist++;
	return( hist < ticks);
}