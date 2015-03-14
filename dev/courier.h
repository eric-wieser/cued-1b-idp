#pragma once

#include <array>
#include <stdexcept>
#include "device.h"
#include "ports.h"
#include "../eggtype.h"

/**
	Interface to the runner holding the eggs, its indicator LEDs, the light
	gate that verifies the presence of an egg, and the bucket at the end of the
	runner which delivers the eggs into their cups.

	Affectionately known as the courier, as it carries things
*/
class Courier : public Device {
private:
	std::array<EggType, 3> _contents;
	Port _ledPort;
	Port _lightGatePort;
	int _volume;

	void _updateLeds();
	enum {
		PIN_LIGHTGATE = 7
	};
public:
	Courier(RLink& r, port::Name ledPort, port::Name lightGatePort);

	/**
		Indicate that a new egg has been added to the rail. This updates the
		internal record of currently-held eggs, and turns on the appropriate
		LEDs.
	 */
	void recordEggAdded(EggType e);
	/// Unload the egg at the bottom of the stack, updating state and LEDs 
	void unloadEgg();

	/// type of the egg n from the bottom
	EggType egg(int n) const;

	/// The number of eggs on the rail
	int volume() const;

	/// if an egg is at the bottom of the courier
	bool eggDetected() const;
};