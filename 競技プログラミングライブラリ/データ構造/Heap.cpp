#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//�ő�̒l���o�Ă���悤�ɂ���HEAP�B�ŏ��ɂ������̂Ȃ�keepHeap()��push()�ŕs�������t�]�����OK�B
template <class T>
struct MyHeap
{
	vector<T> data;

	//1��������HEAP���������Ă���HEAP�ŁAHEAP������ۂ֐��B(1�ӏ������Ȃ̂ŁA�ǂ���̎q�ɂƂ��Ă��e�̓q�[�v�����𖞂����Ȃ��͍l���Ȃ�)
	//�n�����_�Ǝq�Ńq�[�v���������Ă���̂��O��B
	//�����ƃr���h�Ȃǂ��s���΂���ł�����B(����̍X�V�ł�������1�����ł̃q�[�v�����j��ɗ}������̂�)
	void keepHeap(int pos)
	{
		if (pos * 2 + 1 >= data.size())
			return; //�t�܂ōs����
		int largest_idx = pos;
		if (pos * 2 + 1 < data.size() && data[pos * 2 + 1] > data[largest_idx])
			largest_idx = pos * 2 + 1;
		if (pos * 2 + 2 < data.size() && data[pos * 2 + 2] > data[largest_idx])
			largest_idx = pos * 2 + 2;
		//���֐���B
		if (largest_idx == pos * 2 + 1)
		{
			swap(data[pos], data[pos * 2 + 1]);
			keepHeap(pos * 2 + 1);
			return;
		}
		//�E�ւ�����
		if (largest_idx == pos * 2 + 2)
		{
			swap(data[pos], data[pos * 2 + 2]);
			keepHeap(pos * 2 + 2);
			return;
		}
		//����������Ă͂܂�Ȃ��Ƃ��A�q�̕����؂�����HEAP�Ƃ��Đ�������keepHeap�͂����ŏI���B
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
		//�lval��}��
		//�����max_heap���g�p�����ɁA���ʂ�while���Őe�����ǂ�Ȃ���AHEAP�����𖞂����悤��swap���Ă����B
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
		//���ƍŖ��[�̗t��swap���A�T�C�Y��1�����邱�Ƃō��̒l��pop�ł���B
		swap(data[0], data[data.size() - 1]);
		data.pop_back();
		//���Ƃ́AkeepHeap()��������{�������B
		keepHeap(0);
	}

	size_t size()
	{
		return data.size();
	}
};
