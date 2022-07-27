#ifndef CLASSES_H
#define CLASSES_H

#include <atomic>
#include "Constants.h"
#include "Functions.h"

struct Base  
{
    virtual void Execute() const = 0;
		
	Base() : is_enabled(makeRandomBool()), number_of_iterations(makeRandomIterationsNumber()) {}
	virtual ~Base() = default;

    std::atomic<bool> is_enabled;
    std::atomic<uint32_t> number_of_iterations;
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

#endif
