#pragma once

#include <atomic>
#include <thread>
#include <vector>

class mutex {
private:

 public:
  mutex() {
    // Implement me!
  }
  
  void init (int num_threads) {
    // Implement me!
  }
  
  void lock(int thread_id) {
    // Implement me!
  }
  
  void unlock(int thread_id) {
    // Implement me!
  }
};
