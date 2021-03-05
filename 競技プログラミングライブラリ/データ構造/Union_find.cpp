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