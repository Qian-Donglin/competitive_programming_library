struct Rolling_Hash
{

	const int Try = 3;

	const ll MD[Try] = {1000000007, 999999937, 943759273};
	const ll base[Try] = {109, 313, 1289};
	string X; //Xの連続部分列の中にYがあるかをチェック
	vector<ll> memo[Try];
	int XL;

	void setup(const string &a = "")
	{
		X = a;
		for (int t = 0; t < Try; t++)
		{
			memo[t].resize(X.size() + 1);
			XL = X.size();
			//(0, i]のハッシュ値の計算をする 累積和的にやる
			ll powbase = 1;
			memo[t][1] = X[0];
			for (int i = 2; i <= XL; i++)
			{
				powbase *= base[t], powbase %= MD[t];
				memo[t][i] = (memo[t][i - 1] + powbase * X[i - 1]) % MD[t];
			}
		}
	}

	bool match(const string &Y)
	{

		int YL = Y.size();

		for (int t = 0; t < Try; t++)
		{
			ll Yh = 0;
			ll powb = 1;
			for (int i = 0; i < YL; i++)
			{
				Yh = (Yh + Y[i] * powb) % MD[t];
				powb *= base[t], powb %= MD[t];
			}
			for (int i = 0; i + YL <= XL; i++)
			{
				ll Xh = (memo[t][i + YL] - memo[t][i] + MD[t]) % MD[t];
				if (Xh == Yh)
					return true;
				Yh = (Yh * base[t]) % MD[t];
			}
		}

		return false;
	}
};