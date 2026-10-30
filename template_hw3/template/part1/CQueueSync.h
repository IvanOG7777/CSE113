#include <atomic>
#include <queue>

class CQueue {
private:
  float value;
  std::atomic<bool> has_value;

 public:

  CQueue() {
    value = 0.0f;
    has_value = false;
  }

  void enq(float e) {

    while (has_value.load() == true) {} // wait until no value in queue

    // load value and set has_value to true
    value = e;
    has_value.store(true);

    while (has_value.load() == true) {} // wait until consumer takes item from queue
  }

  float deq() {

    while (has_value.load() == false) {} // wait while no value is in the queue

    // consume value and set has_value to false
    float local_value = value;
    has_value.store(false);

    return local_value;
  }
};
