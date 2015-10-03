#pragma once

#include "take_time.h"
#include <mutex>

const int LOOP_SIZE = 10;
//number producers
const int N = 10;
//number of consumers
const int M = 4;

//benchmark objects
take_time::time push_time;
take_time::time pop_time;

using SINGLE_MEASURE_UNIT = take_time::time::_nano_unit;
using TOTAL_MEASURE_UNIT = take_time::time::_micro_unit;

const take_time::time_unit_t SINGLE_MEASURE_UNIT_STR = take_time::_nano_unit_str;
const take_time::time_unit_t TOTAL_MEASURE_UNIT_STR = take_time::_micro_unit_str;

