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
    virtual void Execute() = 0;
		
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

	virtual void Execute() override 
	{
		// print(__PRETTY_FUNCTION__, '\n');
		print(makeRandomNumber(0,10));
	}

private:
	// Some heavy data
	big_data_t data;
};

class B : public Base
{
public:
	B() : data(makeData()) {}
	virtual ~B() = default;

	virtual void Execute() override 
	{
		// print(__PRETTY_FUNCTION__, '\n');
		print(makeRandomNumber(0,10));
	}

private:
	big_data_t data;		// Some heavy data
};

class C : public Base
{
public:
	C() : data(makeData()) {}
	virtual ~C() = default;

	virtual void Execute() override 
	{
		// print(__PRETTY_FUNCTION__, '\n');
		print(makeRandomNumber(0,10));
	}

private:
	big_data_t data;		// Some heavy data
};

class D : public Base
{
public:
	D() : data(makeData()) {}
	virtual ~D() = default;

	virtual void Execute() override 
	{
		// print(__PRETTY_FUNCTION__, '\n');
		print(makeRandomNumber(0,10));
	}

private:
	big_data_t data;		// Some heavy data
};


enum class TestType { A, B, C, D, end };

TestType randomType()
{
	auto random_number = makeRandomNumber(0, 4);
    return static_cast<TestType>(random_number % static_cast<long long>(TestType::end));
}
std::shared_ptr<Base> makeRandomObject()
{
	auto type = randomType();

	if(type == TestType::A)
		return std::shared_ptr<Base>(new A());
	if(type == TestType::B)
		return std::shared_ptr<Base>(new B());
	if(type == TestType::C)
		return std::shared_ptr<Base>(new C());
	if(type == TestType::D)
		return std::shared_ptr<Base>(new D());

	throw std::exception();
}
std::vector<std::shared_ptr<Base>> makeTestObjects()
{
	std::vector<std::shared_ptr<Base>> test_data;

	for(auto i=0; i != NUMBER_OF_TEST_OBJECTS; ++i)
		test_data.push_back(makeRandomObject());
	
	return test_data;
}
		



// Common utility test functions
bool testIterations(const std::shared_ptr<Base>& object_ptr)
{
	return !(object_ptr->number_of_iterations % ITERATION_MODULO);
}
bool testEnabled(const std::shared_ptr<Base>& object_ptr)
{
	return object_ptr->is_enabled;
}
bool test(const std::shared_ptr<Base>& object_ptr)
{
	return testEnabled(object_ptr) && testIterations(object_ptr);
}
void testExecute(const std::shared_ptr<Base>& test_object_ptr)
{
	if(test(test_object_ptr))
		test_object_ptr->Execute();
}
// Our first main test, in which we fetch objects directly without a pointer proxy
void testDirectAccess(const std::vector<std::shared_ptr<Base>>& objects)
{
	// Note a reference here
	for (const auto& object : objects)
		testExecute(object);
}


struct ItemInfo
{
	ItemInfo() = default;
	ItemInfo(const std::shared_ptr<Base>& object) : item(object)
	{
		is_enabled.store(object->is_enabled);
		number_of_iterations.store(object->number_of_iterations);
	}

	ItemInfo(ItemInfo&& item_info) : item(std::move(item_info.item))
	{
		is_enabled.store(item_info.is_enabled);
		number_of_iterations.store(item_info.number_of_iterations);
	}
	ItemInfo(const ItemInfo& item_info) : item(item_info.item)
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

    std::shared_ptr<Base> item;
	std::atomic<bool> is_enabled; 
	std::atomic<std::uint32_t> number_of_iterations; 
};
std::vector<ItemInfo> makeTestPointers(const std::vector<std::shared_ptr<Base>>& test_objects)
{
	std::vector<ItemInfo> test_pointers;

	for(const std::shared_ptr<Base>& object : test_objects)
		test_pointers.emplace_back(object);

	return test_pointers;
}
bool testIterations(const ItemInfo& item_info)
{
	return !(item_info.number_of_iterations % ITERATION_MODULO);
}
bool testEnabled(const ItemInfo& item_info)
{
	return item_info.is_enabled;
}
bool test(const ItemInfo& item_info)
{
	return testEnabled(item_info) && testIterations(item_info);
}
void testExecute(const ItemInfo& item_info)
{
	if(test(item_info))
		item_info.item->Execute();
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

// Note the use of output paramter, which is a bad practice
array prepareIndirect(const std::vector<std::shared_ptr<Base>>& test_objects, std::size_t& end)
{
	array ready_object_ptrs;

	std::size_t index = 0;
	for (const auto& object : test_objects)
	{
		if(!test(object))
			continue;

		ready_object_ptrs[index] = ItemInfo(object);
		++index;
	}
	end = index;
	return ready_object_ptrs;
}

void testIndirectPrepared(const array& ready_object_ptrs, const std::size_t& end)
{
	for(std::size_t index = 0; index != end; ++index)
		ready_object_ptrs[index].item->Execute();
			
}
		


// Profiler function
template <typename F, typename ... Ts>
double timedExecution(F&& f, Ts&&...args)
{
    std::clock_t start = std::clock();
    std::forward<F>(f)(std::forward<Ts>(args)...);
    return static_cast<double>(std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
}



int main()
{
	// Prepare the stdout to display the results properly
	std::cout.precision(8);
	std::cout << std::fixed;

	print("Start generation of test objects...");
    std::vector<std::shared_ptr<Base>> objects = makeTestObjects();
	print("Done!", '\n');

	print("Start generation of pointers to objects...");
	auto pointers = makeTestPointers(objects);
	print("Done!", '\n');

	print("Start generation of prepared case for pointers to objects...");
	std::size_t end = 0;
	auto prepared_pointers = prepareIndirect(objects, end);
	print("Done!", '\n');
	
	std::cout << "Testing direct access... " << std::flush;
    auto direct_result = timedExecution(testDirectAccess, objects);
	std::cout << "Done!" << std::endl;
	std::cout << "Testing indirect access... " << std::flush;
    auto indirect_result = timedExecution(testIndirectAccess, pointers);
	std::cout << "Done!" << std::endl;
	std::cout << "Testing indirect prepared access... " << std::flush;
    auto indirect_prepared_result = timedExecution(testIndirectPrepared, prepared_pointers, end);
	std::cout << "Done!" << std::endl;

	print("Direct access result:");
	print(direct_result, '\n');
	print("Indirect access result:");
	print(indirect_result, '\n');
	print("Indirect prepared access result:");
	print(indirect_prepared_result);
	
	std::cout << std::endl;

}
