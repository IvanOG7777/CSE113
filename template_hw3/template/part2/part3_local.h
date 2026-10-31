ScheduleType t = local_t;

#include <atomic>
#include <vector>
#include <thread>

#include "IOQueue.h"

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);

void parallel_enq(int size, int tid, int num_threads) {
  // Use this function to enqueue indexes to each of the local work stealing queues

  int thread_chunk = size/num_threads; // calculate the chunks per thread
  int chunk_start = tid * thread_chunk; // starting value of threads chunk
  int chunk_end = chunk_start + thread_chunk; // end index of threads chunk

  Q[tid].init(thread_chunk); // resize threads queue to its chunk size

  // Loop through its starting chunk index until its end enqueuing its i to buffer
  for (int i = chunk_start; i < chunk_end; i++) {
    Q[tid].enq(i);
  }
}

void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // Implement me using a workstealing approach.
  // You should use the Q IOQueues to compute
  // local work and then try to steal work
  // from others until all the work is completed


  // keep dequeueing from myself
  while (true) {
    int return_value = Q[tid].deq(); // get my index from returned dequeue

    if (return_value == -1) break; // break when no more elements in my queue exit loop

    // Usual "multiplication" on result[return_value]
    float base = result[return_value];
    for (int i = 0; i < mult[return_value]-1; i++) {
      result[return_value] = result[return_value] + base;
    }
  }

  // Work stealing loop
  // Go through other threads queues
  for (int i = 0; i < num_threads; i++) {
    if (i == tid) continue; // skip myself

    // Do the same process on other threads like I did on me.
    while (true) {
      int return_value = Q[i].deq();

      if (return_value == -1) break;

      float base = result[return_value];
      for (int i = 0; i < mult[return_value]-1; i++) {
        result[return_value] = result[return_value] + base;
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