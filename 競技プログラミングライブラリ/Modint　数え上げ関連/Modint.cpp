//このnamespace内のMODという値を、問題の要求に応じて変更すること
namespace Mint
{

	//このMODという値を、問題の要求に応じて変更すること
	const ll MOD = 998244353;

	template <ll Mod>
	struct Modint
	{
		ll val = 0;

		//コンストラクタ long long, 空, Modintを受け取れる
		Modint() = default;
		Modint(const Modint &) = default;
		Modint(ll _x)
		{
			val = _x >= 0 ? _x % Mod : ((_x % Mod) + Mod) % Mod;
		}

		//繰り返し二乗法、逆元　基本的に外部からいじるのやめたほうがよさそう。
		ll modpow(ll a, ll b) const
		{
			ll ret = 1, kakeru = a;
			while (b > 0)
			{
				if (b & 1)
					ret *= kakeru, ret %= Mod;
				kakeru *= kakeru, kakeru %= Mod;
				b >>= 1;
			}
			return ret;
		}
		Modint inv() const
		{
			return modpow((*this).val, MOD - 2);
		}

		//代入演算子　Modintとlong longの2通りある
		Modint operator=(const Modint &p)
		{
			val = p.val;
			return (*this);
		}

		//二項演算＋代入演算子　二項演算子、同値演算子はクラス外で定義する
		Modint &operator+=(const Modint &p)
		{
			val += p.val;
			if (val >= Mod)
				val -= Mod;
			return (*this);
		}
		Modint &operator-=(const Modint &p)
		{
			val -= p.val;
			if (val < 0)
				val += Mod;
			return (*this);
		}
		Modint &operator*=(const Modint &p)
		{
			val *= p.val;
			val %= Mod;
			return (*this);
		}
		Modint &operator/=(const Modint &p)
		{
			//なんか,p.inv()を使うとthisのポインターが変換できませんって出る
			//Modint tmp(p.inv());
			Modint tmp(modpow(p.val, MOD - 2));
			(*this) *= tmp;
			return (*this);
		}
	};

	//加算
	const Modint<MOD> operator+(const Modint<MOD> &l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp += r;
		return tmp;
	}
	const Modint<MOD> operator+(const Modint<MOD> &l, const ll r)
	{
		Modint<MOD> tmp = l;
		tmp += Modint<MOD>(r);
		return tmp;
	}
	const Modint<MOD> operator+(const ll l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp += r;
		return tmp;
	}
	//減算
	const Modint<MOD> operator-(const Modint<MOD> &l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp -= r;
		return tmp;
	}
	const Modint<MOD> operator-(const Modint<MOD> &l, const ll r)
	{
		Modint<MOD> tmp = l;
		tmp -= Modint<MOD>(r);
		return tmp;
	}
	const Modint<MOD> operator-(const ll l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp -= r;
		return tmp;
	}
	//乗算
	const Modint<MOD> operator*(const Modint<MOD> &l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp *= r;
		return tmp;
	}
	const Modint<MOD> operator*(const Modint<MOD> &l, const ll r)
	{
		Modint<MOD> tmp = l;
		tmp *= Modint<MOD>(r);
		return tmp;
	}
	const Modint<MOD> operator*(const ll l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp *= r;
		return tmp;
	}
	//除算
	const Modint<MOD> operator/(const Modint<MOD> &l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp /= r;
		return tmp;
	}
	const Modint<MOD> operator/(const Modint<MOD> &l, const ll r)
	{
		Modint<MOD> tmp = l;
		tmp /= Modint<MOD>(r);
		return tmp;
	}
	const Modint<MOD> operator/(const ll l, const Modint<MOD> &r)
	{
		Modint<MOD> tmp = l;
		tmp /= r;
		return tmp;
	}
	//同値演算子
	const bool operator==(const Modint<MOD> &l, const Modint<MOD> &r)
	{
		return l.val == r.val;
	}
	const bool operator==(const Modint<MOD> &l, const ll r)
	{
		return l.val == r;
	}
	const bool operator==(const ll l, const Modint<MOD> &r)
	{
		return l == r.val;
	}
	const bool operator!=(const Modint<MOD> &l, const Modint<MOD> &r)
	{
		return !(l.val == r.val);
	}
	const bool operator!=(const Modint<MOD> &l, const ll r)
	{
		return !(l.val == r);
	}
	const bool operator!=(const ll l, const Modint<MOD> &r)
	{
		return !(l == r.val);
	}
	//istream ostream での入出力サポート
	std::ostream &operator<<(std::ostream &stream, const Modint<MOD> &p)
	{
		stream << p.val;
		return stream;
	}
	std::istream &operator>>(std::istream &stream, Modint<MOD> &p)
	{
		stream >> p.val;
		return stream;
	}
	//使う用の繰り返し二乗法 bはlong long に注意
	Modint<MOD> modpow(const Modint<MOD> a, ll b)
	{
		ll ret = 1, kakeru = a.val;
		while (b > 0)
		{
			if (b & 1)
				ret *= kakeru, ret %= MOD;
			kakeru *= kakeru, kakeru %= MOD;
			b >>= 1;
		}
		Modint<MOD> tmpret(ret);
		return tmpret;
	}
} // namespace Mint

using namespace Mint;
using mint = Mint::Modint<MOD>;
