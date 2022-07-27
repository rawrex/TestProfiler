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


std::vector<std::unique_ptr<Base>> makeTestObjects()
{
	Factory factory;
	std::vector<std::unique_ptr<Base>> test_data;

	for(auto i=0; i != NUMBER_OF_TEST_OBJECTS; ++i)
		test_data.push_back(factory.CreateRandom());
	
	return test_data;
}
std::vector<Proxy> makeProxies(const std::vector<std::unique_ptr<Base>>& test_objects)
{
	std::vector<Proxy> test_pointers;

	for(const std::unique_ptr<Base>& object : test_objects)
		test_pointers.emplace_back(*object);

	return test_pointers;
}
std::vector<Proxy> makeCache(const std::vector<std::unique_ptr<Base>>& test_objects)
{
	std::vector<Proxy> ready_object_ptrs;

	for (const auto& object : test_objects)
		if(!test(*object))
			continue;
		else
			ready_object_ptrs.emplace_back(*object);
	return ready_object_ptrs;
}

void testDirect(const std::vector<std::unique_ptr<Base>>& objects)
{
	for (const auto& object : objects)
		testExecute(*object);
}
void testProxy(const std::vector<Proxy>& ptr_objects)
{
	for (const auto& object : ptr_objects)
		testExecute(object);
}
void testCached(const std::vector<Proxy>& cached_proxies)
{
	for(const auto& proxy : cached_proxies)
		proxy.Execute();
}

int main()
{
	std::cout << std::fixed;

	// Prepare the data to test
    auto objects = makeTestObjects();
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
