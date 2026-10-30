#include <shared_mutex>
using namespace std;

class Llist_node {
 public:
  int data;
  Llist_node *next;

  Llist_node(int d) {   
    data = d;
    next = NULL;
  }
};

class CSE113_Stack {
private:
  Llist_node *start;
  std:: shared_mutex shared_stack_mutex;

 public:
  
  CSE113_Stack() {
    start = NULL;
  }
  
  int pop() {
    shared_stack_mutex.lock();
    if (start == NULL) {
      shared_stack_mutex.unlock();
      return -1;
    }

    if (start->next == NULL) {
      int ret = start->data;
      delete start;
      start = NULL;

      shared_stack_mutex.unlock();
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

    shared_stack_mutex.unlock();
    return ret;
  }

  
  int peek() {
    shared_stack_mutex.lock_shared();
    if (start == NULL) {
      shared_stack_mutex.unlock_shared();
      return -1;
    }
    
    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    int value = current->data;

    shared_stack_mutex.unlock_shared();

    return value;
  }
  
  void push(int p) {
    shared_stack_mutex.lock();
    if (start == NULL) {

      start = new Llist_node(p);
      shared_stack_mutex.unlock();
      return;
    }

    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    current->next = new Llist_node(p);

    shared_stack_mutex.unlock();
  }
};

