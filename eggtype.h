#pragma once

enum EggType {
	EGG_NONE,
	EGG_WHITE,  // deliver to D1
	EGG_BROWN,  // deliver to D2
	EGG_TASTY,  // deliver to D3
	EGG_TYPE_COUNT
};

template<typename E>
struct EnumNames {
};


template<>
struct EnumNames<EggType> {
	static const char* of(EggType e) {
		static const char* values[EGG_TYPE_COUNT] = {"none", "white", "brown", "tasty"};
		return values[e];
	}
};