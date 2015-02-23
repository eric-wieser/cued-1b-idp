#include <cstdint>
#include <algorithm>
#include <robot_delay.h>
#include "eggsensor.h"

EggSensor::Reading EggSensor::read() {
	const int READ_DELAY = 5; // ms

	Reading res;

	// read rgb + ambient
	{
		_port = 1 << PIN_LEDR;
		delay(READ_DELAY);
		res.r = _r.request(ADC0);

		_port = 1 << PIN_LEDG;
		delay(READ_DELAY);
		res.g = _r.request(ADC0);

		_port = 1 << PIN_LEDB;
		delay(READ_DELAY);
		res.b = _r.request(ADC0);

		_port = 0;
		delay(READ_DELAY);
		res.a = _r.request(ADC0);
	}

	// do some processing
	res.probabilities[0] = 0;
	res.probabilities[1] = 0;
	res.probabilities[2] = 0;


	// find the egg with the best chance
	auto best = std::max_element(res.probabilities.begin(), res.probabilities.end());

	// if none of our matches are good, then assume no egg? (TODO: no egg vs unknown egg)
	if(*best < 0.5) {
		res.bestGuess = EGG_NONE;
	}
	else {
		// convert pointer -> index -> egg_type
		res.bestGuess = static_cast<EggType>(best - res.probabilities.begin());
	}

	return res;
}