#pragma once
#include "eggtype.h"
#include <Eigen/Dense>
using namespace Eigen;


template<int N>
struct MultivariateNormal {
	Matrix<float, N, 1> mean;
	Matrix<float, N, N> covariance;

	// generalization of [(x - mu) / sigma]^2 to N variables - http://en.wikipedia.org/wiki/Mahalanobis_distance
	double mahalanobisDistanceSq(Matrix<float, N, 1> value) const {
		return (value - mean).transpose() * covariance.inverse() * (value - mean);
	}
};

namespace egg_stats {
	extern std::array<MultivariateNormal<4>, EGG_TYPE_COUNT> expectations;
}
