/*
https://cplusplus.com/forum/general/138359/
の設計を参考にした
*/

#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

template <typename T, typename Compare = std::less<T>>
class Heap {
public:
	void insert (const T& element)
	{
		// 末尾に加える
		heap.push_back(element);
		// 逆転がなくなるまで上にあげる
		heapifyup(heap.size() - 1);
	}

	T deletemin()
	{
		if (heap.empty())
			throw std::out_of_range("Heap is empty");
		// 根を持ってくる
		T min = heap.front();
		// 末尾の要素を根に持ってくる
		heap[0] = heap.back();
		heap.pop_back();
		// 逆転が起きなくなるまで下げる
		heapifydown(0);
		return (min);
	}

	void print()
	{
		std::cout << "Heap = ";
		for (const T& element: heap)
			std::cout << element << " ";
		std::cout << std::endl;
	}

	int size() const
	{
		return (heap.size());
	}

	bool empty()
	{
		return (heap.empty());
	}

private:
	std::vector<T> heap;
	Compare compare;

	int left(int parent)
	{
		return ((2 * parent + 1 < heap.size()) ? 2*parent + 1 : -1);
	}

	int right(int parent)
	{
		return ((2 * parent + 2 < heap.size()) ? 2 * parent + 2 : -1);
	}

	int parent(int child)
	{
		return ((child == 0) ? -1 : (child - 1) / 2);
	}

	void heapifyup(int index)
	{
		// 親よりも子が大きい間、親と子をswapし続ける
		while (index > 0 && parent(index) >= 0 && compare(heap[index], heap[parent(index)]))
		{
			std::swap(heap[parent(index)], heap[index]);
			index = parent(index);
		}
	}

	void heapifydown(int index)
	{
		// 左右の子の小さい(resp.大きい)方を取ってくる
		int child = left(index);
		if (child > 0 && right(index) > 0 && compare(heap[right(index)], heap[child]))
		{
			child = right(index);
		}
		// それが親より小さい(resp.大きい)なら親と子をswap, 再帰的に行う
		if (child > 0 && compare(heap[child], heap[index]))
		{
			std::swap(heap[index], heap[child]);
			heapifydown(child);
		}
	}
};
