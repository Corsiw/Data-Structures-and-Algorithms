#include "btree.h"

#include <algorithm>

constexpr size_t kSmall = 8;

Node::Node(int t) : key(), children(), parent(nullptr), isLeaf(true), t(t) {
  key.reserve(2 * t - 1);
  children.reserve(2 * t);
}

BTree::BTree(int t) : root(nullptr), t_(t), size_(0) {
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

bool Contains(const Node* node, const int key) {
  for (const int v : node->key) {
    if (v == key) {
      return true;
    }
  }
  return false;
}

bool ContainsTree(const Node* node, const int key) {
  while (true) {
    if (Contains(node, key)) {
      return true;
    }
    if (node->isLeaf) {
      break;
    }

    size_t i = 0;
    while (i < node->key.size() && key > node->key[i]) {
      i++;
    }
    node = node->children[i];
  }
  return false;
}

void SplitChild(Node* parent, const int pos, Node* node) {
  const int t = node->t;
  const auto new_node = new Node(t);
  new_node->isLeaf = node->isLeaf;

  for (int j = 0; j < t - 1; ++j) {
    new_node->key.push_back(node->key[j + t]);
  }

  if (!node->isLeaf) {
    for (int j = 0; j < t; ++j) {
      new_node->children.push_back(node->children[j + t]);
      new_node->children.back()->parent = new_node;
    }
  }

  node->key.resize(t - 1);
  if (!node->isLeaf) {
    node->children.resize(t);
  }

  parent->children.insert(parent->children.begin() + pos + 1, new_node);
  new_node->parent = parent;

  parent->key.insert(parent->key.begin() + pos, node->key[t - 1]);
}

void InsertNotFullNode(Node* node, const int new_key, int& s) {
  if (node->isLeaf) {
    const auto it = std::ranges::lower_bound(node->key, new_key);
    node->key.insert(it, new_key);
    return;
  }
  int i = static_cast<int>(node->key.size()) - 1;
  while (i >= 0 && new_key < node->key[i]) {
    i--;
  }
  i++;

  Node* child = node->children[i];

  if (static_cast<int>(child->key.size()) == 2 * node->t - 1) {

    SplitChild(node, i, child);
    s += 2;
    if (new_key > node->key[i]) {
      i++;
    }
  }
  InsertNotFullNode(node->children[i], new_key, s);
}

void BTree::insert(int key) {
  if (root == nullptr) {
    root = new Node(t_);
    root->key.push_back(key);
    size_ = 1;
    return;
  }

  if (ContainsTree(root, key)) {
    return;
  }

  if (root->key.size() == static_cast<size_t>(2 * t_) - 1) {
    auto s = new Node(t_);
    s->isLeaf = false;
    s->children.push_back(root);
    root->parent = s;

    SplitChild(s, 0, root);
    size_ += 2;
    root = s;
  }

  InsertNotFullNode(root, key, size_);
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