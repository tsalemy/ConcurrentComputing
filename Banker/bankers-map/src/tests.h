// CSE 375/475 Assignment #1
// Fall 2019

// Description: This file declares a function that should be able to use the
// configuration information to drive tests that evaluate the correctness and
// performance of the map type you create.

#include "simplemap.h"
#include "locks.h"
#include "config_t.h"
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <thread>
#include "string.h"
#include "threads.h"

#define MAX_AMOUNT 100000
#define EXISTS(pair) (pair).second
#define GET_BALANCE(pair) (pair).first
#define OPTIMIZED 0

void test_driver(config_t& cfg);
void do_work(thread_data_t* thread);
bool deposit(simplemap_t<int, float>* map, thread_data_t* thread);
float balance(simplemap_t<int, float>* map, unsigned int range, bool lockfree);
