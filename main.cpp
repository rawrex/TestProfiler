#include <memory>
#include <vector>
#include <numeric>
#include "Constants.h"
#include "Functions.h"
#include "Classes.h"
#include "Factory.h"
#include "Result.h"

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

Result runTest()
{
	Result result;

	// Prepare the data to test
	auto objects = makeObjects();
	auto proxies = makeProxies(objects);
	auto cached = makeCache(objects);

	// Run timed tests
	result.direct = timedExecution(testDirect, objects);
	result.proxy = timedExecution(testProxy, proxies);
	result.cache = timedExecution(testCached, cached);

	return result;
}
void printResult(const Result& result)
{
	print("\nDirect access result\t\t", result.direct);
	print("Indirect access result\t\t", result.proxy);
	print("Cached access result\t\t", result.cache);
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


int main()
{
	std::cout << std::fixed;
	auto results = runTests(4000);
	printResult(makeAverageResult(results));
}
