ScheduleType t = local_32_t;

#include <atomic>
#include <vector>
#include <thread>

#include "IOQueue.h"

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);

void parallel_enq(int size, int tid, int num_threads) {
  // Use this function to equeue indexes to each of the local
  // workstealing queues

  // Hint: this should be the same as in main3.cpp

  int thread_chunk = size/num_threads;
  int chunk_start = tid * thread_chunk;
  int chunk_end = chunk_start + thread_chunk;

  for (int i = chunk_start; i < chunk_end; i++) {
    Q[tid].enq(i);
  }
}

void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // Implement me using a workstealing approach.
  // You should use the Q IOQueues to compute
  // local work and then try to steal work
  // from others until all the work is completed

  // Unlike in part3_local.h, you should deq 32 elements
  // at a time.

  while (true) {
    int return_array[32] = {0};

    int ok = Q[tid].deq_32(return_array);
    if (ok == -1) break;

    for (int i = 0; i < 32; i++) {
      float base = result[return_array[i]];

      for (int j = 0; j < mult[return_array[i]]-1; j++) {
        result[return_array[i]] = result[return_array[i]] + base;
      }
    }
  }

  for (int i = 0; i < num_threads; i++) {
    if (i == tid) continue;

    while (true) {
      int return_array[32] = {0};

      int ok = Q[i].deq_32(return_array);
      if (ok == -1) break;

      for (int i = 0; i < 32; i++) {
        float base = result[return_array[i]];

        for (int j = 0; j < mult[return_array[i]]-1; j++) {
          result[return_array[i]] = result[return_array[i]] + base;
        }
      }
    }
  }
}

void launch_threads(float* result_parallel, int* mult) {
  // Launch NUM_THREADS threads to intialize the IOQueues in Q with
  // the indexes for each thread. Remember to initialize the Queues
  // with the size that they will need. Join the threads afterwards.

  // Next, launch the parallel function that performs the parallel_mult
  // function from part1_static.h and part2_global.h but using workstealing

  std:: vector<std:: thread> threads(NUM_THREADS);

  // launch threads for enqing
  for (int i = 0; i < threads.size(); i++) {
    threads[i] = std::thread(parallel_enq, SIZE, i, NUM_THREADS);
  }

  for (int i = 0; i < threads.size(); i++) {
    threads[i].join();
  }

  // launch threads for multiplying
  for (int i = 0; i < threads.size(); i++) {
    threads[i] = std::thread(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
  }

  for (int i = 0; i < threads.size(); i++) {
    threads[i].join();
  }
}