#include <cstdint>
#include "ports.h"

uint8_t Port::lastWrites[port::NUM_PORTS] = {0xFF};
uint8_t Port::assignedBits[port::NUM_PORTS] = {0};