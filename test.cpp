#include <iostream>
#include <atomic>
#include <memory>
#include <random>
#include <vector>

constexpr unsigned ITERATION_MODULO = 3;

struct Base  
{
    virtual void Execute() = 0;
		
	virtual ~Base() = default;

    std::atomic<bool> is_enabled;
    std::atomic<std::uint32_t> number_of_iterations;
};

class A : public Base
{
	// Some heavy data
	std::vector<long long> data;	
};

class B : public Base
{
	// Some heavy data
	std::vector<std::wstring> data;	
};

class C : public Base
{
	// Some heavy data
	std::vector<int64_t> data;	
};

class D : public Base
{
	// Some heavy data
	std::vector<long long> data;	
};

struct ItemInfo
{
    Base* item;

	std::atomic<bool> is_enabled; 
	std::atomic<std::uint32_t> numebr_of_iterations; 
};

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
