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
constexpr unsigned NUMBER_OF_TEST_OBJECTS = 1000;
constexpr unsigned NUMBER_OF_DATUMS = 10;

class Base;

using big_number_t = long long;
using big_data_t = std::array<big_number_t, NUMBER_OF_DATUMS>;		// 80 bytes on my machine

// Utility funcitons
void print(const big_number_t& n) 
{
	std::cout << n << std::endl;
}
void print(const std::string& msg, const char delim = ' ')
{
	std::cout << msg << delim;
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
		print(__PRETTY_FUNCTION__, '\n');
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
		print(__PRETTY_FUNCTION__, '\n');
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
		print(__PRETTY_FUNCTION__, '\n');
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
		print(__PRETTY_FUNCTION__, '\n');
	}

private:
	big_data_t data;		// Some heavy data
};


enum class TestType { A, B, C, D, end};

TestType randomType()
{
	auto random_number = makeRandomNumber(0, 4);
    return static_cast<TestType>(random_number % static_cast<long long>(TestType::end));
}
std::shared_ptr<Base> makeRandomObject()
{
	auto type = randomType();

	if(type == TestType::A)
	{
		print("A");
		return std::shared_ptr<Base>(new A());
	}
	if(type == TestType::B)
	{
		print("B");
		return std::shared_ptr<Base>(new B());
	}
	if(type == TestType::C)
	{
		print("C");
		return std::shared_ptr<Base>(new C());
	}
	if(type == TestType::D)
	{
		print("D");
		return std::shared_ptr<Base>(new D());
	}

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
void testExecute(const std::shared_ptr<Base>& test_object_ptr)
{
	if(testEnabled(test_object_ptr) && testIterations(test_object_ptr))
		test_object_ptr->Execute();
}
// Our first main test, in which we fetch objects directly without a pointer proxy
void testDirectObjectAccess(const std::vector<std::shared_ptr<Base>>& objects)
{
	// Note a reference here
	for (const auto& object : objects)
		testExecute(object);
}


struct ItemInfo
{
	ItemInfo(const std::shared_ptr<Base> object) : item(object)
	{
		is_enabled.store(object->is_enabled);
		number_of_iterations.store(object->number_of_iterations);
	}

    std::shared_ptr<Base> item;
	std::atomic<bool> is_enabled; 
	std::atomic<std::uint32_t> number_of_iterations; 
};


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
	std::cout << "\n\nSize of big_data_t: " << sizeof(big_data_t) << std::endl;
	std::cout << "Size of a descendant class (e.g. class A): " << sizeof(A) << std::endl;

	print("Start generation of test object", '\n');
    std::vector<std::shared_ptr<Base>> objects = makeTestObjects();

	print("\nFinished generation of objects", '\n');
    std::cout << timedExecution(testDirectObjectAccess, objects);

}
