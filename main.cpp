#include <memory>
#include <vector>
#include <numeric>
#include "Constants.h"
#include "Functions.h"
#include "Classes.h"
#include "Factory.h"
#include "Result.h"
#include "NoVirtual.h"

using vector_base = std::vector<std::unique_ptr<Base>>;
using vector_proxy = std::vector<Proxy>;

vector_base makeObjects()
{
	Factory factory;
	vector_base objects;

	for (auto i = 0; i != NUMBER_OF_TEST_OBJECTS; ++i)
		objects.push_back(factory.CreateRandom());

	return objects;
}
vector_proxy makeProxies(const vector_base& objects)
{
	vector_proxy proxies;

	for (const auto& object : objects)
		proxies.emplace_back(*object);

	return proxies;
}
vector_proxy makeCache(const vector_base& objects)
{
	vector_proxy cache;

	for (const auto& object : objects)
		if (test(*object))
			cache.emplace_back(*object);

	return cache;
}

namespace Virtual
{
	void testDirect(const vector_base& objects)
	{
		for (const auto& object : objects)
			testExecute(*object);
	}
	void testProxy(const vector_proxy& proxies)
	{
		for (const auto& proxy : proxies)
			testExecute(proxy);
	}
	void testCached(const vector_proxy& cache)
	{
		for (const auto& proxy : cache)
			proxy.Execute();
	}
}
namespace NoVirtual
{
	void testDirect(const vector_base& objects)
	{
		for (const auto& object : objects)
			Execute(*object);
	}
	void testProxy(const vector_proxy& proxies)
	{
		for (const auto& proxy : proxies)
			Execute(proxy);
	}
	void testCached(const vector_proxy& cache)
	{
		for (const auto& proxy : cache)
			Execute(proxy);
	}
}
Result runTest()
{
	Result result_virtual;
	Result result_no_virtual;

	// Prepare the data to test
	auto objects = makeObjects();
	auto proxies = makeProxies(objects);
	auto cached = makeCache(objects);

	// Run timed tests
	result.direct = timedExecution(Virtual::testDirect, objects);
	result.proxy = timedExecution(Virtual::testProxy, proxies);
	result.cache = timedExecution(Virtual::testCached, cached);

	// Run with no virtual functions
	result.direct = timedExecution(NoVirtual::testDirect, objects);
	result.proxy = timedExecution(NoVirtual::testProxy, proxies);
	result.cache = timedExecution(NoVirtual::testCached, cached);

	return result;
}
std::vector<Result> runTests(const unsigned& n_times)
{
	std::vector<Result> results;
	for (unsigned i = 0; i != n_times; ++i)
		results.emplace_back(runTest());
	return results;
}
Result makeAverageResult(const std::vector<Result>& results)
{
	auto n = results.size();
	auto sum = std::accumulate(results.cbegin(), results.cend(), Result());
	return Result(sum.direct / n, sum.proxy / n, sum.cache / n);
}
void printResult(const Result& result)
{
	print("\nПрямой доступ к обьекту\t\t", result.direct);
	print("Доступ через прокси\t\t", result.proxy);
	print("Кешированный доступ\t\t", result.cache);
}


int main()
{
	std::cout << std::fixed;
	auto results = runTests(2000);
	printResult(makeAverageResult(results));
}
