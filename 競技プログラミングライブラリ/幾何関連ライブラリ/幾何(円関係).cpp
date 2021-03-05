namespace circle_2d {
	//交点が存在しないのなら、make_pair(error_val, error_val);を返す
	pair<v2d::Vec2, v2d::Vec2> crossPointInCircle(v2d::Vec2 p1, double r1, v2d::Vec2 p2, double r2) {
		
		if (p1 == p2)return make_pair(v2d::error_val, v2d::error_val);

		//完全に内包関係にある場合
		if (v2d::sgn(abs(r1 - r2) - (p1 - p2).length()) == 1)
			return make_pair(v2d::error_val, v2d::error_val);

		//p1中心の円を原点として考えることで、コーディング量が減る。

		v2d::Vec2 fp = p2 - p1;
		double fC = (pow(r1, 2) - pow(r2, 2) + fp.lengthSquare()) / 2.0;
		v2d::Vec2 ret1, ret2;

		if (v2d::sgn(fp.y) == 0) {
			//yをxの式で表す関係上、fp.yで割る必要があるので、fp.y==0ならば例外処理をする。
			ret1.x = fC / fp.x, ret2.x = fC / fp.x;
			ret1.y = sqrt(max(0.0, pow(r1, 2) - pow(ret1.x, 2)));
			ret2.y = -sqrt(max(0.0, pow(r1, 2) - pow(ret1.x, 2)));
			ret1 += p1, ret2 += p1;
			return make_pair(ret1, ret2);
		}
		else {

			//これは、交点を持たないとき
			if (v2d::sgn(pow(fp.x * fC, 2) - fp.lengthSquare() * (pow(fC, 2) - pow(fp.y * r1, 2))) == -1) {
				return make_pair(v2d::error_val, v2d::error_val);
			}

			ret1.x = (fp.x * fC + sqrt(pow(fp.x * fC, 2) - fp.lengthSquare() * (pow(fC, 2) - pow(fp.y * r1, 2))))
				/ fp.lengthSquare();
			ret2.x = (fp.x * fC - sqrt(pow(fp.x * fC, 2) - fp.lengthSquare() * (pow(fC, 2) - pow(fp.y * r1, 2))))
				/ fp.lengthSquare();
			ret1.y = (fC - fp.x * ret1.x) / fp.y;
			ret2.y = (fC - fp.x * ret2.x) / fp.y;
			ret1 += p1, ret2 += p1;
			return make_pair(ret1, ret2);
		}

	}
}
