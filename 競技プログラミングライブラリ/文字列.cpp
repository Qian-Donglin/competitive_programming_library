namespace str_algo {
	//Manacher法
	//R[i] := A[i]を中心とする回文の半径をO(A.size())で求める
	//これだと、全長が偶数の回文の検出ができないけど、これは
	//daabbaad -> $d$a$a$b$b$a$a$d$　と文字列を加工させば、奇数の奴に帰着可能

	vector<int> Manacher(string S) {
		int c = 0, n = S.length();
		vector<int> R(n, 1);
		for (int i = 0; i < n; i++) {
			int l = c - (i - c);
			if (i + R[l] < c + R[c]) {
				R[i] = R[l];
				//一番回文の右端が右のやつを、走査の中心回文とする。
				//中心回文は、これからの点の中心の回文列の半径を既存のデータから求める基準となる。
			}
			else {
				//
				int j = c + R[c] - i;
				while (i - j >= 0 && i + j < n && S[i - j] == S[i + j])j++;
				R[i] = j;
				c = i;
			}
		}
		return R;
	}

	vector<int> Z_algorithm(string S) {
		int c = 0, n = S.length();
		vector<int> Z(n, 0);
		for (int i = 1; i < n; i++) {
			int l = i - c;
			if (i + Z[l] < c + Z[c]) {
				Z[i] = Z[l];
			}
			else {
				int j = max(0, c + Z[c] - i);
				while (i + j < n && S[j] == S[i + j])j++;
				Z[i] = j;
				c = i;
			}
		}

		Z[0] = n;
		return Z;
	}
}