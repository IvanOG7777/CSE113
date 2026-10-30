#include <atomic>

#define CQUEUE_SIZE 2048

class CQueue {
private:
  std::atomic<int> head;
  std::atomic<int> tail;
  float buffer[CQUEUE_SIZE]{};

 public:

  CQueue() {
    head.store(0);
    tail.store(0);
  }

  void enq(float e) {

    // wait while buffer is full
    while (tail.load() - head.load() == CQUEUE_SIZE) {}

    // calculate index of where to write
    const int index = tail.load() % CQUEUE_SIZE;
    // ex tail = 14, 14 % 2048 = 14
    // ex tail = 3000, 3000 % 2048 = 952

    // load element into buffer
    buffer[index] = e;

    // add 1 to tail and load
    int new_tail = tail.load() + 1;
    tail.store(new_tail);
  }

  float deq() {

    // wait while there are no elements in the buffer
    while (tail.load() == head.load()) {};

    // Calculate index of where to read from
    const int index = head.load() % CQUEUE_SIZE;

    float value = buffer[index]; // get the value

    // add 1 to head and store
    int new_head = head.load() + 1;
    head.store(new_head);

    return value; // return value
  }

  int size() {
    return tail.load() - head.load(); // ex tail = 120, head = 80, 120-80 = 40 elements in buffer
  }

  void enq_8(float e[8]) {

    while (CQUEUE_SIZE - (tail.load() - head.load()) < 8) {}; // wait while has less than 8 slots available

    int buffer_index = tail.load() % CQUEUE_SIZE; // calculate where to place items at in buffer

    for (int i = 0; i < 8; i++) {

      if (buffer_index == CQUEUE_SIZE) buffer_index = 0; // wrap back around buffer

      buffer[buffer_index++] = e[i]; // place item
    }

    int new_tail = tail.load() + 8;

    tail.store(new_tail);
  }

  void deq_8(float e[8]) {

    while (tail.load() - head.load() < 8) {}; // wait while less than 8 slots are available to read

    int buffer_index = head.load() % CQUEUE_SIZE; // calculate where to get items at in buffer

    for (int i = 0; i < 8; i++) {

      if (buffer_index == CQUEUE_SIZE) buffer_index = 0; // wrap back around buffer

      e[i] = buffer[buffer_index++]; // load buffer_index++ to e[i]
    }

    // load old head and att 8 to it for new index and store
    int new_head = head.load() + 8;
    head.store(new_head);
  }
};
