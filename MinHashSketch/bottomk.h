#pragma once

#include "head.h"
#include "estimator.h"

bool cmp(const pair<int, double>& lhs, const pair<int, double>& rhs)
{
	return lhs.second < rhs.second;
}

class BottomK : public Estimator{
private:
	int k;
	hash<int> func;
	vector<pair<int, double>> sketch;
	unordered_set<int> lookup;

public:
	BottomK(int _k) : k(_k) {}

	void add(int ele) {
		size_t h = func(ele);
		double v = (double)(h % INT_MAX) / INT_MAX;
		if (sketch.size() < k) {
			sketch.push_back(make_pair(ele, v));
			push_heap(sketch.begin(), sketch.end(), cmp);
			lookup.insert(ele);
		}
		else if (v < sketch[0].second && lookup.count(ele) == 0) {
			pop_heap(sketch.begin(), sketch.end(), cmp);
			lookup.erase(sketch.back().first);
			lookup.insert(ele);
			sketch.back() = make_pair(ele, v);
			push_heap(sketch.begin(), sketch.end(), cmp);
		}
	}

	void add(vector<int> &eles) {
		for (int& e : eles) {
			add(e);
		}
	}

	vector<pair<int, double>> getSketch()
	{
		vector<pair<int, double> > ret = sketch;
		sort_heap(ret.begin(), ret.end(), cmp);
		return ret;
	}

	int size()
	{
		return sketch.size();
	}

	double cardinality() 
	{
		if (sketch.size() < k)
			return sketch.size();
		else
			return k / sketch[0].second;
	}

	friend BottomK operator +(const BottomK &lhs, const BottomK &rhs) {
		BottomK ret(lhs.k);
		for (auto p : lhs.sketch) {
			ret.add(p.first);
		}
		for (auto p : rhs.sketch) {
			ret.add(p.first);
		}
		return ret;
	}

	~BottomK() {}
};
