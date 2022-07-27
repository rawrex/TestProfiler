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

std::vector<ItemInfo> makeTestPointers(const std::vector<std::unique_ptr<Base>>& test_objects)
{
	std::vector<ItemInfo> test_pointers;

	for(const std::unique_ptr<Base>& object : test_objects)
		test_pointers.emplace_back(*object);

	return test_pointers;
}
// Execute as soon as an object has passed the tests
void testIndirectAccess(const std::vector<ItemInfo>& ptr_objects)
{
	for (const auto& object : ptr_objects)
		testExecute(object);
}

// Prepare the ready items first and then execute them in one iteration
// We'll use the std::array
using array = std::array<ItemInfo, NUMBER_OF_TEST_OBJECTS>;


std::vector<ItemInfo> prepareIndirect(const std::vector<std::unique_ptr<Base>>& test_objects)
{
	std::vector<ItemInfo> ready_object_ptrs;

	for (const auto& object : test_objects)
	{
		if(!test(*object))
			continue;

		ready_object_ptrs.emplace_back(*object);
	}
	return ready_object_ptrs;
}

void testIndirectPrepared(const std::vector<ItemInfo>& ready_items)
{
	for(const auto& item : ready_items)
		item.Execute();
}
		
void testDirectAccess(const std::vector<std::unique_ptr<Base>>& objects)
{
	for (const auto& object : objects)
		testExecute(*object);
}





int main()
{
	std::cout << std::fixed;

    auto objects = makeTestObjects();
	auto pointers = makeTestPointers(objects);
	auto prepared_pointers = prepareIndirect(objects);
	
    auto direct_result = timedExecution(testDirectAccess, objects);
    auto indirect_result = timedExecution(testIndirectAccess, pointers);
    auto indirect_prepared_result = timedExecution(testIndirectPrepared, prepared_pointers);

	print("\nDirect access result:");
	print(direct_result, '\n');
	print("Indirect access result:");
	print(indirect_result, '\n');
	print("Indirect prepared access result:");
	print(indirect_prepared_result);
	
	std::cout << std::endl;

}
