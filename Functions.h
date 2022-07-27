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

// Profiler function
template <typename F, typename ... Ts>
double timedExecution(F&& f, Ts&&...args)
{
    std::clock_t start = std::clock();
    std::forward<F>(f)(std::forward<Ts>(args)...);
    return static_cast<double>(std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
}

big_number_t makeRandomNumber(long long, long long);


big_data_t makeData();

bool makeRandomBool();

long long makeRandomIterationsNumber();

#endif
