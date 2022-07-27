#include <iostream>
#include <exception>
#include <limits>
#include <atomic>
#include <string>
#include <memory>
#include <random>
#include <vector>
#include <ctime>

#include "Constants.h"
#include "Functions.h"
#include "Classes.h"
#include "Factory.h"

using vector_base = std::vector<std::unique_ptr<Base>>;
using vector_proxy = std::vector<Proxy>;

vector_base makeObjects()
{
	Factory factory;
	vector_base objects;

	for(auto i=0; i != NUMBER_OF_TEST_OBJECTS; ++i)
		objects.push_back(factory.CreateRandom());
	
	return objects;
}
vector_proxy makeProxies(const vector_base& objects)
{
	vector_proxy proxies;

	for(const auto& object : objects)
		proxies.emplace_back(*object);

	return proxies;
}
vector_proxy makeCache(const vector_base& objects)
{
	vector_proxy cache;

	for (const auto& object : objects)
		if(test(*object))
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
	for(const auto& proxy : cache)
		proxy.Execute();
}

int main()
{
	std::cout << std::fixed;

	// Prepare the data to test
    auto objects = makeObjects();
	auto proxies = makeProxies(objects);
	auto cached = makeCache(objects);
	
	// Run timed tests
    auto direct_result = timedExecution(testDirect, objects);
    auto proxy_result = timedExecution(testProxy, proxies);
    auto cached_result = timedExecution(testCached, cached);

	print("\nDirect access result:");
	print(direct_result, '\n');
	print("Indirect access result:");
	print(proxy_result, '\n');
	print("Indirect prepared access result:");
	print(cached_result, '\n');
}
