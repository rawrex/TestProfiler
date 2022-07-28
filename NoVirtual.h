#ifndef NOVIRTUAL_H
#define NOVIRTUAL_H

#include <atomic>
#include "Factory.h"
#include "Constants.h"
#include "Functions.h"

struct NoVirtualBase  
{
	NoVirtualBase() 
		: is_enabled(makeRandomBool()), number_of_iterations(makeRandomIterationsNumber()) {}
	virtual ~NoVirtualBase() = default;

    std::atomic<bool> is_enabled;
    std::atomic<uint32_t> number_of_iterations;
};

class NoVirtualA : public NoVirtualBase
{
public:
	NoVirtualA() : data(makeData()) {}
	virtual ~NoVirtualA() = default;
	void Execute() const 
	{
		print(makeRandomNumber(0,10));
	}

private:
	big_data_t data;
};

class NoVirtualB : public NoVirtualBase
{
public:
	NoVirtualB() : data(makeData()) {}
	virtual ~NoVirtualB() = default;
	void Execute() const 
	{
		print(makeRandomNumber(0,10));
	}
private:
	big_data_t data;
};

class NoVirtualC : public NoVirtualBase
{
public:
	NoVirtualC() : data(makeData()) {}
	virtual ~NoVirtualC() = default;
	void Execute() const 
	{
		print(makeRandomNumber(0,10));
	}
private:
	big_data_t data;
};

class NoVirtualD : public NoVirtualBase
{
public:
	NoVirtualD() : data(makeData()) {}
	virtual ~NoVirtualD() = default;
	void Execute() const 
	{
		print(makeRandomNumber(0,10));
	}

private:
	big_data_t data;
};

struct NoVirtualProxy
{
	NoVirtualProxy() = default;
	NoVirtualProxy(NoVirtualProxy&& item_info) : item(std::move(item_info.item))
	{
		is_enabled.store(item_info.is_enabled);
		number_of_iterations.store(item_info.number_of_iterations);
	}
	NoVirtualProxy& operator=(const NoVirtualProxy& item_info) 
	{
		item = item_info.item;
		is_enabled.store(item_info.is_enabled);
		number_of_iterations.store(item_info.number_of_iterations);
		return *this;
	}
	NoVirtualProxy(const NoVirtualBase& object) : item(&object) 
	{
		is_enabled.store(object.is_enabled);
		number_of_iterations.store(object.number_of_iterations);
	}

	const NoVirtualBase* item = nullptr;
	std::atomic<bool> is_enabled = false; 
	std::atomic<std::uint32_t> number_of_iterations = 0;
};

void Execute(const NoVirtualA& object)
{
	object.Execute();
}
void Execute(const NoVirtualB& object)
{
	object.Execute();
}
void Execute(const NoVirtualC& object)
{
	object.Execute();
}
void Execute(const NoVirtualD& object)
{
	object.Execute();
}
#endif
