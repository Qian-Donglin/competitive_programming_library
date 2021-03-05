#include <functional>
#include <vector>
#include <climits>

//operation1 -> nodeのデータ同士の統合の演算
template <class Monoid>
struct SegTree
{
	int N;
	vector<Monoid> node;
	std::function<Monoid(Monoid, Monoid)> operation;
	Monoid unit; //単位元

	std::function<bool(Monoid, Monoid)> comp;
	Monoid not_find_sgn;

	SegTree(vector<Monoid> &data, std::function<Monoid(Monoid, Monoid)> op, Monoid U)
	{
		//データ　演算　単位元
		operation = op;
		unit = U;
		int size = data.size();
		N = 1;
		while (N < size)
			N <<= 1;
		node.resize(2 * N - 1, unit);

		for (int i = 0; i < size; i++)
		{
			node[i + N - 1] = data[i];
		}

		for (int i = N - 2; i >= 0; i--)
		{
			node[i] = operation(node[i * 2 + 1], node[i * 2 + 2]);
		}
	}

	SegTree() = default;

	void update(int pos, Monoid val)
	{
		pos += N - 1;
		node[pos] = val;
		while (pos > 0)
		{
			pos = (pos - 1) / 2;
			node[pos] = operation(node[pos * 2 + 1], node[pos * 2 + 2]);
		}
	}

	Monoid getval(int a, int b, int idx, int l, int r = -1)
	{
		//[a, b)を求める。　使い方はgetval(a, b, 0, 0, -1);
		if (r == -1)
			r = N;
		if (b <= l || r <= a)
			return unit;
		if (a <= l && r <= b)
			return node[idx];
		Monoid left = getval(a, b, idx * 2 + 1, l, (l + r) / 2);
		Monoid right = getval(a, b, idx * 2 + 2, (l + r) / 2, r);
		return operation(left, right);
	}

	Monoid getval(int a, int b)
	{
		//ラッパー。
		return getval(a, b, 0, 0, -1);
	}
};

//遅延セグ木
/*
Monoid1 -> データの型
Monoid2 -> 遅延配列に入れる、Monoid1型のデータに作用する用のデータの型 (RSQならば、Monoid1, 2共にint型)
operation1 -> Monoid1(Monoid1, Monoid1) 保存されてるデータ同士の統合の演算子
operation2 -> Monoid2(Monoid2, Monoid2) 遅延配列に入ってるデータ同士の統合の演算子
operation3 -> Monoid1(Monoid1, Monoid2) 保存されてるデータに、遅延配列を作用させる演算子
operation4 -> Monoid2(Monoid2, int) 遅延配列の区間の長さに対応して、もともと割り当てられた遅延配列の中身を演算するときに使う演算子
RSQならば、上記の3つはすべて加算(と乗算)でまかなえる。operation4は、RSQならばカバーしてる区間倍することに相当する。
*/
template <class Monoid1, class Monoid2>
struct LazySegTree
{
	int N;
	vector<Monoid1> node;
	vector<Monoid2> lazy;
	std::function<Monoid1(Monoid1, Monoid1)> operation1;
	std::function<Monoid2(Monoid2, Monoid2)> operation2;
	std::function<Monoid1(Monoid1, Monoid2), int, int> operation3; //最後の2つはlとr
	std::function<Monoid2(Monoid2, int)> operation4;
	Monoid1 unit1; //Monoid1の単位元
	Monoid2 unit2; //Monoid2の単位元

	LazySegTree(vector<Monoid1> &data,
				std::function<Monoid1(Monoid1, Monoid1)> op1,
				std::function<Monoid2(Monoid2, Monoid2)> op2,
				std::function<Monoid1(Monoid1, Monoid2)> op3,
				std::function<Monoid2(Monoid2, int)> op4,
				Monoid1 U1, Monoid2 U2)
	{
		//データ　演算1~4　単位元1~2
		operation1 = op1;
		operation2 = op2;
		operation3 = op3;
		operation4 = op4;
		unit1 = U1;
		unit2 = U2;
		int size = data.size();
		N = 1;
		while (N < size)
			N <<= 1;
		node.resize(2 * N - 1, unit1);
		lazy.resize(2 * N - 1, unit2);

		for (int i = 0; i < size; i++)
		{
			node[i + N - 1] = data[i];
		}

		for (int i = N - 2; i >= 0; i--)
		{
			node[i] = operation1(node[i * 2 + 1], node[i * 2 + 2]);
		}
	}

	//番号kのデータを伝播させる。
	void eval(int k, int l, int r)
	{
		if (lazy[k] == unit2)
			return;
		node[k] = operation3(node[k], operation4(lazy[k], r - l));
		if (r - l > 1)
		{
			//子を持ってる
			lazy[2 * k + 1] = operation2(lazy[2 * k + 1], lazy[k]);
			lazy[2 * k + 2] = operation2(lazy[2 * k + 2], lazy[k]);
		}
		lazy[k] = unit2;
	}

	void update(Monoid2 val, int idx, int a, int b, int l = 0, int r = -1)
	{
		//[a, b)
		if (r < 0)
			r = N;
		eval(idx, l, r);
		//範囲外なら何もしない
		if (b <= l || r <= a)
			return;
		if (a <= l && r <= b)
		{
			//完全に被覆してるのなら、遅延配列を更新して評価。
			lazy[idx] = operation3(lazy[idx], val);
			eval(idx, l, r);
			return;
		}
		update(val, idx * 2 + 1, a, b, l, (l + r) / 2);
		update(val, idx * 2 + 2, a, b, (l + r) / 2, r);
		node[idx] = operation1(node[idx * 2 + 1], node[idx * 2 + 2]);
	}

	void update(Monoid2 val, int a, int b)
	{
		//[a, b)
		update(val, 0, a, b, 0, -1);
	}

	Monoid1 getval(int a, int b, int idx, int l, int r = -1)
	{
		eval(idx, l, r);
		//[a, b)を求める。　使い方はgetval(a, b, );
		if (r == -1)
			r = N;
		if (b <= l || r <= a)
			return unit1;
		if (a <= l && r <= b)
			return node[idx];
		Monoid1 left = getval(a, b, idx * 2 + 1, l, (l + r) / 2);
		Monoid1 right = getval(a, b, idx * 2 + 2, (l + r) / 2, r);
		return operation1(left, right);
	}

	Monoid1 getval(int a, int b)
	{
		//ラッパー。
		return getval(a, b, 0, 0, -1);
	}
};

//operation1 -> nodeのデータ同士の統合の演算
template <class Monoid>
struct SegTree_2d
{
	int hN, wN;
	vector<SegTree<Monoid>> node;
	std::function<Monoid(Monoid, Monoid)> operation;
	Monoid unit; //単位元

	std::function<bool(Monoid, Monoid)> comp;
	Monoid not_find_sgn;

	SegTree<Monoid> unite_seg_unit(SegTree<Monoid> &l, SegTree<Monoid> &r)
	{
		//2つのサイズが同じのセグメント木を統合する
		vector<Monoid> tmp(l.datasize, unit);
		SegTree<Monoid> ret(tmp, operation, unit);
		for (int i = 0; i < l.datasize; i++)
		{
			ret.node[i + l.N - 1] = operation(l.node[i + l.N - 1], r.node[i + l.N - 1]);
		}

		//更新していく
		for (int i = l.N - 2; i >= 0; i--)
		{
			ret.node[i] = operation(ret.node[i * 2 + 1], ret.node[i * 2 + 2]);
		}
		return ret;
	}

	SegTree_2d(vector<vector<Monoid>> &data, std::function<Monoid(Monoid, Monoid)> op, Monoid U)
	{
		//データ　演算　単位元
		operation = op;
		unit = U;
		int h = data.size(), w = data[0].size();
		hN = 1, wN = 1;
		while (hN < h)
			hN <<= 1;
		while (wN < w)
			wN <<= 1;

		node.resize(2 * hN - 1);
		for (int i = 0; i < 2 * hN - 1; i++)
		{
			vector<Monoid> tmp(wN, U);
			node[i] = SegTree<Monoid>(tmp, operation, U);
		}
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				node[i + hN - 1].node[j + wN - 1] = data[i][j];
			}
			for (int j = wN - 2; j >= 0; j--)
			{
				node[i + hN - 1].node[j] = operation(node[i + hN - 1].node[j * 2 + 1], node[i + hN - 1].node[j * 2 + 2]);
			}
		}

		for (int i = hN - 2; i >= 0; i--)
		{
			node[i] = unite_seg_unit(node[i * 2 + 1], node[i * 2 + 2]);
		}
	}

	void update(int a, int b, Monoid val)
	{
		//[0, h) * [0, w)の中での(a, b)を更新する
		a += hN - 1, b += wN - 1;
		node[a][b] = val;
		while (b > 0)
		{
			b = (b - 1) / 2;
			node[a][b] = operation(node[a][b * 2 + 1], node[a][b * 2 + 2]);
		}
		while (a > 0)
		{
			a = (a - 1) / 2;
			node[a] = unite_seg_unit(node[a * 2 + 1], node[a * 2 + 2]);
		}
	}

	Monoid getval_from_data(SegTree<Monoid> &segunit, int a, int b, int idx, int l, int r = -1)
	{
		//[a, b)を求める。　セグ木のノードのセグ木の求値関数　基本的にこれを単体で呼ばない
		if (r == -1)
			r = wN;
		if (b <= l || r <= a)
			return unit;
		if (a <= l && r <= b)
			return segunit.node[idx];
		Monoid left = getval_from_data(segunit, a, b, idx * 2 + 1, l, (l + r) / 2);
		Monoid right = getval_from_data(segunit, a, b, idx * 2 + 2, (l + r) / 2, r);
		return operation(left, right);
	}

	Monoid getval_from_unit(int a1, int b1, int a2, int b2, int idx, int l, int r = -1)
	{
		if (r == -1)
			r = hN;
		if (a2 <= l || r <= a1)
			return unit;
		if (a1 <= l && r <= a2)
		{
			return getval_from_data(node[idx], b1, b2, 0, 0, -1);
		}
		Monoid left = getval_from_unit(a1, b1, a2, b2, idx * 2 + 1, l, (l + r) / 2);
		Monoid right = getval_from_unit(a1, b1, a2, b2, idx * 2 + 2, (l + r) / 2, r);
		return operation(left, right);
	}

	Monoid getval(int a1, int b1, int a2, int b2)
	{
		//ラッパー。
		return getval_from_unit(a1, b1, a2, b2, 0, 0, -1);
	}
};
