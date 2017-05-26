#pragma once

#include "head.h"
#include "estimator.h"

class LinearCounting : public Estimator{
private:
	hash<int> function;
	vector<bool> bitmap;
	size_t m; // hash space size
	int zeros;

	int calM(int n, double e) {
		int lb = 1, ub = INT_MAX;
		while (lb + 1 < ub) {
			int mid = lb + (ub - lb) / 2;
			double t = n * 1. / mid;
			double val = max(5.0, 1. / t / t / e / e) * (exp(t) - t - 1);
			if (mid > val)
				ub = mid;
			else
				lb = mid;
		}
		return ub;
	}

public:
	LinearCounting(int n, double epsilon=0.01) {
		m = calM(n, epsilon);
		bitmap = vector<bool>(m, false);
		zeros = m;
	}

	void add(int ele) {
		int h = function(ele) % m;
		if (!bitmap[h]) {
			--zeros;
			bitmap[h] = true;
		}
	}

	double cardinality() {
		double ret = -(double)m * log((double)zeros / m);
		return ret;
	}

	~LinearCounting() {}
};