#include <cstdint>
#include <algorithm>
#include <robot_delay.h>
#include "eggsensor.h"
#include "egg_stats.h"
#include <Eigen/Dense>
using namespace Eigen;

EggSensor::Reading EggSensor::read() {
	const int READ_DELAY = 50; // ms

	Reading res;

	// read rgb + ambient
	{
		_port = ~(1 << PIN_LEDR);
		delay(READ_DELAY);
		res.r = _r.request(ADC0);

		_port = ~(1 << PIN_LEDG);
		delay(READ_DELAY);
		res.g = _r.request(ADC0);

		_port = ~(1 << PIN_LEDW);
		delay(READ_DELAY);
		res.w = _r.request(ADC0);

		_port = 0xff;
		delay(READ_DELAY);
		res.a = _r.request(ADC0);
	}

	Matrix<float,4,1> normed;
	normed << res.r, res.g, res.w, res.a;

	// do some processing
	for(int i = 0; i < EGG_TYPE_COUNT; i++) {
		res.probabilities[i] = egg_stats::expectations[i].mahalanobisDistanceSq(normed);
	}


	// find the egg with the best chance
	auto best = std::min_element(res.probabilities.begin(), res.probabilities.end());

	// convert pointer -> index -> egg_type
	res.bestGuess = static_cast<EggType>(best - res.probabilities.begin());

	return res;
}

std::ostream& operator <<(std::ostream& stream, const EggSensor::Reading& r) {
	return stream << "{egg: " << EnumNames<EggType>::of(r.bestGuess)
		<< ", r: " << r.r
		<< ", g: " << r.g
		<< ", w: " << r.w
		<< ", a: " << r.a
		<< ", p: {"
			<< "n: " << r.probabilities[EGG_NONE]
			<< ", w: " << r.probabilities[EGG_WHITE]
			<< ", b: " << r.probabilities[EGG_BROWN]
			<< ", c: " << r.probabilities[EGG_TASTY]
		<< "}"
		<< "}";
}
