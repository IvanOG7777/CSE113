#define SRBARRIER // So that the header is not included twice in driver.cpp
#include "SRBarrier.h"

using namespace std;

// You should use the barrier in this part to increase the number of relaxed behaviors

void t0_function(atomic_int* x, atomic_int* y, int* r0, barrier_object* B) {
  // wait for barrier with id of 0
  B->barrier(0);
  // store 1 to x with relaxed memory ordering
  x->store(1, std::memory_order_relaxed);
  r0[0] = y->load(std::memory_order_relaxed); // load y value to r0
}

void t1_function(atomic_int* x, atomic_int* y, int* r1,  barrier_object* B) {
  B->barrier(1); // wait for barrier with id of 1
  // store 1 to y with relaxed memory ordering
  y->store(1, std::memory_order_relaxed);
  r1[0] = x->load(std::memory_order_relaxed); // load x value to r1
}
