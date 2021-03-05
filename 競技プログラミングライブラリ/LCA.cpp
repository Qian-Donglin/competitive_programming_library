//STL: <functional>
template <class Monoid> struct SegTree {
	int N;
	vector<Monoid> node;
	std::function<Monoid(Monoid, Monoid)> operation;
	Monoid unit;//単位元

	std::function<bool(Monoid, Monoid)> comp;
	Monoid not_find_sgn;

	SegTree() = default;

	SegTree(vector<Monoid>& data, std::function<Monoid(Monoid, Monoid)> op, Monoid U) {
		//データ　演算　単位元
		operation = op;
		unit = U;
		int size = data.size();
		N = 1;
		while (N < size)N <<= 1;
		node.resize(2 * N - 1, unit);

		for (int i = 0; i < size; i++) {
			node[i + N - 1] = data[i];
		}

		for (int i = N - 2; i >= 0; i--) {
			node[i] = operation(node[i * 2 + 1], node[i * 2 + 2]);
		}
	}

	void update(int pos, Monoid val) {
		pos += N - 1;
		node[pos] = val;
		while (pos > 0) {
			pos = (pos - 1) / 2;
			node[pos] = operation(node[pos * 2 + 1], node[pos * 2 + 2]);
		}
	}

	Monoid getval(int a, int b, int idx, int l, int r = -1) {
		//[a, b)を求める。　使い方はgetval(a, b, );
		if (r == -1)r = N;
		if (b <= l || r <= a)return unit;
		if (a <= l && r <= b)return node[idx];
		Monoid left = getval(a, b, idx * 2 + 1, l, (l + r) / 2);
		Monoid right = getval(a, b, idx * 2 + 2, (l + r) / 2, r);
		return operation(left, right);
	}

	Monoid getval(int a, int b) {
		//ラッパー。
		return getval(a, b, 0, 0, -1);
	}

};

//STL:<tuple> <algorithm>  MyLibrary: "SegTree" 
struct LCA {
	SegTree<pair<int, int>> rmq;
	vector<int> dfs_tour_order;
	vector<pair<int, int>> visited_order;//pos, depth

	void dfs(vector<vector<int>>& E, int pos, int beforepos, int depth) {
		if (dfs_tour_order[pos] == -1)dfs_tour_order[pos] = visited_order.size();
		visited_order.push_back(make_pair(pos, depth));
		for (int i = 0; i < E[pos].size(); i++) {
			int nxt = E[pos][i];
			if (nxt == beforepos)continue;
			dfs(E, nxt, pos, depth + 1);
			visited_order.push_back(make_pair(pos, depth));
		}
	}

	LCA(vector<vector<int>>& E, int root) {
		//基本的に0-idxで
		dfs_tour_order.resize(E.size(), -1);//初めてその頂点を尋ねた際、何番目の
		
		dfs(E, root, -1, 0);

		rmq = SegTree<pair<int, int>>(visited_order,
			[](pair<int, int> a, pair<int, int> b) {
			if (a.second != b.second) return a.second > b.second ? b : a; 
			return a.first > b.first ? b : a;//ここは逆の順番にしてもいい。LCAは最終的には一位に定まり、aとbのsecondによってユニークのため
		}
		, make_pair(0, E.size()));
	}
	
	pair<int, int> lca(int u, int v) {
		//LCAになってる頂点の番号と、根から見た深さを返す
		if (dfs_tour_order[u] > dfs_tour_order[v])swap(u, v);
		return rmq.getval(dfs_tour_order[u], dfs_tour_order[v] + 1);
	}

	int dis(int u, int v) {
		//u->vへの最短距離
		int root_to_u = rmq.getval(dfs_tour_order[u], dfs_tour_order[u] + 1).second;
		int root_to_v = rmq.getval(dfs_tour_order[v], dfs_tour_order[v] + 1).second;
		return root_to_u + root_to_v - 2 * lca(u, v).second;
	}

};
