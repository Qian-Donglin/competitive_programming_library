namespace v2d {

	struct Vec2 {
		//2次元ベクトルのクラス

		double x, y;
		Vec2(double _x, double _y) {
			x = _x, y = _y;
		}
		Vec2() {
			x = 0, y = 0;
		}

		Vec2 operator+() const {
			return *this;
		}
		Vec2 operator-() const {
			return{ -x, -y };
		}
		Vec2 operator+ (const Vec2& b) const {
			return{ x + b.x, y + b.y };
		}
		Vec2 operator- (const Vec2& b) const {
			return{ x - b.x, y - b.y };
		}
		Vec2 operator* (const double b) const {
			return{ x * b, y * b };
		}
		Vec2 operator/ (const double b) const {
			return{ x / b, y / b };
		}
		Vec2 operator+= (const Vec2& b) {
			x += b.x, y += b.y;
			return *this;
		}
		Vec2 operator-= (const Vec2& b) {
			x -= b.x, y -= b.y;
			return *this;
		}
		Vec2 operator*= (const double b) {
			x *= b, y *= b;
			return *this;
		}
		Vec2 operator/= (const double b) {
			x /= b, y /= b;
			return *this;
		}
		bool operator== (const Vec2& b) {
			return b.x == x && b.y == y;
		}

		double lengthSquare() {
			return (x * x + y * y);
		}
		double length() {
			return std::sqrt(lengthSquare());
		}
		double dot(const Vec2& b) {
			return x * b.x + y * b.y;
		}
		double cross(const Vec2& b) {
			//Generally, cross product is vector, but in 2D, cross product is also scalar.
			return x * b.y - y * b.x;
		}
		double distanceFrom(const Vec2& b) {
			return std::sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
		}
		Vec2 unit() {
			//単位ベクトル
			return{ x / length(), y / length() };
		}
		Vec2 normal() {
			//今のベクトルを反時計回りに90度回した法線ベクトルを返す。
			return{-y, x};
		}
		bool isZero() {
			return x == 0.0 && y == 0.0;
		}
	};

	inline Vec2 operator*(double a, const Vec2& b) {
		return{ b.x * a, b.y * a };
	}

	template <class Char>
	inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const Vec2& v)
	{
		return os << Char('(') << v.x << Char(',') << v.y << Char(')');
	}

	template <class Char>
	inline std::basic_istream<Char>& operator >> (std::basic_istream<Char>& is, Vec2& v)
	{
		return is >> v.x >> v.y;
	}

}

using namespace v2d;

namespace convex_h {

	bool comp_x(const v2d::Vec2& p, const v2d::Vec2& q) {
		if (p.x != q.x)return p.x < q.x;
		return p.y < q.y;
	}
	bool comp_y(const v2d::Vec2& p, const v2d::Vec2& q) {
		if (p.y != q.y)return p.y < q.y;
		return p.x < q.x;
	}

	struct convex_hull {

		const double EPS = 1e-10;

		vector<int> ans;
		vector<v2d::Vec2> candidate;
		vector<pair<v2d::Vec2, int>> taiou;

		void calc(bool is_cover = true) {
			for (int i = 0; i < candidate.size(); i++) {
				taiou.push_back(make_pair(candidate[i], i));
			}
			sort(taiou.begin(), taiou.end(), [](pair<v2d::Vec2, int> &a, pair<v2d::Vec2, int> &b) {
				return comp_x(a.first, b.first);
			});
			int k = 0;
			ans.resize(2 * taiou.size());
			for (int i = 0; i < taiou.size(); i++) {
				while (k > 1 && (is_cover && ((candidate[ans[k - 1]] - candidate[ans[k - 2]]).
					cross(taiou[i].first - candidate[ans[k - 1]]) <= 0) ||
					(!is_cover && (candidate[ans[k - 1]] - candidate[ans[k - 2]]).
						cross(taiou[i].first - candidate[ans[k - 1]]) < 0)))k--;
				ans[k++] = taiou[i].second;
			}

			for (int i = taiou.size() - 2, t = k; i >= 0; i--) {
				while (k > t && (is_cover && (candidate[ans[k - 1]] - candidate[ans[k - 2]]).
					cross(taiou[i].first - candidate[ans[k - 1]]) <= 0) ||
					((!is_cover && (candidate[ans[k - 1]] - candidate[ans[k - 2]]).
						cross(taiou[i].first - candidate[ans[k - 1]]) < 0)))k--;
				ans[k++] = taiou[i].second;
			}
			ans.resize(k - 1);
		}

		double farthest_dis() {
			//Using effective algorithm, 
			//this function can calculate farthest distance of all point in convex hull int O(n).
			//This algorithm name is Rotating Calipers, according to 蟻本.

			double res = 0;
			int n = ans.size();
			if (ans.size() == 2) {
				return candidate[ans[0]].distanceFrom(candidate[ans[1]]);
			}
			int i = 0, j = 0;
			for (int k = 0; k < n; k++) {
				if (!comp_x(candidate[ans[i]], candidate[ans[k]]))i = k;
				if (comp_x(candidate[ans[j]], candidate[ans[k]]))j = k;
			}
			int si = i, sj = j;
			while (i != sj || j != si) {
				res = max(res, candidate[ans[i]].distanceFrom(candidate[ans[j]]));
				int nxti = (i + 1) % n, nxtj = (j + 1) % n;

				if ((candidate[ans[nxti]] - candidate[ans[i]]).cross(candidate[ans[nxtj]] - candidate[ans[j]]) < 0) {
					i = (i + 1) % n;
				}
				else {
					j = (j + 1) % n;
				}
			}
			return res;
		}

		double hull_distance() {
			double ret = candidate[ans[0]].distanceFrom(candidate[ans.back()]);
			for (int i = 1; i < ans.size(); i++) {
				ret += candidate[ans[i]].distanceFrom(candidate[ans[i - 1]]);
			}
			return ret;
		}

	};

}