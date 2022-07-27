#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>

constexpr unsigned ITERATION_MODULO = 3;
constexpr unsigned MIN_ITERATIONS = 0;
constexpr unsigned MAX_ITERATIONS = 100;
constexpr unsigned NUMBER_OF_TEST_OBJECTS = 255;
constexpr unsigned NUMBER_OF_DATUMS = 10;

using big_number_t = long long;
using big_data_t = std::array<big_number_t, NUMBER_OF_DATUMS>;

#endif
