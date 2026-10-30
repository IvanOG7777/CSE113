#include <mutex>
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
  std:: mutex stack_mutex;

 public:
  
  CSE113_Stack() {
    start = NULL;
  }
  
  int pop() {
    stack_mutex.lock();
    // if no head return -1
    if (start == NULL) {
      stack_mutex.unlock();
      return -1;
    }

    // if heads next is null
    if (start->next == NULL) {
      int ret = start->data; // get heads data
      delete start; // delete node
      start = NULL; // make node null
      stack_mutex.unlock();
      return ret; // return value
    }

    // make current, heads next node
    Llist_node *current = start->next;
    Llist_node *previous = start; // make previous the head node

    // while current next isnt null
    while (current->next != NULL) {
      previous = current;
      current = current->next;
    }

    // get the final nodes data
    int ret = current->data;

    // delete and set to null
    previous->next = NULL;
    delete current;

    stack_mutex.unlock();

    // return value
    return ret;
  }

  
  int peek() {
    stack_mutex.lock();

    // if head is null return error
    if (start == NULL) {
      stack_mutex.unlock();
      return -1;
    }

    // make current head
    Llist_node *current = start;
    // while current's next inst null
    while (current->next != NULL) {
      current = current->next;
    }

    // get current nodes data before returning
    int value = current->data;

    stack_mutex.unlock();

    // return final nodes data
    return value;
  }
  
  void push(int p) {
    stack_mutex.lock();
    // if no head
    if (start == NULL) {
      // create new starting node
      start = new Llist_node(p);
      stack_mutex.unlock();
      return;
    }

    // else loop nodes next node until null
    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    // make last nodes next point to a new node.
    current->next = new Llist_node(p);

    stack_mutex.unlock();
  }
};

