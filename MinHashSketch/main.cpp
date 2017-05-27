#include "head.h"
#include "estimator.h"
#include "bottomk.h"
#include "linear_counting.h"
#include "loglog_counting.h"
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

void test_lc(int n)
{
	auto sketch = make_unique<LinearCounting>(n);
	for (int i = 0; i < n; ++i) {
		sketch->add(rand() % n);
		sketch->add(i);
	}
	double estimation = sketch->cardinality();
	printf_s("Cardinality using Linear Counting: %lf, relative error: %lf\n", estimation, estimation / n - 1);
}

void test_llc(int n)
{
	auto sketch = make_unique<LLC>(64);
	for (int i = 0; i < n; ++i) {
		sketch->add(rand() % n);
		sketch->add(i);
	}
	double estimation = sketch->cardinality();
	printf_s("Cardinality using LogLog Counting: %lf, relative error: %lf\n", estimation, estimation / n - 1);
}

void test_hyperllc(int n)
{
	auto sketch = make_unique<HyperLLC>(1<<13);
	for (int i = 0; i < n; ++i) {
		sketch->add(rand() % n);
		sketch->add(i);
	}
	double estimation = sketch->cardinality();
	printf_s("Cardinality using Hyper LogLog Counting: %lf, relative error: %lf\n", estimation, estimation / n - 1);
}

void test_adc(int n)
{
	auto sketch = make_unique<AdaptiveCounting>(64);
	for (int i = 0; i < n; ++i) {
		sketch->add(rand() % n);
		sketch->add(i);
	}
	double estimation = sketch->cardinality();
	printf_s("Cardinality using Adaptive Counting: %lf, relative error: %lf\n", estimation, estimation / n - 1);
}

void test_bottomk(int n)
{
	int k = sqrt(n * 1.);
	auto sketch = make_unique<BottomK>(k);
	for (int i = 0; i < n; ++i) {
		sketch->add(rand() % n);
		sketch->add(i);
	}
	double estimation = sketch->cardinality();
	printf_s("Cardinality using Bottom-K: %lf, relative error: %lf\n", estimation, estimation / n - 1);
}

int main()
{
	srand(time(NULL));
	int n;
	while (EOF != scanf_s("%d", &n)) {
		test_bottomk(n);
		test_lc(n);
		test_llc(n);
		test_hyperllc(n);
		test_adc(n);
	}
	return 0;
}