#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <ctime>
#include <iostream>

#include "Constants.h"

template <typename T>
void print(const T& msg, char delimiter = ' ')
{
	std::cout << msg << delimiter;
}
template <typename T>
void print(const T& msg, double result, char delimiter = '\n')
{
	std::cout << msg << ": " << result << delimiter;
}

// Profiler function
template <typename F, typename ... Ts>
double timedExecution(F&& f, Ts&&...args)
{
    std::clock_t start = std::clock();
    std::forward<F>(f)(std::forward<Ts>(args)...);
    return static_cast<double>(std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
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

big_number_t makeRandomNumber(long long, long long);

big_data_t makeData();

bool makeRandomBool();

long long makeRandomIterationsNumber();

#endif
