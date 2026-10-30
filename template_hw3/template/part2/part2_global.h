ScheduleType t = global_t;

#include <atomic>
#include <vector>
#include <thread>

std::atomic_int counter(0);

void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // Write me a parallel way using the global counter
  // as an implicit worklist. That is, every thread
  // should get its index from incrementing the counter

  // start i at current counter and add 1
  // loop until i = SIZE - 1
  // move i to next available counter and add one
  for (int i = counter.fetch_add(1); i < size; i = counter.fetch_add(1)) {
    float base = result[i];
    for (int w = 0; w < mult[i]-1; w++) {
      result[i] = result[i]+base;
    }
  }
}

void launch_threads(float* result_parallel, int* mult) {

  std:: vector<std:: thread> threads(NUM_THREADS);

  // launch all threads
  for (int i = 0; i < threads.size(); i++) {
    threads[i] = std::thread(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
  }

  // join threads when done with work
  for (int i = 0; i < threads.size(); i++) {
    threads[i].join();
  }

}