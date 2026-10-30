#pragma once

#include <atomic>
#include <mutex>
using namespace std;

class rw_mutex {
private:
  mutex internal_mutex;
  bool writer;
  int num_readers;

 public:
  rw_mutex() {
    writer = false;
    num_readers = 0;
  }

  void lock_reader() {
    bool acquired = false;
    while (!acquired) { // while not acquired
      internal_mutex.lock(); // lock mutex
      if (!writer) { // if no writer
        num_readers++; // add to num of readers
        acquired = true; // set flag to true
      }
      internal_mutex.unlock(); // unlock for next thread
    }
  }
  
  void unlock_reader() {
    internal_mutex.lock();
    num_readers--;
    internal_mutex.unlock();
  }

  
  void lock() {
    bool acquired = false;
    // keep looping when not acquired
    while (!acquired) {
      internal_mutex.lock(); // keep mutex locked when acquired
      if (!writer && num_readers == 0) {
        writer = true;
        acquired = true;
      }
      internal_mutex.unlock();
    }
  }
  
  void unlock() {
    internal_mutex.lock();
    writer = false;
    internal_mutex.unlock();
  }
};
