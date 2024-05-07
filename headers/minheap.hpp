#pragma once
#include <unordered_map>
#include <vector>

class MinHeap {
	public:
		MinHeap(std::vector<std::pair<double,int>> D);
		int size();
		bool empty();
		double peek(int v);
		std::pair<double,int> pop();
		void update(int v, double k);

	private:
		std::vector<std::pair<double,int>> A;
		std::unordered_map<int,int> M;
		void heapify(int i);
		void build_min_heap();
		void decrease_key(int i, double k);
};
