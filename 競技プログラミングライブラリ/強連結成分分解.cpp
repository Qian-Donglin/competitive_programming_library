struct Union_Find {
	vector<int> rank, par;
	int independent_tree;
	int N;

	Union_Find() = default;

	Union_Find(int n) {
		N = n;
		for (int i = 0; i < N; i++)rank.push_back(1), par.push_back(i);
		independent_tree = N;
	}

	void setsize(int n) {
		N = n;
		for (int i = 0; i < N; i++)rank.push_back(1), par.push_back(i);
		independent_tree = N;
	}

	int getroot(int X) {
		if (par[X] == X)return X;
		return par[X] = getroot(par[X]);
	}

	bool issame(int A, int B) {
		return getroot(A) == getroot(B);
	}

	void unite(int A, int B) {
		A = getroot(A), B = getroot(B);
		if (A == B)return;
		independent_tree--;
		if (rank[A] > rank[B]) {
			par[B] = A;
		}
		else if (rank[A] == rank[B]) {
			par[B] = A;
			rank[A]++;
		}
		else {
			par[A] = B;
		}
	}

};

//強連結成分分解 O(V+E)　Union Findを使って一部の機能を実装してる
struct StronglyConnectedComponents {
	vector<vector<int>> E, rE;
	vector<int> belong;//belong[i] := i番目の頂点が属してる強連結成分の集合番号
	vector<int> order, used;
	int N;
	int anscnt = 1;//集合番号は1-idxで実装した
	Union_Find uf;

	StronglyConnectedComponents(vector<vector<int>> e) {
		E = e;
		N = E.size();
		rE.resize(N);
		used.resize(N);
		belong.resize(N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < E[i].size(); j++) {
				rE[E[i][j]].push_back(i);
			}
		}

		uf.setsize(N);
	}

	void dfs(int pos, int bef) {
		used[pos] = 1;
		for (int i = 0; i < E[pos].size(); i++) {
			int nxt = E[pos][i];
			if (nxt == bef)continue;
			if(!used[nxt])dfs(nxt, pos);
		}
		order.push_back(pos);
	}

	void rdfs(int pos, int bef) {
		belong[pos] = anscnt;
		if (bef != -1)uf.unite(pos, bef);
		for (int i = 0; i < rE[pos].size(); i++) {
			int nxt = rE[pos][i];
			if (nxt == bef || belong[nxt] != 0) {
				continue;
			}
			rdfs(nxt, pos);
		}
	}

	void conduct() {
		for (int i = 0; i < N; i++)
			if (used[i] == 0)dfs(i, -1);

		reverse(order.begin(), order.end());

		for (int i = 0; i < N; i++) {
			if (belong[order[i]] == 0)rdfs(order[i], -1), anscnt++;
		}
	}

	int operator[](int k) {
		return belong[k];
	}

	vector<vector<int>> trans_adjacency_list(){
		//i番目の今日連結グループの中身を返す。 例によって1-idxで
		vector<vector<int>> ret(anscnt);
		for (int i = 0; i < N; i++) {
			ret[belong[i]].push_back(i);
		}
		return ret;
	}

	vector<vector<int>> get_SCC_DAG(){
		//強連結成分を頂点としたグラフを作る。
		//強連結成分の番号はbelong[i]にある通りで1-idxで実装されてる
		//UFを使うことでO(N)でできた

		vector<vector<int>> ret;
		ret.resize(anscnt);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < E[i].size(); j++) {
				int nxt = E[i][j];
				if (!uf.issame(i, nxt)) {
					ret[belong[i]].push_back(belong[nxt]);
				}
			}
		}
		return ret;
	}
};
