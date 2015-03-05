#pragma once
#include "eggtype.h"
#include <cmath>
#include <iostream>
#include <iomanip>
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
	extern MultivariateNormal<4> expectations[EGG_TYPE_COUNT];
}