#ifndef RESULT_H
#define RESULT_H
struct Result
{
	double direct;
	double proxy;
	double cache;
	virtual ~Result() = default;
};

struct AverageResult : public Result
{
private:
	void addDirectSample(const Result& new_sample)
	{
		direct -= direct / N;
		direct += new_sample.direct	/ N;
	}
	void addProxySample(const Result& new_sample)
	{
		proxy -= proxy / N;
		proxy += new_sample.proxy / N;
	}
	void addCacheSample(const Result& new_sample)
	{
		cache -= cache / N;
		cache += new_sample.cache / N;
	}

public:
	unsigned N = 1;
	AverageResult() = default;
	~AverageResult() = default;
	AverageResult(const unsigned& n) : N(n) {}
	void addSample(const Result& new_sample)
	{
		addDirectSample(new_sample);
		addProxySample(new_sample);
		addCacheSample(new_sample);
	}
};

#endif
