#ifndef QUEUE_H
#define QUEUE_H

#include "Stack.h"

class Queue {
public:
  Queue() = default;

  void push(int val);

  int pop();

  int& front();

  int& back();

  int currentMin();

  bool empty() const;

private:
  Stack<std::pair<int, int> > pushStack_;
  Stack<std::pair<int, int> > popStack_;
  int* last_ = nullptr;

  void transfer();
};
#endif //QUEUE_H