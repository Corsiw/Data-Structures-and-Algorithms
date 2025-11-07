#include "Queue.h"

#include <stdexcept>

void Queue::push(int val) {
  int mn = val;
  if (!pushStack_.empty()) {
    mn = std::min(mn, pushStack_.top().second);
  }
  pushStack_.push({val, mn});
  last_ = &pushStack_.top().first;
}

int Queue::pop() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }

  if (popStack_.empty()) {
    transfer();
  }

  const int val = popStack_.top().first;
  popStack_.pop();

  if (empty()) {
    last_ = nullptr;
  }
  return val;
}

int Queue::currentMin() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }

  int mn = INT_MAX;
  if (!pushStack_.empty()) {
    mn = pushStack_.top().second;
  }

  if (!popStack_.empty()) {
    mn = std::min(mn, popStack_.top().second);
  }
  return mn;
}

int& Queue::back() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }
  if (!pushStack_.empty()) {
    return pushStack_.top().first;
  }
  return *last_;
}

int& Queue::front() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }

  if (popStack_.empty()) {
    transfer();
  }
  return popStack_.top().first;
}

bool Queue::empty() const {
  return popStack_.empty() && pushStack_.empty();
}

void Queue::transfer() {
  const int val = pushStack_.top().first;
  pushStack_.pop();
  popStack_.push({val, val});
  last_ = &popStack_.top().first;

  while (!pushStack_.empty()) {
    const int value = pushStack_.top().first;
    pushStack_.pop();
    popStack_.push({value, std::min(value, popStack_.top().second)});
  }
}