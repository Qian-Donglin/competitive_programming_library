struct BIT {
	vector<ll> bit;
	int N;
	
	BIT(vector<ll> data) {
		bit = data;
		N = bit.size() - 1;
	}

	//差分更新せよ！
	void update(int pos, ll x) {
		for (int i = pos; i <= N; i += (i & (-i)))
			bit[i] += x;
	}

	ll sum(int a) {
		//[1, a]の和を求める
		ll ret = 0;
		for(int i = a; i > 0; i -= (i & (-i)))
			ret += bit[i];
		return ret;
	}
	ll sum(int a, int b) {
		//[a, b]の和を求める
		return sum(b) - sum(a - 1);
	}
	ll lower_bound(ll w) {
		if (w <= 0)return 0;
		int a = 1;
		while (a < N)a <<= 1;
		a /= 2;
		if (a * 2 == N)a <<= 1;
		int x = 0;
		for (int k = a; k > 0; k >>= 1) {
			if (x + k <= N && w > bit[x + k]) {
				w -= bit[x + k];
				x += k;
			}
		}
		return x + 1;
	}
};

//BITは基本的に1-dix覚えておけよ
