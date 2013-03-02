
template<class T> static T Interpolate(T x0,T x1,T y0,T y1,T Y)
{
	return (x0 + ((Y - y0) * ((x1 - x0) / (y1 - y0))));
}