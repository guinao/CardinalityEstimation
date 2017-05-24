#include "head.h"
#include "bottomk.h"
#include "linear_counting.h"
#include "loglog_counting.h"
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

void test_lc(int n)
{
	LinearCounting lc(n);
	for (int i = 0; i < n; ++i) {
		lc.add(rand() % n);
		lc.add(i);
	}
	printf_s("Cardinality using Linear Counting: %lf\n", lc.cardinality());
}

void test_llc(int n)
{
	LLC llc(1 << 13);
	for (int i = 0; i < n; ++i) {
		llc.add(rand() % n);
		llc.add(i);
	}
	printf_s("Cardinality using Loglog Counting: %lf\n", llc.cardinality());
}

void test_hyperllc(int n)
{
	HyperLLC hllc(1<<13);
	for (int i = 0; i < n; ++i) {
		hllc.add(rand() % n);
		hllc.add(i);
	}
	printf_s("Cardinality using Hyper Loglog Counting: %lf\n", hllc.cardinality());
}

void test_bottomk(int n)
{
	int k = sqrt(n*1.);
	BottomK bk(k);
	for (int i = 0; i < n; ++i) {
		bk.add(rand() % n);
		bk.add(i);
	}
	printf_s("Cardinality using Bottom-K: %lf\n", bk.cardinality());
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
	}
	return 0;
}