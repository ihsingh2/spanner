#pragma once
#include <vector>
#include <unordered_map>

class MinHeap {
	public:
		MinHeap(std::vector<std::pair<double,int>> D);
		int size();
		bool empty();
		double peek(int v);
		std::pair<double,int> pop();
		void update(double k, int v);

	private:
		std::vector<std::pair<double,int>> A;
		std::unordered_map<int,int> M;
		void heapify(int i);
		void build_min_heap();
		void decrease_key(double k, int i);
};
