#pragma once
#include "eggtype.h"

namespace egg_stats {

struct NormValue {
	double r;
	double g;
	double w;

	NormValue& operator+=(const NormValue &other) {
		r += other.r;
		g += other.g;
		w += other.w;
		return *this;
	};
	NormValue& operator-=(const NormValue &other) {
		r -= other.r;
		g -= other.g;
		w -= other.w;
		return *this;
	};

	NormValue operator/=(const NormValue &v) {
		r /= v.r;
		g /= v.g;
		w /= v.w;
		return *this;
	}
	NormValue operator*=(const NormValue &v) {
		r *= v.r;
		g *= v.g;
		w *= v.w;
		return *this;
	}

	double sum() const { return r + g + w; }
};
inline NormValue operator+(NormValue lhs, const NormValue& rhs) { return lhs += rhs; }
inline NormValue operator-(NormValue lhs, const NormValue& rhs) { return lhs -= rhs; }
inline NormValue operator*(NormValue lhs, const NormValue& rhs) { return lhs *= rhs; }
inline NormValue operator/(NormValue lhs, const NormValue& rhs) { return lhs /= rhs; }

struct EggExpectation {
	NormValue mean;
	NormValue variance;

	// arbitrary units - normal distributions are hard
	double likelyhood(NormValue reading) const {
		auto diff = (reading - mean);
		auto scaled = diff*diff / variance;
		return 1 / scaled.sum();
	}
};

extern EggExpectation expectations[EGG_TYPE_COUNT];

}
