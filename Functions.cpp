#include <iostream>
#include <random>
#include <ctime>
#include <limits>

#include "Functions.h"
#include "Constants.h"


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
