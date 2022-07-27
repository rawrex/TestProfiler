#include <iostream>
#include <exception>
#include <limits>
#include <atomic>
#include <string>
#include <memory>
#include <random>
#include <vector>
#include <ctime>

constexpr unsigned ITERATION_MODULO = 3;
constexpr unsigned MIN_ITERATIONS = 0;
constexpr unsigned MAX_ITERATIONS = 100;
constexpr unsigned NUMBER_OF_TEST_OBJECTS = 255;
constexpr unsigned NUMBER_OF_DATUMS = 10;

class Base;

using big_number_t = long long;
using big_data_t = std::array<big_number_t, NUMBER_OF_DATUMS>;		// 80 bytes on my machine

// Utility funcitons
template <typename T>
void print(const T& msg, char delimiter = ' ')
{
	std::cout << msg << delimiter;
}

big_number_t makeRandomNumber(long long min, long long max)
{
	static std::default_random_engine engine(time(nullptr));
	std::uniform_int_distribution<big_number_t> dist(min, max);
	return dist(engine);
}

// Profiler function
template <typename F, typename ... Ts>
double timedExecution(F&& f, Ts&&...args)
{
    std::clock_t start = std::clock();
    std::forward<F>(f)(std::forward<Ts>(args)...);
    return static_cast<double>(std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
}

// Make a container populated with NUMBER_OF_TEST_OBJECTS with random values
big_data_t makeData()
{
	big_data_t data;
	
	auto min = std::numeric_limits<big_number_t>::min();
	auto max = std::numeric_limits<big_number_t>::max();
	
	for(auto i=0; i != NUMBER_OF_DATUMS; ++i)
	{
		auto random_number = makeRandomNumber(min, max);
		data[i] = random_number;
	}

	return data;
}
bool makeRandomBool()
{
	return makeRandomNumber(0, 1);
}
long long makeRandomIterationsNumber()
{
	return makeRandomNumber(MIN_ITERATIONS, MAX_ITERATIONS);
}
	
struct Base  
{
    virtual void Execute() const = 0;
		
	Base() : is_enabled(makeRandomBool()), number_of_iterations(makeRandomIterationsNumber()) {}
	virtual ~Base() = default;

    std::atomic<bool> is_enabled;
    std::atomic<std::uint32_t> number_of_iterations;
};

class A : public Base
{
public:
	A() : data(makeData()) {}
	virtual ~A() = default;
	virtual void Execute() const override 
	{
		print(makeRandomNumber(0,10));
	}

private:
	big_data_t data;
};

class B : public Base
{
public:
	B() : data(makeData()) {}
	virtual ~B() = default;
	virtual void Execute() const override 
	{
		print(makeRandomNumber(0,10));
	}
private:
	big_data_t data;
};

class C : public Base
{
public:
	C() : data(makeData()) {}
	virtual ~C() = default;
	virtual void Execute() const override 
	{
		print(makeRandomNumber(0,10));
	}
private:
	big_data_t data;
};

class D : public Base
{
public:
	D() : data(makeData()) {}
	virtual ~D() = default;
	virtual void Execute() const override 
	{
		print(makeRandomNumber(0,10));
	}

private:
	big_data_t data;
};

enum class TestType { A, B, C, D, end };

TestType randomType()
{
	auto random_number = makeRandomNumber(0, 4);
    return static_cast<TestType>(random_number % static_cast<long long>(TestType::end));
}
std::unique_ptr<Base> makeRandomObject()
{
	auto type = randomType();

	if(type == TestType::A)
		return std::unique_ptr<Base>(new A());
	if(type == TestType::B)
		return std::unique_ptr<Base>(new B());
	if(type == TestType::C)
		return std::unique_ptr<Base>(new C());
	if(type == TestType::D)
		return std::unique_ptr<Base>(new D());

	throw std::exception();
}
std::vector<std::unique_ptr<Base>> makeTestObjects()
{
	std::vector<std::unique_ptr<Base>> test_data;

	for(auto i=0; i != NUMBER_OF_TEST_OBJECTS; ++i)
		test_data.push_back(makeRandomObject());
	
	return test_data;
}
		
// Test framework
template <typename T>
bool testIterations(const T& object)
{
	return !(object.number_of_iterations % ITERATION_MODULO);
}
template <typename T>
bool testEnabled(const T& object)
{
	return object.is_enabled;
}
template <typename T>
bool test(const T& object)
{
	return testEnabled(object) && testIterations(object);
}
template <typename T>
void testExecute(const T& object)
{
	if(test(object))
		object.Execute();
}

struct ItemInfo
{
	ItemInfo() = default;
	ItemInfo(ItemInfo&& item_info) : item(std::move(item_info.item))
	{
		is_enabled.store(item_info.is_enabled);
		number_of_iterations.store(item_info.number_of_iterations);
	}
	ItemInfo& operator=(const ItemInfo& item_info) 
	{
		item = item_info.item;
		is_enabled.store(item_info.is_enabled);
		number_of_iterations.store(item_info.number_of_iterations);
		return *this;
	}
	ItemInfo(const Base& object) : item(&object) 
	{
		is_enabled.store(object.is_enabled);
		number_of_iterations.store(object.number_of_iterations);
	}
	void Execute() const 
	{
		item->Execute();
	}
	const Base* item = nullptr;
	std::atomic<bool> is_enabled = false; 
	std::atomic<std::uint32_t> number_of_iterations = 0;
};
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
