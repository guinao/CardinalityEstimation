#pragma once

#include "head.h"
#include "estimator.h"

static uint8_t num_of_trail_zeros(uint64_t i)
{
	uint64_t y;
	uint8_t n = 63;

	if (i == 0)
		return 64;

	y = i << 32;    if (y != 0) { n -= 32; i = y; }
	y = i << 16;    if (y != 0) { n -= 16; i = y; }
	y = i << 8;     if (y != 0) { n -= 8; i = y; }
	y = i << 4;     if (y != 0) { n -= 4; i = y; }
	y = i << 2;     if (y != 0) { n -= 2; i = y; }

	return n - (uint8_t)((i << 1) >> 63);
}

class LLC : public Estimator {
private:
	hash<int> function;
	vector<int> first1bit;
	double alpham;
	int m;

public:
	LLC(int _m) {
		m = _m;
		double pi = acos(-1);
		alpham = 0.79402*m - (2 * pi * pi + log(4)) / 24;
//		cout << alpha << endl;
		first1bit = vector<int>(m, 0);
	}

	double cardinality() {
		double z = 0.0;
		for (int num : first1bit) {
			z += num;
		}
		return alpham * pow(2.0, z / m);
	}

	void add(int ele) {
		size_t h = function(ele);
		int bucket = h % m;
		int num = h / m;
		first1bit[bucket] = max((int)num_of_trail_zeros(num), first1bit[bucket]);
	}

	~LLC() {}
};

class HyperLLC : public Estimator {
private:
	hash<int> function;
	vector<int> first1bit;
	double alphamm;
	double pow2_32;	// a magic number 2^32/30 in origin paper
	int num_of_zeros;
	int m;

public:
	HyperLLC(int _m) {
		m = _m;
		int log2m = num_of_trail_zeros(m);
		pow2_32 = pow(2.0, 32);
		/*
		* Description of the following magical numbers:
		*
		* In the HyperLogLog paper page 12-13, alphaMM is a_m*m^2, where:
		*
		*  a_m := 1/(m*J_0(m))
		*
		* Here J_s(m) is not the first-kind Bessel function, but defined as the
		* value of a special integrals:
		*
		*  J_s(m) := integral(u^s*f(u)^m, u=0..inf)
		*
		* where f(u) := log_2((2+u)/(1+u))
		*
		* After some deductions, we know that J_0(m) can be estimated by:
		*
		*  J_0(m) ~= 2*ln(2)/m*(1+(3*ln(2)-1)/m)
		*
		* As 1/(2*ln(2)) ~= 0.72135, 3*ln(2)-1 ~= 1.0794, thus:
		*
		*  a_m ~= 0.72135/(1+1.0794/m)
		*
		* When log_2(m)={4,5,6}, the corresponding a_m will be:
		*
		*  a_16 ~= 0.72135/(1+1.0794/16) = 0.67576
		*  a_32 ~= 0.72135/(1+1.0794/32) = 0.69781
		*  a_64 ~= 0.72135/(1+1.0794/64) = 0.70939
		*
		* There're small errors between calculated and actually used values,
		* because stream-lib copied those values from the pseudo code in page 14
		* directly. We had to keep compatibility with stream-lib so can't correct
		* these values.
		**/

		if (4 == log2m) {
			alphamm = 0.673 * m * m;
		}
		else if (5 == log2m) {
			alphamm = 0.697 * m * m;
		}
		else if (6 == log2m) {
			alphamm = 0.709 * m * m;
		}
		else {
			alphamm = (0.7213 / (1 + 1.079 / m)) * m * m;
		}

		first1bit = vector<int>(m, 0);
		num_of_zeros = m;
	}

	double cardinality() {
		double z = 0.0;
		for (int num : first1bit) {
			z += pow(2.0, -1.0*num);
		}
		double estimator = alphamm / z;
		if (estimator <= 2.5*m) {
			if (num_of_trail_zeros == 0)
				return estimator;
			else
				return m*1.0*log(m*1. / num_of_zeros);
		}
		else if (estimator <= pow2_32/30) {
			return estimator;
		}
		else {
			return -pow2_32 * log(1 - estimator / pow2_32);
		}
	}

	void add(int ele) {
		size_t h = function(ele);
		int bucket = h % m;
		int num = h / m;
		if (first1bit[bucket] == 0)
			--num_of_zeros;

		first1bit[bucket] = max((int)num_of_trail_zeros(num)+1, first1bit[bucket]);
	}

	~HyperLLC() {}
};