//
// Created by corsi on 16.09.2025.
//

#ifndef STACK_H
#define STACK_H

#include <memory>

template <typename T>
struct Node {
  T value;
  std::unique_ptr<Node> next;

  explicit Node(const T& val) : value(val), next(nullptr) {
  }

  explicit Node(T&& val) : value(std::move(val)), next(nullptr) {
  }
};

template <typename T>
class Stack {
  size_t size_;
  std::unique_ptr<Node<T> > top_;

public:
  Stack() : size_(0), top_(nullptr) {
  }

  T& top() const {
    return top_->value;
  }

  bool empty() const {
    return size_ == 0;
  }

  void push(const T& val) {
    std::unique_ptr<Node<T>> node = std::make_unique<Node<T>>(val);

    if (top_ == nullptr) {
      top_ = std::move(node);
      size_++;
      return;
    }

    node->next = std::move(top_);
    top_ = std::move(node);
    size_++;
  }

  void pop() {
    if (top_ == nullptr) {
      return;
    }
    auto node = std::move(top_->next);
    top_.reset();
    top_ = std::move(node);
    size_--;
  }
};
#endif //STACK_H