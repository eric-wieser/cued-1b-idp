#include <cstdint>
#include <algorithm>
#include <robot_delay.h>
#include "eggsensor.h"
#include "egg_stats.h"
#include <Eigen/Dense>
using namespace Eigen;


EggSensor::EggSensor(RLink& r, port::Name port) :
	Device(r),
	_port(r, port, (1 << PIN_LEDR) | (1 << PIN_LEDB) | (1 << PIN_LEDW))
{
	// turn off LEDs at startup
	_port = 0xff;
};

EggSensor::~EggSensor() {
	// turn off LEDs on shutdown too
	_port = 0xff;
}

static int sampleADC(RLink& r, int samples) {
	int res = 0;
	for(int i = 0; i < samples; i++)
		res += r.request(ADC0);
	return res / samples;
}

EggSensor::Reading EggSensor::read(int samples) {
	const int READ_DELAY = 50; // ms

	Reading res;

	// read rgb + ambient
	{
		_port = ~(1 << PIN_LEDR);
		delay(READ_DELAY);
		res.r = sampleADC(_r, samples);

		_port = ~(1 << PIN_LEDB);
		delay(READ_DELAY);
		res.b = sampleADC(_r, samples);

		_port = ~(1 << PIN_LEDW);
		delay(READ_DELAY);
		res.w = sampleADC(_r, samples);

		_port = 0xff;
		delay(READ_DELAY);
		res.a = sampleADC(_r, samples);
	}

	Matrix<float,4,1> normed;
	normed << res.r, res.b, res.w, res.a;

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
	return stream << "{"
		<< "egg: " << r.bestGuess << ", "
		<< "r: " << r.r << ", "
		<< "b: " << r.b << ", "
		<< "w: " << r.w << ", "
		<< "a: " << r.a << ", "
		<< "p: {"
			<< "n: " << r.probabilities[EGG_NONE] << ", "
			<< "w: " << r.probabilities[EGG_WHITE] << ", "
			<< "b: " << r.probabilities[EGG_BROWN] << ", "
			<< "c: " << r.probabilities[EGG_TASTY]
		<< "}"
	<< "}";
}
