#include <iostream>
using std::cout;
using std::cin;

template <class T>
struct Node {
  T data;
  Node* right;
  Node* left;
  Node* parent;

  Node(T val = {}) {
    data = val;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
  }
};

template <class T>
class Tree {
public:
  Tree() {
    root_ = new Node<T>();
  }

  void insert(int value) {
    if (root_->data == 0) {
      root_->data = value;
      return;
    }

    Node<T>* current = root_;
    Node<T>* parent = nullptr;

    while (current != nullptr) {
      if (value < current->data) {
        parent = current;
        current = current->left;
      } else if (value > current->data) {
        parent = current;
        current = current->right;
      } else {
        return;
      }
    }

    current = new Node<T>(value);
    current->parent = parent;

    if (value < parent->data) {
      parent->left = current;
    } else if (value > parent->data) {
      parent->right = current;
    }
  }

  struct Iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Node<T>;
    using pointer = Node<T>*; // or also value_type*
    using reference = Node<T>&;


    Iterator(pointer ptr) : ptr_(ptr) {
    }

    reference operator*() {
      return *ptr_;
    }

    pointer operator->() {
      return ptr_;
    }

    Iterator& operator--() {
      if (ptr_->left) {

        ptr_ = ptr_->left;

        while (ptr_->right) {
          ptr_ = ptr_->right;
        }

        return *this;
      } else {
        pointer prnt = ptr_->parent;

        while (prnt != nullptr && prnt->left == ptr_) {
          ptr_ = prnt;
          prnt = prnt->parent;
        }

        ptr_ = prnt;

        return *this;
      }
    }

    Iterator& operator++() {
      // Tree succesor
      // реализовать на паре
      if (ptr_->right) {

        ptr_ = ptr_->right;

        while (ptr_->left) {
          ptr_ = ptr_->left;
        }

        return *this;
      } else {
        pointer prnt = ptr_->parent;

        while (prnt != nullptr && prnt->right == ptr_) {
          ptr_ = prnt;
          prnt = prnt->parent;
        }

        ptr_ = prnt;

        return *this;
      }
    }

    // Необходимые для пользования итераторами методы
    friend bool operator==(const Iterator& it1, const Iterator& it2) {
      return (it1.ptr_ == it2.ptr_);
    }

    friend bool operator!=(const Iterator& it1, const Iterator& it2) {
      return (it1.ptr_ != it2.ptr_);
    }

  private:
    pointer ptr_;

  };

  // Минимум, начало дерева
  Iterator begin() {
    if (root_->data == 0) {
      return Iterator(nullptr);
    }

    Node<T>* cur = root_;

    while (cur->left) {
      cur = cur->left;
    }

    return Iterator(cur);
  }

  Iterator rbegin() {
    if (root_->data == 0) {
      return Iterator(nullptr);
    }

    Node<T>* cur = root_;

    while (cur->right) {
      cur = cur->right;
    }

    return Iterator(cur);
  }

  Iterator end() {
    return Iterator(nullptr);
  }

  Iterator rend() {
    return Iterator(nullptr);
  }

private:
  Node<T>* root_;

};

int main() {
  // Это просто данные с семинара
  Tree<int> tree;
  tree.insert(5);
  tree.insert(3);
  tree.insert(10);
  tree.insert(1);
  tree.insert(4);
  tree.insert(2);
  tree.insert(9);
  tree.insert(13);
  tree.insert(11);
  tree.insert(14);

  for (Tree<int>::Iterator it = tree.begin(); it != tree.end(); ++it) {
    std::cout << it->data << ' ';
  }
  std::cout << '\n';

  for (Tree<int>::Iterator it = tree.rbegin(); it != tree.rend(); --it) {
    std::cout << it->data << ' ';
  }
  std::cout << '\n';
}