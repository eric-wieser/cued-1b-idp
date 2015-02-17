#pragma once
#include "device.h"
#include "eggtype.h"

class Rail : Device {
	EggType _contents[3];
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
};