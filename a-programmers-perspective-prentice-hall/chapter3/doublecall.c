
long Q(long v)
{
		return v + 3;
}


long P(long x, long y)
{
		long u = Q(y);
		long v = Q(x);
		return u + v;
}
