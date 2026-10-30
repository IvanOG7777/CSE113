#include <thread>
#include <vector>

ScheduleType t = static_t;

void parallel_mult(float *result, int *mult, int size, int tid, int num_threads) {
  // write me in a SPMD parallel way!  partition the data using
  // "chunks" rather than strides.  That is thread 0 computes elements
  // 0 through size/num_threads.  thread 1 computes elements
  // size/num_threads through 2*(size/num_threads)

  // start at t_id * chunk (size/num_threads)
  // loop while i is less than chunk (t_id * size/num_threads)
  // add 1 to i per loop
  for (int i = tid * (size/num_threads); i < tid * (size/num_threads) + size/num_threads; i++) {

    float base = result[i]; // load in current base

    // loop the N times and add to result
    for (int w = 0; w < mult[i]-1; w++) {
      result[i] = result[i] + base;
    }
  }
}

void launch_threads(float* result_parallel, int* mult) {
  // Launch NUM_THREADS threads and then join them

  std:: vector<std::thread> threads(NUM_THREADS); // create threads array of size NUM_THREADS
  for (int i  = 0; i < NUM_THREADS; i++) {
    threads[i] = std::thread(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS); // launch each thread
  }

  // loop through threads and join when done
  for (int i = 0; i < NUM_THREADS; i++) {
    threads[i].join();
  }
}