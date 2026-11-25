#define SRBARRIER // So that the header is not included twice in driver.cpp
#include "SRBarrier.h"

using namespace std;

// This should be the same as part 2.2 but with memory orders changed

void t0_function(atomic_int* x, atomic_int* y, int* r0, barrier_object* B) {
  B->barrier(0); // wait for barrier with id of 0
  x->store(1, std:: memory_order_seq_cst); // store 1 with mem seq mode
  r0[0] = y->load(std:: memory_order_seq_cst); // load y to r0 with mem seq mode
}

void t1_function(atomic_int* x, atomic_int* y, int* r1,  barrier_object* B) {
  B->barrier(1); // wait for barrier with id of 1
  y->store(1, std:: memory_order_seq_cst); // store 1 with mem seq mode
  r1[0] = x->load(std:: memory_order_seq_cst); // load y to r0 with mem seq mode
}
