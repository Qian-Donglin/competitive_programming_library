#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//最大の値が出てくるようにするHEAP。最小にしたいのならkeepHeap()とpush()で不等号を逆転すればOK。
template <class T>
struct MyHeap
{
	vector<T> data;

	//1か所だけHEAP条件が壊れているHEAPで、HEAP条件を保つ関数。(1箇所だけなので、どちらの子にとっても親はヒープ条件を満たさないは考えない)
	//渡す頂点と子でヒープ条件が壊れているのが前提。
	//ちゃんとビルドなどを行えばこれでいける。(毎回の更新でたかだか1か所でのヒープ条件破壊に抑えられるので)
	void keepHeap(int pos)
	{
		if (pos * 2 + 1 >= data.size())
			return; //葉まで行った
		int largest_idx = pos;
		if (pos * 2 + 1 < data.size() && data[pos * 2 + 1] > data[largest_idx])
			largest_idx = pos * 2 + 1;
		if (pos * 2 + 2 < data.size() && data[pos * 2 + 2] > data[largest_idx])
			largest_idx = pos * 2 + 2;
		//左へ潜る。
		if (largest_idx == pos * 2 + 1)
		{
			swap(data[pos], data[pos * 2 + 1]);
			keepHeap(pos * 2 + 1);
			return;
		}
		//右へもぐる
		if (largest_idx == pos * 2 + 2)
		{
			swap(data[pos], data[pos * 2 + 2]);
			keepHeap(pos * 2 + 2);
			return;
		}
		//いずれも当てはまらないとき、子の部分木たちはHEAPとして正しいのkeepHeapはここで終わる。
	}

	void buildHeap()
	{
		int parent = data.size() / 2 - 1;
		for (int parent = data.size() / 2 - 1; parent >= 0; parent--)
		{
			keepHeap(parent);
		}
	}

public:
	MyHeap() = default;
	MyHeap(const MyHeap&) = default;
	MyHeap(const vector<T>& ar)
	{
		for (auto item : ar)
		{
			data.push_back(item);
		}
		buildHeap();
	}

	void push(T val)
	{
		//値valを挿入
		//これはmax_heapを使用せずに、普通にwhile文で親をたどりながら、HEAP条件を満たすようにswapしていく。
		data.push_back(val);
		int target = data.size() - 1;
		while (target > 0)
		{
			int parent = (target - 1) / 2;
			if (data[target] < data[parent])
				break;
			swap(data[target], data[parent]);
			target = parent;
		}
	}

	T top()
	{
		return data[0];
	}

	void pop()
	{
		//根と最末端の葉をswapし、サイズを1つけずることで根の値をpopできる。
		swap(data[0], data[data.size() - 1]);
		data.pop_back();
		//あとは、keepHeap()を根から施すだけ。
		keepHeap(0);
	}

	size_t size()
	{
		return data.size();
	}
};
