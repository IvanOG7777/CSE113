#include <shared_mutex>
using namespace std;

class Llist_node {
 public:  
  Llist_node(int d) {   
    data = d;
    next = NULL;
  }
  
  int data;
  Llist_node *next;
};

class CSE113_Stack {
private:
  Llist_node *start;
  std:: shared_mutex swap_top_stack_mutex;


 public:
  
  CSE113_Stack() {
    start = NULL;
  }
  
  int pop() {
    swap_top_stack_mutex.lock();
    if (start == NULL) {
      swap_top_stack_mutex.unlock();
      return -1;
    }

    if (start->next == NULL) {
      int ret = start->data;
      delete start;
      start = NULL;
      swap_top_stack_mutex.unlock();
      return ret;
    }

    Llist_node *current = start->next;
    Llist_node *previous = start;
    while (current->next != NULL) {
      previous = current;
      current = current->next;
    }

    int ret = current->data;
    previous->next = NULL;
    delete current;

    swap_top_stack_mutex.unlock();

    return ret;
  }

  
  int peek() {
    swap_top_stack_mutex.lock_shared();
    if (start == NULL) {
      swap_top_stack_mutex.unlock_shared();
      return -1;
    }
    
    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    int value = current->data;
    swap_top_stack_mutex.unlock_shared();
    return value;
  }
  
  void push(int p) {
    swap_top_stack_mutex.lock();
    if (start == NULL) {

      start = new Llist_node(p);

      swap_top_stack_mutex.unlock();
      return;
    }

    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    current->next = new Llist_node(p);

    swap_top_stack_mutex.unlock();
  }

  void swaptop(int to_swap) {
    int top = peek();
    if (top != to_swap) {
      swap_top_stack_mutex.lock();

      if (start == NULL) {
        swap_top_stack_mutex.unlock();
        return;
      }

      Llist_node *current = start;
      while (current->next != NULL) {
        current = current->next;
      }

      current->data = to_swap;
      swap_top_stack_mutex.unlock();
    }
  }
};
