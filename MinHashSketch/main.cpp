#include "head.h"
#include "bottomk.h"
#include "linear_counting.h"
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
	printf_s("Cardinality %lf\n", lc.cardinality());
}

void test_bottomk(int n)
{
	int k = sqrt(n*1.);
	BottomK bk(k);
	for (int i = 0; i < n; ++i) {
		bk.add(rand() % n);
		bk.add(i);
	}
	printf_s("Cardinality %lf\n", bk.cardinality());
}

int main()
{
	srand(time(NULL));
	int n;
	while (EOF != scanf_s("%d", &n)) {
		test_bottomk(n);
		test_lc(n);
	}
	return 0;
}