namespace geometry2d {

	//eps　浮動小数点誤差
	const double EPS = 1e-10;

	//a > 0ならば+1, a == 0ならば0, a < 0ならば-1　を返す。　基本的にEPS込みの評価はこれで行う。
	//不等式は、加減算に直してこれに適用する。
	int sgn(const double a) {
		return (a < -EPS ? -1 : (a > EPS ? +1 : 0));
	}

	struct Point {
		//2次元ベクトルのクラス

		double x, y;
		Point(double _x, double _y) {
			x = _x, y = _y;
		}
		Point() {
			x = 0, y = 0;
		}

		Point operator+() const {
			return *this;
		}
		Point operator-() const {
			return{ -x, -y };
		}
		Point operator+ (const Point& b) const {
			return{ x + b.x, y + b.y };
		}
		Point operator- (const Point& b) const {
			return{ x - b.x, y - b.y };
		}
		Point operator* (const double b) const {
			return{ x * b, y * b };
		}
		Point operator/ (const double b) const {
			return{ x / b, y / b };
		}
		Point operator+= (const Point& b) {
			x += b.x, y += b.y;
			return *this;
		}
		Point operator-= (const Point& b) {
			x -= b.x, y -= b.y;
			return *this;
		}
		Point operator*= (const double b) {
			x *= b, y *= b;
			return *this;
		}
		Point operator/= (const double b) {
			x /= b, y /= b;
			return *this;
		}
		bool operator== (const Point& b) {
			return b.x == x && b.y == y;
		}

		double lengthSquare() const {
			return (x * x + y * y);
		}
		double length() const {
			return std::sqrt(lengthSquare());
		}
		double dot(const Point& b) const {
			return x * b.x + y * b.y;
		}
		double cross(const Point& b) const {
			//Generally, cross product is vector, but in 2D, cross product is also scalar.
			return x * b.y - y * b.x;
		}
		double distanceFrom(const Point& b) const {
			return std::sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
		}
		Point normalized() const {
			return{ x / length(), y / length() };
		}
		bool isZero() const {
			return sgn(x) == 0 && sgn(y) == 0;
		}
		//単位法線ベクトル
		Point normalUnitVector() const {
			return{ -normalized().y, normalized().x };
		}
		//(0, 0)中心にarg(弧度法)回転した座標
		Point rotation(double arg) const {
			double cs = cos(arg), sn = sin(arg);
			return Point(x * cs - y * sn, x * sn + y * cs);
		}
		//(0, 0)中心の円上に乗ってるとしたときの、偏角
		double angle() const {
			return atan2(y, x);
		}
	};

	inline Point operator*(double a, const Point& b) {
		return{ b.x * a, b.y * a };
	}

	template <class Char>
	inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const Point& v)
	{
		return os << Char('(') << v.x << Char(',') << v.y << Char(')');
	}

	template <class Char>
	inline std::basic_istream<Char>& operator >> (std::basic_istream<Char>& is, Point& v)
	{
		return is >> v.x >> v.y;
	}

	//幾何ライブラリにおいて、解なしのような状態の時に返される点の値
	const Point error_val = { 114514.0, -191981.0 };

	inline bool operator==(const Point& a, const Point& b) {
		return (sgn(a.x - b.x) == 0 && sgn(a.y - b.y) == 0);
	}

	inline bool operator!=(const Point& a, const Point& b) {
		return !(a == b);
	}

	//第一にx, 第二にyを小さい順にソートするためのオーバーロード
	inline bool operator<(const Point& a, const Point& b) {
		if (sgn(a.x - b.x) != 0)return sgn(a.x - b.x) < 0;
		else return sgn(a.y - b.y) < 0;
	}

	/*
	3点A, B, Cの位置関係を返す関数 A, Bがすべて異なった点であるのが前提
	//ABから見てBCは左に曲がるのなら +1
	//ABから見てBCは右に曲がるのなら -1
	//ABC(CBA)の順番で一直線上に並ぶなら +2
	//ACB(BCA)の順番で一直線上に並ぶなら 0
	BAC(CAB)の順番で一直線上に並ぶなら -2
	*/
	int iSP(const Point& a, const Point& b, const Point& c) {
		int flg = sgn((b - a).cross(c - a));
		if (flg == 1) {
			return +1;
		}
		else if (flg == -1) {
			return -1;
		}
		else {
			//ABC(CBA)
			if (sgn((b - a).dot(c - b)) > 0)
				return +2;
			//BAC(CAB)
			else if (sgn((a - b).dot(c - a)) > 0)
				return -2;
			//ACB(BCA)　CがA or Bと一致しても、こっちに含まれる。
			else
				return 0;
		}
	}

	//角ABCが鋭角なら0、直角なら1、鈍角なら2を返す。
	int angletype(const Point& a, const Point& b, const Point& c) {
		auto v = (a - b).dot(c - b);
		if (sgn(v) > 0)return 0;
		else if (sgn(v) == 0)return 1;
		else return 2;
	}

	//直線に関する操作や判定
	namespace line2d {

		//向きつき直線を想定。Segment
		struct Line {

			//直線の通る二点。有向直線でないのならば、beginやendに違いはない。
			Point begin, end;

			Line() {
				begin = Point(), end = Point();
			}

			Line(const Point& b, const Point& e) {
				begin = b, end = e;
			}

			//ax+by+c=0
			Line(const double a, const double b, const double c) {
				if (sgn(a) == 0 && sgn(b) == 0) {
					assert(-1);
				}

				if (sgn(b) == 0) {
					//ax+c=0になる。
					begin = Point(-c / a, 0.0);
					end = Point(-c / a, 1.0);
				}
				else {
					//y=-(ax+c)/b　傾きは-a/bで、y切片が-c/b
					begin = Point(0, -c / b);
					end = Point(1.0, -(a + c) / b);
				}
			}

			Point vec() const {
				return end - begin;
			}

			Point countervec() const {
				return begin - end;
			}

		};

		//半直線なら、Lineと同じだけど、はっきりと区別する。
		typedef Line Ray;
		//線分の場合、Lineと同じものになるが、はっきりと区別する
		typedef Line Segment;


		//直線の交点を返す。交わってなければ、error_valを返す。
		Point lineIntersection(const Line& l1, const Line& l2) {
			if (sgn(l1.vec().cross(l2.vec())) == 0)return error_val;

			Point ret;
			ret = l1.begin + l1.vec() *
				abs((l2.end - l1.begin).cross(l2.vec()) / l1.vec().cross(l2.vec()));
			return ret;
		}

		//線分が共通部分を持つかどうか？と線分の交点を返す。共通部分がない、もしくは交点が一意ではないなら、error_valを返す。
		//trueなら、共通部分を持つ。falseなら、共通部分を持たない。
		pair<bool, Point> segmentIntersection(const Segment& s1, const Segment& s2) {

			if (iSP(s1.begin, s1.end, s2.begin) * iSP(s1.begin, s1.end, s2.end) <= 0 &&
				iSP(s2.begin, s2.end, s1.begin) * iSP(s2.begin, s2.end, s1.end) <= 0) {

				//平行ならば、交点は定まらない。(完全に重なってるので)
				if (s1.vec().cross(s2.vec()) == 0)
					return make_pair(true, error_val);
				else //そうでないのなら、lineIntersection()で交点を返しておく。
					return make_pair(true, lineIntersection(s1, s2));

			}
			return make_pair(false, error_val);
		}


		//点と直線の距離。引数は、点、直線上の2点
		double distanceBetweenPointAndLine(const Point& p, const Line& l) {
			return abs(l.vec().cross(p - l.begin) / l.vec().length());
		}

		//点と半直線の距離。引数は、点、半直線(始点から終点方向に延びる)
		double distanceBetweenPointAndRay(const Point& p, const Ray& r) {
			//始点との距離のパターン
			if (sgn((p - r.begin).dot(r.vec())) < 0)
				return r.begin.distanceFrom(p);
			return abs(r.vec().cross(p - r.begin) / r.vec().length());
		}

		//点と線分の距離。引数は、点、線分の両端
		double distanceBetweenPointAndSegment(const Point& p, const Segment& s) {
			if (sgn(s.vec().dot(p - s.begin)) < 0 || sgn(s.countervec().dot(p - s.end)) < 0) {
				//下した垂線は線分の上にはない
				return min(p.distanceFrom(s.begin), p.distanceFrom(s.end));
			}
			return distanceBetweenPointAndLine(p, s);
		}

		//二線分間の距離
		double distanceBetweenSegmentAndSegment(const Segment& s1, const Segment& s2) {
			if (segmentIntersection(s1, s2).first)return 0;//交点を持つ

			double ans = distanceBetweenPointAndSegment(s1.begin, s2);
			ans = min(ans, distanceBetweenPointAndSegment(s1.end, s2));
			ans = min(ans, distanceBetweenPointAndSegment(s2.begin, s1));
			ans = min(ans, distanceBetweenPointAndSegment(s2.end, s1));
			return ans;
		}

		//正射影
		//引数は点A, B, Cで、Aの直線BC上の正射影を求める。
		Point projection(const Point& a, const Line& l) {
			Point ret;
			ret = l.begin +
				l.vec().normalized() * (a - l.begin).dot(l.vec()) / l.vec().length();
			return ret;
		}

		//鏡映変換。引数は点A, B, Cで、直線BCにおいて、Aと線対称な点を求める。
		Point reflection(const Point& a, const Line& l) {
			Point ret;
			ret = a + 2 * (projection(a, l) - a);
			return ret;
		}
	}

	namespace circle2d {

		struct Circle {
			Point o;
			double r;

			Circle() {
				o = Point();
				r = 0;
			}

			Circle(const Point& C, const double& R) {
				o = C, r = R;
			}
		};

		//直線と円の交点を返す。
		//存在しないのならば、空のvectorになる。
		vector<Point> intersectionCircleAndLine(const Circle& c, const line2d::Line& l) {
			//交点を持たない
			if (sgn(line2d::distanceBetweenPointAndLine(c.o, l) - c.r) > 0) {
				return vector<Point>();
			}
			else if (sgn(line2d::distanceBetweenPointAndLine(c.o, l)) == 0) {
				//中心を通るなら
				vector<Point> ret;
				ret.push_back(c.o + l.vec().normalized() * c.r);
				ret.push_back(c.o - l.vec().normalized() * c.r);
				return ret;
			}
			else {

				//垂線の足
				Point H = line2d::projection(c.o, l);
				vector<Point> ret;

				ret.push_back(
					H + (c.o - H).normalUnitVector() *
					sqrt(max(0.0, pow(c.r, 2) - pow(line2d::distanceBetweenPointAndLine(c.o, l), 2)))
				);
				ret.push_back(
					H - (c.o - H).normalUnitVector() *
					sqrt(max(0.0, pow(c.r, 2) - pow(line2d::distanceBetweenPointAndLine(c.o, l), 2)))
				);
				return ret;
			}
		}

		//線分と円の交点を返す。1つ目のbool値は,trueなら交わっている。
		//交点はたかだか2つであり、1つしか交点を持たない場合は同じものを2つ返す。
		vector<Point> intersectionCircleAndSegment(const Circle& c, const line2d::Segment& s) {
			auto tmp = intersectionCircleAndLine(c, s);
			vector<Point> ret;
			for (int i = 0; i < tmp.size(); i++) {
				if (sgn(line2d::distanceBetweenPointAndSegment(tmp[i], s)) == 0)
					ret.push_back(tmp[i]);
			}
			return ret;
		}

		/*2つの円の位置関係を返す。交点を持つのなら、その交点も返す。
		1つ目のintの意味は下の通り
		円AがBを内包してる。+1
		円BがAを内包してる。-1
		円AとBは内接してる。0
		円AとBは2つの交点を持つ。+2
		円AとBは外接してる。+3
		円AとBは交わらない+4
		交点が存在しないのなら、error_val2つが返される。
		*/
		pair<int, vector<Point>> intersectionCircleAndCircle(const Circle& c1, const Circle& c2) {
			Point d = c2.o - c1.o;
			vector<Point> ret;
			int mode;

			//遠すぎて交点を持たない or　内包してる
			if (sgn(d.length() - c1.r - c2.r) > 0 || sgn(d.length() - abs(c1.r - c2.r)) < 0) {
				if (sgn(d.length() - c1.r - c2.r) > 0)mode = +4;
				else {
					if (sgn(c1.r - c2.r) > 0)mode = +1;
					else mode = -1;
				}
				return make_pair(mode, ret);
			}
			else {
				if (sgn(d.length() - c1.r - c2.r) == 0)mode = +3;
				else if (sgn(d.length() - (c1.r + c2.r)) < 0 &&
					sgn(d.length() - abs(c1.r - c2.r)) > 0)mode = +2;
				else mode = 0;
			}
			double AH = (pow(c1.r, 2) - pow(c2.r, 2) + d.lengthSquare()) / (2.0 * d.length());
			double CH = sqrt(max(0.0, pow(c1.r, 2) - pow(AH, 2)));

			ret.push_back(c1.o + d.normalized() * AH + d.normalUnitVector() * CH);
			if (mode != 0 && mode != +3)
				ret.push_back(c1.o + d.normalized() * AH - d.normalUnitVector() * CH);
			return make_pair(mode, ret);
		}

		//与えられた点を通る円の接線の円上の点を(2つ)返す。接する場合は同じのを2つ返す。
		//円の内部に点があるのなら、error_valを2つ返す。
		vector<Point> tangentToCircle(const Point& p, const Circle& c) {
			if (sgn(c.o.distanceFrom(p) - c.r) < 0) {
				//円の内部にある
				return vector<Point>();
			}
			return intersectionCircleAndCircle(c,
				Circle(p,
					sqrt(max(0.0, (p - c.o).lengthSquare() - pow(c.r, 2))))
			).second;
		}

		vector<geometry2d::line2d::Line> tangentBetweenCircleAndCircle(const Circle& c1, const Circle& c2) {
			vector<geometry2d::line2d::Line> ret;
			if (sgn(abs(c1.r - c2.r) - (c1.o - c2.o).length()) > 0) {
				//包含関係にある
				return ret;
			}
			double d = (c1.o - c2.o).length();
			for (double s : {-1.0, 1.0}) {
				//外接or内接してるのなら
				if (d == abs(c1.r + c2.r * s)) {
					geometry2d::line2d::Line tmp;
					tmp.begin = c1.o +
						(sgn(c1.r + c2.r * s) >= 0 ?
						(c2.o - c1.o).normalized() * c1.r :
							(c1.o - c2.o).normalized() * c1.r
							);
					tmp.end = tmp.begin + (c2.o - c1.o).normalUnitVector();//適当にその向きでもう1点を取ってる
					ret.push_back(tmp);
					return ret;
				}
				else if (sgn(d - (c1.r + c2.r * s)) > 0) {
					double cs = (c1.r + c2.r * s) / d;//cos
					double sn = sqrt(max(0.0, 1 - cs * cs));//sin
					Point U = (c2.o - c1.o).normalized(), V = U.normalUnitVector();
					U *= cs, V *= sn;
					geometry2d::line2d::Line tmp1, tmp2;
					tmp1 = geometry2d::line2d::Line(c1.o + (U + V) * c1.r, c2.o - (U + V) * c2.r * s);
					tmp2 = geometry2d::line2d::Line(c1.o + (U - V) * c1.r, c2.o - (U - V) * c2.r * s);
					ret.push_back(tmp1), ret.push_back(tmp2);

				}
			}
			return ret;
		}

	}

	//多角形の頂点は反時計回りで与えるように
	namespace polygon2d {

		typedef vector<Point> Polygon;//リネーム

									  //点集合から凸包を作る。参照渡しをする。
									  //凸包の頂点にならない点も含む->f==true
									  //反時計回りとなってる。
		vector<Point> convexHull(vector<Point>& p, bool f) {


			sort(p.begin(), p.end());

			vector<Point> ret(2 * p.size());
			int k = 0;//凸包のサイズ

					  //下側凸包の構築
			for (int i = 0; i < p.size(); i++) {
				while (k > 1 &&
					((!f && iSP(ret[k - 2], ret[k - 1], p[i]) != +1) ||
					(f && iSP(ret[k - 2], ret[k - 1], p[i]) == -1)))k--;
				ret[k] = p[i];
				k++;
			}

			//上側凸包の構築
			for (int i = p.size() - 2, t = k; i >= 0; i--) {
				while (k > t &&
					((!f && iSP(ret[k - 2], ret[k - 1], p[i]) != +1) ||
					(f && iSP(ret[k - 2], ret[k - 1], p[i]) == -1)))k--;
				ret[k] = p[i];
				k++;
			}
			ret.resize(k - 1);//最後の戻った始点のWカウント防止
			return ret;
		}

		//任意の多角形の面積を求める。退化した場合でもできる
		double polygonArea(const Polygon& p) {
			double ret = 0;
			for (int i = 0; i < p.size(); i++) {
				ret += p[i].cross(p[(i + 1) % p.size()]);
			}
			return ret / 2.0;
		}

		//点が多角形に包含されてるか？
		//0->包含されてない
		//1->包含されてる
		//2->辺上に包含されてる
		int enclose(const Polygon& pol, const Point& poi) {
			int cnt = 0;
			for (int i = 0; i < pol.size(); i++) {
				Point u = pol[i] - poi, v = pol[(i + 1) % pol.size()] - poi;

				if (sgn(u.cross(v)) == 0 && sgn(u.dot(v)) <= 0) {
					return 2;
				}

				if (sgn(u.y - v.y) > 0)swap(u, v);
				if (sgn(u.y) <= 0 && sgn(v.y) > 0 && sgn(u.cross(v)) > 0)cnt++;

			}
			return cnt % 2 ? 1 : 0;
		}

		int enclose2(const Polygon& pol, const Point& poi) {
			line2d::Ray ray(poi, poi + Point(11.4, 5.14)); //適当な方向に半直線を伸ばす
			int cnt = 0;
			for (int i = 0; i < pol.size(); i++) {
				line2d::Segment s(pol[i], pol[(i + 1) % pol.size()]);

				if (sgn(line2d::distanceBetweenPointAndSegment(poi, s)) == 0) {
					return 2;
				}

				//直線同士ならば、交わってるはずの交点を求める。(仮想交点)
				Point vi = line2d::lineIntersection(s, ray);
				if (vi == error_val)continue;
				//半直線上にも、線分上にも載っていて、かつ線分の両端(多角形の頂点を通ってない)のならば、
				//多角形の辺と1回交わるということである。
				if (sgn(line2d::distanceBetweenPointAndRay(vi, ray)) == 0 &&
					sgn(line2d::distanceBetweenPointAndSegment(vi, s)) == 0 &&
					s.begin != vi && s.end != vi)cnt++;

			}
			return cnt % 2 ? 1 : 0;
		}

		//encloseの3つ目。非推奨かも
		int enclose3(const Polygon& pol, const Point& poi) {
			double ang = 0;
			for (int i = 0; i < pol.size(); i++) {

				//poiは辺上に存在してるか？
				if (sgn(line2d::distanceBetweenPointAndSegment(poi,
					line2d::Segment(pol[i], pol[(i + 1) % pol.size()]))) == 0) {
					return 2;
				}

				Point u = (pol[i] - poi), v = (pol[(i + 1) % pol.size()] - poi);
				double ASIN = u.cross(v) / u.length() / v.length();
				if (angletype(pol[i], poi, pol[(i + 1) % pol.size()]) == 2) {
					//鈍角なら
					double tmp = asin(max(-1.0, min(ASIN, 1.0)));
					if (sgn(tmp) > 0)ang += (acos(-1) - tmp);
					else ang += (-acos(-1) - tmp);
				}
				else {
					ang += asin(max(-1.0, min(ASIN, 1.0)));
				}
			}

			//ang==0ならば外　ang==2*piならば回ってる
			//誤差を考えて判定境界をpiにした
			if (sgn(ang - acos(-1)) > 0) {
				return 1;
			}
			else return 0;
		}

		//重心　三角形に限定しない。
		Point centerOfGravity(const Polygon& p) {
			if (p.size() <= 2)return error_val;
			Point ret;
			for (int i = 0; i < p.size(); i++) {
				Point u = p[i], v = p[(i + 1) % p.size()];
				ret += Point((u.x + v.x) / 3, (u.y + v.y) / 3) * u.cross(v);
			}
			double S = polygonArea(p);
			cout << S << endl;
			ret /= 2.0, ret /= S;
			return ret;
		}

		//内心　三角形限定
		Point incenter(const Polygon& p) {
			if (p.size() != 3)return error_val;
			Point a = p[0], b = p[1], c = p[2];
			Point f, e;

			f = b + (c - b) * (b - a).length() / ((b - a).length() + (c - a).length());
			e = c + (a - c) * (c - b).length() / ((c - b).length() + (a - b).length());
			return line2d::lineIntersection(line2d::Line(b, e), line2d::Line(a, f));
		}

		//外心　三角形限定
		Point circumcenter(const Polygon& p) {
			if (p.size() != 3)return error_val;
			Point a = p[0], b = p[1], c = p[2];

			line2d::Line d, e;
			d = line2d::Line((b + a) / 2.0, (b + a) / 2.0 + (b - a).normalUnitVector());
			e = line2d::Line((c + a) / 2.0, (c + a) / 2.0 + (c - a).normalUnitVector());
			return line2d::lineIntersection(d, e);
		}

		//垂心　三角形限定
		Point orthocenter(const Polygon& p) {
			if (p.size() != 3)return error_val;
			Point a = p[0], b = p[1], c = p[2];

			Point d, e;
			d = line2d::projection(c, line2d::Line(a, b));
			e = line2d::projection(a, line2d::Line(b, c));
			return line2d::lineIntersection(line2d::Line(d, c), line2d::Line(e, a));
		}

	}

}