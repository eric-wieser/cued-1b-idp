#pragma once
#include <array>
#include "device.h"
#include "ports.h"
#include "eggtype.h"

class Courier : public Device {
private:
	std::array<EggType, 3> _contents;
	Port _ledPort;
	int _volume;

	void _updateLeds();
public:
	/**
		Indicate that a new egg has been added to the rail. This updates the
		internal record of currently-held eggs, and turns on the appropriate
		LEDs.
	 */
	void recordEggAdded(EggType e);
	/// Unload the egg at the bottom of the stack, updating state and LEDs 
	void unloadEgg();
	/// The number of eggs on the rail
	int volume();

	Courier(RLink& r, port::Name ledPort)
		: Device(r), _ledPort(r, ledPort), _volume(0) {};
};