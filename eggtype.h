#pragma once
#include <iostream>

enum EggType {
	EGG_NONE,
	EGG_WHITE,  // deliver to D1
	EGG_BROWN,  // deliver to D2
	EGG_TASTY,  // deliver to D3
	EGG_TYPE_COUNT
};

inline std::ostream& operator <<(std::ostream& stream, const EggType& r) {
	static const char* values[EGG_TYPE_COUNT] = {"none", "white", "brown", "tasty"};
	return stream << values[r];
}
