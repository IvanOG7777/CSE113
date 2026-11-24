#include <atomic>
#include <vector>

class barrier_object {
private:
  int total_threads;
  std:: atomic<int> count; // amount of threads remaining the barrier
  std::atomic<bool> global_sense; // global flag that is flipped by last thread
  std:: vector<int> local_senses; // local flags per thread

 public:
  barrier_object() {
    global_sense = false;
    total_threads = 0;
  }

  void init(int num_threads) {
    count.store(num_threads);
    total_threads = num_threads;
    global_sense = false;
    local_senses.assign(count.load(), 0);
  }

  void barrier(int tid) {
    int local_sense = local_senses[tid];
    local_senses[tid] = !local_sense; // flip local flag

    // if current thread is the last thread
    if (count.fetch_sub(1) == 1) {
      count.store(total_threads); // reset counter
      global_sense.store(!local_sense);
    } else {
      while (global_sense.load() == )
    }
  }
};
