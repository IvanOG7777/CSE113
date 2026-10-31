#pragma once
#include <atomic>

class IOQueue {
private:
  std:: atomic<int> head;
  std:: atomic<int> tail;
  int *buffer;
  int buffer_size;

public:
  IOQueue() {
    head.store(0);
    tail.store(0);
    buffer = nullptr;
    buffer_size = 0;
  }

  ~IOQueue() {
    delete[] buffer;
  }


  // Use this function to initialize the queue to
  // the size that you need.
  void init(int size) {
    // if buffer hasn't been initialized
    if (buffer == nullptr) {
      buffer = new int[size];
      buffer_size = size;
    } else {
      // delete old buffer allocate new space and reset head/tail
      delete[] buffer;
      buffer = new int[size];
      buffer_size = size;
      head.store(0);
      tail.store(0);
    }
  }

  // enqueue the element e into the queue
  void enq(int e) {

    if (tail.load() >= buffer_size) return; // check if tail is greater than or equal to buffer size

    int buffer_index = tail.load() % buffer_size; // calculate index

    buffer[buffer_index] = e; // input element at calculated index

    tail.fetch_add(1); // atomically fetch and add
  }

  // return a value from the queue.
  // return -1 if there is no more values from the queue.
  int deq() {

    while (true) {
      int current_head = head.load(); // load in current fetched head

      if (current_head >= tail.load()) return -1; // if true return 1 (nno more elements)

      // if head.load() is equal to current head, swap head with current_head + 1 annd enter
      if (head.compare_exchange_weak(current_head,current_head + 1)) {
        return buffer[current_head % buffer_size]; // return buffer value
      }
    }
  }

  // return 32 values from the queue. The
  // values should be returned through
  // the array passed in.
  // return 0 if successful. return 1 if not
  // i.e. if the queue does not have 32 elements
  int deq_32(int ret[32]) {
    return -1;
  }

};
