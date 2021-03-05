struct cum_sum {
	vector<ll> ar;
	vector<ll> sum;
	int N;
	cum_sum(int _N) {
		N = _N;
		ar.resize(N), sum.resize(N + 1);
	}
	cum_sum(vector<ll>& _ar) {
		ar = _ar;
	}

	ll& operator[](int _idx) {
		return ar[_idx];
	}

	void setup() {
		for (int i = 0; i < N; i++) {
			sum[i + 1] = sum[i] + ar[i];
		}
	}
	ll getval(int l, int r) {
		//[l, r)
		ll ret = sum[r] - sum[l];
		return ret;
	}
};

struct cum_sum_lm {
	vector<ll> sum;
	int N;
	cum_sum_lm() = default;
	cum_sum_lm(int _N) {
		N = _N;
		sum.resize(N + 1);
	}

	void setup(vector<ll> & _ar) {
		for (int i = 0; i < N; i++) {
			sum[i + 1] = sum[i] + _ar[i];
		}
	}
	ll getval(int l, int r) {
		//[l, r)
		ll ret = sum[r] - sum[l];
		return ret;
	}
};