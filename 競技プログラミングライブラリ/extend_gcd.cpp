//e_gcd()自体はaとbの最大公約数を返す。
//xとyは、a*x+b*y=gcd(a,b)となる(x,y)の1つの組を返す。
//a*x+b*y=gcdの時、a=b*p+(a%b)を代入して、b*p*x+(a%b)*x+b*y=b*(p*x+y)+(a%b)*x
//次のx->p*x+y
//次のy->x
//次のa->b
//次のb->a%b
ll e_gcd(ll a, ll b, ll &x, ll &y)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	ll gcd_res = extend_gcd(b, a % b, x, y);
	ll p = a / b;
	ll nx = x, ny = y;
	x = ny;
	y = nx - p * x;
	return gcd_res;
}