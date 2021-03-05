//中国剰余定理で存在を保証されている値の計算。
//連立合同式を解く。

namespace CRT {

	//負も考慮したmod [0, mod - 1]
	ll positive_mod(const ll a, const ll mod) {
		ll ret = a % mod;
		if (ret < 0)ret += mod;
		return ret;
	}

	ll gcd(ll a, ll b) {
		if (a % b == 0)return b;
		return gcd(b, a % b);
	}

	ll lcm(ll a, ll b) {
		return a * b / gcd(a, b);
	}

	//e_gcd()自体はaとbの最大公約数を返す。
	//xとyは、a*x+b*y=gcd(a,b)となる(x,y)の1つの組を返す。
	//a*x+b*y=gcdの時、a=b*p+(a%b)を代入して、b*p*x+(a%b)*x+b*y=b*(p*x+y)+(a%b)*x
	//次のx->p*x+y
	//次のy->x
	//次のa->b
	//次のb->a%b
	ll e_gcd(ll a, ll b, ll& x, ll& y)
	{
		if (b == 0)
		{
			x = 1;
			y = 0;
			return a;
		}
		ll gcd_res = e_gcd(b, a % b, x, y);
		ll p = a / b;
		ll nx = x, ny = y;
		x = ny;
		y = nx - p * x;
		return gcd_res;
	}

	//拡張GCDを複数回使用することで得られる復元方法。
	//n本の式があったとき、O(n log m)
	//ただし、多倍長対策はしていない。

	//合同式を連立させたときの中国剰余定理で保障された解のうちの1つXを返す。
	//(返すのは求めたx, lcm)
	//解なしの場合は、(-1, -1)と返す。
	//オーバーフローする危険がある。かなり大きくなるかつ、任意modが欲しいならGarner's Algorithmを使うこと。
	std::pair<ll, ll> CRT(const vector<ll>& val, const vector<ll>& mod) {
		//O(n log(max{mod[i]}))
		//x == val[i] (mod[i])がn本ある感じ
		if (val.size() != mod.size())return make_pair(-1, -1);
		int n = val.size();
		ll x = 0;//最初は0 (mod 1)で始める。

		ll mlcm = 1;

		for (int i = 0; i < n; i++) {
			//m[i] % mgcdが等しくない場合は、解がない。

			ll p, q;
			ll mgcd = e_gcd(mlcm, mod[i], p, q);//pとqの符号は逆になる
			if ((val[i] - x) % mgcd != 0) {
				return make_pair(-1, -1);
			}

			//実際のpを求め(e_gcdのは右辺がgcd(mlcm, mod[i])でval[i]-xと定数倍の差がある。
			//その差を補正して、mod[i] / mgcdであまりを取ると正で一番小さいpを得られる。
			//pは常に正でqは常に負として考える。
			ll min_p = positive_mod((p * (val[i] - x) / mgcd), (mod[i] / mgcd));
			x += min_p * mlcm;//これはpでの計算

			//理論上、以下のようにqで求めてもいい。
			//しかし、実際はオーバーフローする。
			//ll max_q = (q * (val[i] - x) / mgcd) % (mlcm / mgcd);
			//if(max_q > 0)max_q -= mlcm / mgcd;
			//x = val[i] - max_q * mod[i];

			mlcm = mlcm * (mod[i] / mgcd);
		}
		x %= mlcm;
		return make_pair(x, mlcm);
	}

	//ユークリッドの互除法から逆元を求める関数。解なしなら-1を返す。
	ll invModGCD(const ll val, const ll mod) {
		ll p, q;
		if (e_gcd(val, mod, p, q) != 1)
			return -1;
		return positive_mod(p, mod);
	}

	//Garner's Algorithmに適用させるために、mod同士が互いに素になるようにする。
	//Garnerのアルゴリズムの前に実行するべき(というか、Garner()でWrapして使うことになると思う)
	//そもそも与えられた式が解なしなら、false、それ以外ならtrueを返す。
	bool preGarner(vector<ll>& val, vector<ll>& mod) {
		//計算量O(n^2 * log(max{mod[i]}))
		int n = val.size();

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < i; j++) {
				ll g = gcd(mod[i], mod[j]);
				if ((val[j] - val[i]) % g != 0)
					return false;

				//ここの部分はブログで詳しく例を挙げて紹介している。
				mod[i] /= g, mod[j] /= g;
				ll gi = gcd(mod[i], g);
				ll gj = g / gi;

				//ちなみにこのループは一度では終わらない。
				do{
					ll g = gcd(gi, gj);
					gi *= g, gj /= g;
				} while (gcd(gi, gj) != 1);

				mod[i] *= gi, mod[j] *= gj;

				//val[i], val[j]は小さくなったmod[i], mod[j]に合わせて再計算する。
				val[i] %= mod[i], val[j] %= mod[j];

			}
		}
		return true;
	}

	//Garner's Algorithmの本体。
	//計算量はO(n^2 * (log(max{mod[i]})))
	//普通の拡張ユークリッドでの復元と比べてnの次元が1つ上がってる。
	ll GarnerBody(const vector<ll>& val, const vector<ll>& mod, const ll MOD) {
		int n = val.size();

		ll x_MOD = val[0];//MODで割れれてるもの
		ll m_mul_MOD = 1;
		ll lcm_MOD = val[0];
		vector<ll> t(n);//MODで割られてないもの t[0]は使わない

		for (int i = 1; i < n; i++) {
			//mod mod[i]でのt[i]を求める。t[i]

			ll x_mod = val[0] % mod[i];
			ll m_mul_mod = 1;
			//現時点のx_MODの値をx_modにするためには、線形時間で再計算するしかない。
			for (int j = 1; j < i; j++) {
				m_mul_mod *= mod[j - 1], m_mul_mod %= mod[i];
				x_mod += m_mul_mod * t[j], x_mod%= mod[i];
			}

			ll m_inv_mod = 1;
			for (int j = 0; j < i; j++) {
				m_inv_mod *= invModGCD(mod[j], mod[i]), m_inv_mod %= mod[i];
			}

			ll val_minus_x_mod = positive_mod(val[i] - x_mod, mod[i]);

			t[i] = val_minus_x_mod * m_inv_mod, t[i] %= mod[i];

			m_mul_MOD *= mod[i - 1], m_mul_MOD %= MOD;
			//x_MODは累積和的に更新できる。
			x_MOD += t[i] * m_mul_MOD, x_MOD %= MOD;

		}

		return x_MOD;
	}

	//Garner's Algorithmのラッパー関数　これを呼んで使えば間違いない。
	//参照渡ししたvectorは変わりうる(modが互いに素じゃないとき)ので注意
	//(連立合同式の解, LCM) ただし、すべて%MODされている。
	//解なしの時、CRTと同様に(-1, -1)を返す。
	pair<ll, ll> Garner(vector<ll>& val, vector<ll>& mod, const ll MOD) {
		if (val.size() != mod.size())
			return make_pair(-1, -1);
		if (!preGarner(val, mod))
			return make_pair(-1, -1);
		ll lcm = 1;
		for (int i = 0; i < mod.size(); i++)
			lcm *= mod[i], lcm %= MOD;

		return make_pair(GarnerBody(val, mod, MOD), lcm);
	}

};
