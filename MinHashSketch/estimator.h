#pragma once
class Estimator {
public:
	Estimator() {}
	virtual void add(int ele) = 0;
	virtual double cardinality() = 0;
	virtual ~Estimator() {}
};