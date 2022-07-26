#include <iostream>
#include <atomic>
#include <memory>
#include <random>
#include <vector>

constexpr unsigned ITERATION_MODULO = 3;
constexpr unsigned NUMBER_OF_TEST_OBJECTS = 1000;

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
		// Fill the data with big random numbers
	}
	// Some heavy data
	std::vector<long long> data;	
};

class B : public Base
{
public:
	B()
	{
		// Fill the data with big random numbers
	}
	// Some heavy data
	std::vector<std::wstring> data;	
};

class C : public Base
{
public:
	C()
	{
		// Fill the data with big random numbers
	}
	// Some heavy data
	std::vector<int64_t> data;	
};

class D : public Base
{
public:
	D()
	{
		// Fill the data with big random numbers
	}
	// Some heavy data
	std::vector<long long> data;	
};

struct ItemInfo
{
    Base* item;

	std::atomic<bool> is_enabled; 
	std::atomic<std::uint32_t> numebr_of_iterations; 
};


// Utility functions which we'll use to populate our test objects with random data
void makeTestData()
{
	std::vector<std::unique_ptr<Base>> test_data;
	for(auto i=0; i != NUMBER_OF_TEST_OBJECTS; ++i)
		test_data.emplace_back();
		
	


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
