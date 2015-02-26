#include <cstdint>
#include "ports.h"

uint8_t Port::lastWrites[port::NUM_PORTS] = {0xFF };