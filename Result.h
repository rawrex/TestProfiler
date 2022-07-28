#ifndef RESULT_H
#define RESULT_H
struct Result
{
	double direct = 0;
	double proxy = 0;
	double cache = 0;

	Result() = default;
	Result(double d, double p, double c) : direct(d), proxy(p), cache(c) {}
};

Result operator+(const Result& lhs, const Result& rhs)
{
	return Result(lhs.direct + rhs.direct, lhs.proxy + rhs.proxy, lhs.cache + rhs.cache);
}

#endif
