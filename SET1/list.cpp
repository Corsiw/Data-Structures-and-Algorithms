#include "list.h"

#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

List::List() : _size(0), head(nullptr), tail(nullptr) {
}

List::List(const List& other) : _size(other._size), head(nullptr), tail(nullptr) {
  if (this == &other) {
    return;
  }

  if (other._size == 0) {
    return;
  }

  const auto dummy = new Node();

  const Node* cur = other.head;
  Node* new_cur = dummy;

  while (cur != nullptr) {
    const auto new_node = new Node(cur->value);
    new_node->prev = new_cur;
    new_cur->next = new_node;

    new_cur = new_cur->next;
    cur = cur->next;
  }

  head = dummy->next;
  head->prev = nullptr;
  tail = new_cur;
}

List::List(const std::vector<int> array): _size(array.size()), head(nullptr), tail(nullptr) {
  if (array.empty()) {
    return;
  }

  const auto dummy = new Node();
  Node* new_cur = dummy;

  for (size_t i = 0; i < array.size(); i++) {
    const auto new_node = new Node(array[i]);
    new_node->prev = new_cur;
    new_cur->next = new_node;

    new_cur = new_cur->next;
  }

  head = dummy->next;
  head->prev = nullptr;
  tail = new_cur;
}

List::~List() {
  if (!check_cycle()) {
    clear();
  } else {
    std::unordered_set<Node*> visited;
    Node* current = head;

    while (current != nullptr) {
      if (visited.find(current) != visited.end()) {
        break;
      }

      visited.insert(current);
      Node* temp = current;
      current = current->next;

      temp->prev = nullptr;
      temp->next = nullptr;
      delete temp;
    }

    head = nullptr;
    tail = nullptr;
    _size = 0;
  }
}

void List::push_back(const int value) {
  const auto new_node = new Node(value);
  new_node->prev = tail;
  if (tail != nullptr) {
    tail->next = new_node;
  }

  if (_size == 0) {
    head = new_node;
  }
  tail = new_node;
  _size++;
}

void List::push_front(const int value) {
  const auto new_node = new Node(value);
  new_node->next = head;
  if (head != nullptr) {
    head->prev = new_node;
  }

  if (_size == 0) {
    tail = new_node;
  }
  head = new_node;
  _size++;
}

void List::insert(Node* pos, const int value) {
  if (pos == nullptr) {
    throw std::runtime_error("Incorrect position!");
  }

  if (pos == tail) {
    push_back(value);
    return;
  }

  bool in_list = false;
  const Node* cur = this->head;
  while (cur != nullptr) {
    if (cur == pos) {
      in_list = true;
      break;
    }
    cur = cur->next;
  }
  if (!in_list) {
    throw std::runtime_error("Incorrect position!");
  }

  const auto new_node = new Node(value);
  new_node->prev = pos;
  new_node->next = pos->next;

  if (pos->next != nullptr) {
    pos->next->prev = new_node;
  }
  pos->next = new_node;
  _size++;
}

void List::pop_back() {
  if (_size == 0) {
    throw std::runtime_error("Deleting in empty list");
  }

  const auto pr = this->tail->prev;
  delete tail;
  tail = pr;
  _size--;

  if (_size == 0) {
    head = nullptr;
  } else {
    tail->next = nullptr;
  }
}

void List::pop_front() {
  if (_size == 0) {
    throw std::runtime_error("Deleting in empty list");
  }

  const auto nx = this->head->next;
  delete head;
  head = nx;
  _size--;

  if (_size == 0) {
    tail = nullptr;
  } else {
    head->prev = nullptr;
  }
}

void List::erase(Node* pos) {
  if (pos == nullptr) {
    throw std::runtime_error("Incorrect position!");
  }
  if (pos == tail) {
    pop_back();
    return;
  }
  if (pos == head) {
    pop_front();
    return;
  }

  pos->prev->next = pos->next;
  pos->next->prev = pos->prev;
  _size--;
  delete pos;
}

int List::front() {
  if (_size == 0) {
    throw std::runtime_error("Empty list");
  }
  return head->value;
}

int List::back() {
  if (_size == 0) {
    throw std::runtime_error("Empty list");
  }
  return tail->value;
}

bool List::empty() const {
  return _size == 0;
}

bool List::check_cycle() const {
  if (_size <= 1) {
    return false;
  }

  const Node* slow = head;
  const Node* fast = head->next;
  while (fast->next != nullptr && fast->next->next != nullptr) {
    if (slow == fast) {
      return true;
    }
    fast = fast->next->next;
    slow = slow->next;
  }
  return false;

}

size_t List::size() const {
  return _size;
}

void List::clear() {
  Node* cur = head;
  while (cur != nullptr) {
    Node* tmp = cur->next;
    delete cur;
    cur = tmp;
  }
  _size = 0;
  head = nullptr;
  tail = nullptr;
}

void List::reverse() {
  if (_size == 0) {
    return;
  }

  Node* cur = head;
  while (cur != nullptr) {
    const auto temp = cur->next;
    cur->next = cur->prev;
    cur->prev = temp;
    cur = cur->prev;
  }
  const auto tmp = head;
  head = tail;
  tail = tmp;
}

void List::remove_duplicates() {
  std::unordered_map<int, int> duplicates;

  Node* cur = head;
  while (cur != nullptr) {
    const auto new_cur = cur->next;
    if (duplicates[cur->value] >= 1) {
      erase(cur);
    } else {
      duplicates[cur->value]++;
    }
    cur = new_cur;
  }
}

void List::replace(const int old_value, const int new_value) {
  Node* cur = head;
  while (cur != nullptr) {
    auto new_cur = cur->next;
    if (cur->value == old_value) {
      insert(cur, new_value);
      new_cur = cur->next;
      erase(cur);
    }
    cur = new_cur;
  }
}

void List::copy(const List& other) {
  if (this == &other) {
    return;
  }

  clear();

  if (other._size == 0) {
    return;
  }

  const auto dummy = new Node();

  const Node* cur = other.head;
  Node* new_cur = dummy;

  while (cur != nullptr) {
    const auto new_node = new Node(cur->value);
    new_node->prev = new_cur;
    new_cur->next = new_node;

    new_cur = new_cur->next;
    cur = cur->next;
  }

  head = dummy->next;
  head->prev = nullptr;
  tail = new_cur;
  _size = other._size;
}

void List::merge(const List& other) {
  if (other._size == 0) {
    return;
  }

  Node* cur = tail;
  const Node* other_cur = other.head;

  while (other_cur != nullptr) {
    const auto new_node = new Node(other_cur->value);
    cur->next = new_node;
    new_node->prev = cur;

    cur = new_node;
    other_cur = other_cur->next;
  }
  _size += other._size;
  tail = cur;
}