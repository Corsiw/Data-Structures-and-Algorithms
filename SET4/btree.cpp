#include "btree.h"

#include <algorithm>

constexpr size_t kSmall = 8;

Node::Node(int t) : key(), children(), parent(nullptr), isLeaf(true), t(t) {
  key.reserve(std::min<size_t>(std::max(1, 2 * t - 1), kSmall));
  children.reserve(std::min<size_t>(std::max(1, 2 * t), kSmall));
}

BTree::BTree(int t) : root(new Node(t)), t_(t), size_(0) {
}

void DeleteSubtree(const Node* node) {
  if (!node) {
    return;
  }
  for (auto child : node->children) {
    DeleteSubtree(child);
  }
  delete node;
}

BTree::~BTree() {
  DeleteSubtree(root);
}

Node* Split(Node*& node, int*& key) {
  const int t = node->t;
  const auto new_node = new Node(t);
  new_node->isLeaf = node->isLeaf;
  *key = node->key[t-1];

  for (int i = t; i < 2 * t; i++) {
    new_node->key.push_back(node->key[i]);
  }
  node->key.resize(t-1);

  if (!node->isLeaf) {
    for (int i = t; i < 2 * t + 1; i++) {
      new_node->children.push_back(node->children[i]);
    }
    node->children.resize(t);
  }
  return new_node;
}

Node* InsertNode(Node*& node, int*& key) {
  if (!node->isLeaf) {
    size_t i = 0;
    while (i < node->key.size() && *key > node->key[i]) {
      i++;
    }
    if (*key == node->key[i]) {
      key = nullptr;
      return nullptr;
    }

    const auto new_node = InsertNode(node->children[i], key);
    if (new_node) {
      node->children.push_back(new_node);
      node->isLeaf = false;
    }
  }

  if (key == nullptr) {
    return nullptr;
  }

  const int t = node->t;
  if (std::find(node->key.begin(), node->key.end(), *key) != node->key.end()) {
    key = nullptr;
    return nullptr;
  }
  node->key.push_back(*key);
  size_t i = node->key.size() - 1;

  while (i > 0 && node->key[i] < node->key[i - 1]) {
    std::swap(node->key[i - 1], node->key[i]);
    std::swap(node->children[i], node->children[i + 1]);
    i--;
  }

  if (static_cast<int>(node->key.size()) <= 2 * t - 1) {
    key = nullptr;
    return nullptr;
  }

  const auto new_node = Split(node, key);
  return new_node;
}

void BTree::insert(int key) {
  if (root == nullptr) {
    root = new Node(t_);
    root->key.push_back(key);
    return;
  }

  int* k = &key;

  if (const auto new_node = InsertNode(root, k)) {
    auto parent = new Node(t_);
    parent->key.push_back(*k);
    parent->children.push_back(root);
    parent->children.push_back(new_node);
    root = parent;
    parent->isLeaf = false;
  }
}

size_t SizeNode(const Node* node) {
  if (!node) {
    return 0;
  }

  if (node->isLeaf) {
    return 1;
  }

  size_t sum = 0;
  for (const auto child : node->children) {
    sum += SizeNode(child);
  }

  sum++;
  return sum;
}

size_t BTree::size() const {
  return SizeNode(root);
}

int64_t SumNode(const Node* node) {
  if (!node) {
    return 0;
  }

  if (node->isLeaf) {
    int64_t sum = 0;
    for (const auto key : node->key) {
      sum += key;
    }
    return sum;
  }

  int64_t sum = 0;
  for (const auto child : node->children) {
    sum += SumNode(child);
  }
  return sum;
}

int64_t BTree::sum() const {
  return SumNode(root);
}