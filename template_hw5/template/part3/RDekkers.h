#include <atomic>

class dekkers_mutex {
private:
  std:: atomic<int> turn;
  std:: atomic<int> wants_to_enter[2];
public:
  dekkers_mutex() {
    turn.store(0, std::memory_order_relaxed); // init turn to thread 0;

    // init both threads to not interested
    wants_to_enter[0].store(0, std::memory_order_relaxed);
    wants_to_enter[1].store(0, std::memory_order_relaxed);
  }

  void lock(int tid) {
    // set my intent to enter
    wants_to_enter[tid].store(1, std::memory_order_relaxed);

    // get other threads id
    int other;
    if (tid == 0) other = 1;
    if (tid == 1) other = 0;

    // Keep looping while other also wants to enter
    while (wants_to_enter[other].load(std::memory_order_relaxed) == 1) {

      // if the turn isnt for me,
      if (turn.load(std::memory_order_relaxed) != tid) {
        wants_to_enter[tid].store(0, std::memory_order_relaxed); // take my intent away

        while (turn.load(std::memory_order_relaxed) != tid) {} // wait until its my turn

        wants_to_enter[tid].store(1, std::memory_order_relaxed); // request my intent again
      }
    }
  }

  void unlock(int tid) {
    // calculate other threads
    int other;
    if (tid == 0) other = 1;
    if (tid == 1) other = 0;

    // give other thread its turn
    turn.store(other, std::memory_order_relaxed);

    // remove my intent
    wants_to_enter[tid].store(0, std::memory_order_relaxed);
  }
};
