#include <iostream>
#include <climits>
#include <atomic>
#include <memory>
#include <random>
#include <vector>
#include <ctime>

constexpr unsigned ITERATION_MODULO = 3;
constexpr unsigned NUMBER_OF_TEST_OBJECTS = 1000;

using big_number_t = long long;
using big_data_t= std::vector<big_number_t>;

void print(const big_number_t& n) 
{
	std::cout << n << std::endl;
}


// Make a container populated with NUMBER_OF_TEST_OBJECTS with random values
big_data_t makeData()
{
	big_data_t data;
	
	auto min = LLONG_MAX - LLONG_MAX/2;
	auto max = LLONG_MAX;	
	
	std::default_random_engine engine(time(nullptr));
	std::uniform_int_distribution<big_number_t> dist(min, max);

	for(auto i=0; i != NUMBER_OF_TEST_OBJECTS; ++i)
	{
		auto random_number = dist(engine);
		print(random_number);
		data.emplace_back(dist(engine));
	}

	return data;
}


struct Base  
{
    virtual void Execute() = 0;
		
	virtual ~Base() = default;

    std::atomic<bool> is_enabled;
    std::atomic<std::uint32_t> number_of_iterations;
};

class A : public Base
{
public:
	A()
	{
		data = makeData();	// Fill the data with big random numbers
	}
	big_data_t data;		// Some heavy data
};

class B : public Base
{
public:
	B()
	{
		data = makeData();	// Fill the data with big random numbers
	}
	big_data_t data;		// Some heavy data
};

class C : public Base
{
public:
	C()
	{
		data = makeData();	// Fill the data with big random numbers
	}
	big_data_t data;		// Some heavy data
};

class D : public Base
{
public:
	D()
	{
		data = makeData();	// Fill the data with big random numbers
	}
	big_data_t data;		// Some heavy data
};

struct ItemInfo
{
    Base* item;

	std::atomic<bool> is_enabled; 
	std::atomic<std::uint32_t> numebr_of_iterations; 
};




// Common utility test functions
bool testIterations(const std::unique_ptr<Base>& object_ptr)
{
	return !(object_ptr->number_of_iterations % ITERATION_MODULO);
}
bool testEnabled(const std::unique_ptr<Base>& object_ptr)
{
	return object_ptr->is_enabled;
}
void testExecute(const std::unique_ptr<Base>& test_object_ptr)
{
	if(testEnabled(test_object_ptr) && testIterations(test_object_ptr))
		test_object_ptr->Execute();
}

// Our first main test, in which we fetch objects directly without a pointer proxy
void testDirectObjectAccess(const std::vector<std::unique_ptr<Base>>& objects)
{
	// Note a reference here
	for (const auto& object : objects)
		testExecute(object);
}

int main()
{
    std::vector<std::unique_ptr<Base>> objects;

    testDirectObjectAccess(objects);

}
