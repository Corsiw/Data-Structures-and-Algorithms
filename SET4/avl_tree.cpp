#include "avl_tree.h"

#include <algorithm>

Node::Node(int value) : height(1), left(nullptr), right(nullptr), value(value) {
}

Node::~Node() {
  delete left;
  delete right;
}

AVLTree::AVLTree(): root_(nullptr), size_(0) {
}

AVLTree::AVLTree(int value): root_(new Node(value)), size_(1) {
}

int AVLTree::getHeight() {
  if (!root_) {
    return 0;
  }
  return root_->height;
}

void AVLTree::insert(int value) {
  if (root_ == nullptr) {
    root_ = new Node(value);
    size_ = 1;
    return;
  }
  root_ = insertNode(root_, value);
}

void AVLTree::erase(int value) {
  root_ = removeNode(root_, value);
}

int* AVLTree::find(int value) {
  if (!root_) {
    return nullptr;
  }
  Node* node = findNode(root_, value);
  return node ? &node->value : nullptr;
}

int* AVLTree::traversal() {
  if (!root_) {
    return nullptr;
  }

  int* arr = new int[size_];
  int index = 0;
  traversalInternal(root_, arr, &index);
  return arr;
}

int* AVLTree::lowerBound(int value) {
  auto node = lowerBoundInternal(root_, value);
  return node ? &node->value : nullptr;
}

bool AVLTree::empty() {
  return size_ == 0;
}

Node* AVLTree::getRoot() {
  return root_;
}

int AVLTree::getSize() {
  return size_;
}

AVLTree::~AVLTree() {
  delete root_;
}

int AVLTree::getNodeHeight(Node* node) {
  return node ? node->height : 0;
}

int AVLTree::balanceFactor(Node* node) {
  if (!node) {
    return 0;
  }
  int leftHeight = node->left ? node->left->height : 0;
  int rightHeight = node->right ? node->right->height : 0;
  return leftHeight - rightHeight;
}

void AVLTree::balanceHeight(Node* node) {
  if (!node)
    return;

  int leftHeight = node->left ? node->left->height : 0;
  int rightHeight = node->right ? node->right->height : 0;

  node->height = 1 + std::max(leftHeight, rightHeight);
}

Node* AVLTree::rotateRight(Node* node) {
  auto newRoot = node->left;
  auto newRootRight = newRoot->right;

  newRoot->right = node;
  node->left = newRootRight;

  balanceHeight(node);
  balanceHeight(newRoot);

  return newRoot;
}

Node* AVLTree::rotateLeft(Node* node) {
  auto newRoot = node->right;
  auto newRootLeft = newRoot->left;

  newRoot->left = node;
  node->right = newRootLeft;

  balanceHeight(node);
  balanceHeight(newRoot);

  return newRoot;
}

Node* AVLTree::balanceNode(Node* node) {
  balanceHeight(node);

  const int factor = balanceFactor(node);

  if (factor == 2) {
    if (balanceFactor(node->left) < 0)
      node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  if (factor == -2) {
    if (balanceFactor(node->right) > 0)
      node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

Node* AVLTree::insertNode(Node* node, int value) {
  if (!node) {
    size_++;
    return new Node(value);
  }

  if (node->value == value) {
    return node;
  }

  if (value < node->value) {
    node->left = insertNode(node->left, value);
  } else {
    node->right = insertNode(node->right, value);
  }

  balanceHeight(node);
  return balanceNode(node);
}

Node* AVLTree::findMinNode(Node* node) {
  return node->left ? findMinNode(node->left) : node;
}

Node* AVLTree::removeMinNode(Node* node) {
  if (!node->left) {
    Node* right = node->right;
    delete node;
    size_--;
    return right;
  }
  node->left = removeMinNode(node->left);
  return balanceNode(node);
}

Node* AVLTree::removeNode(Node* node, int value) {
  if (!node)
    return nullptr;

  if (value < node->value)
    node->left = removeNode(node->left, value);
  else if (value > node->value)
    node->right = removeNode(node->right, value);
  else {
    if (!node->left && !node->right) {
      delete node;
      size_--;
      return nullptr;
    }

    if (!node->left || !node->right) {
      Node* child = node->left ? node->left : node->right;
      delete node;
      size_--;
      return child;
    }

    const Node* minNode = findMinNode(node->right);
    node->value = minNode->value;
    node->right = removeNode(node->right, minNode->value);
  }

  balanceHeight(node);
  return balanceNode(node);
}

Node* AVLTree::findNode(Node* node, int value) {
  if (!node) {
    return nullptr;
  }
  if (node->value == value) {
    return node;
  }

  if (value < node->value) {
    return findNode(node->left, value);
  }
  return findNode(node->right, value);
}

void AVLTree::traversalInternal(Node* node, int* array, int* index) {
  if (node->left) {
    traversalInternal(node->left, array, index);
  }
  array[*index] = node->value;
  *index += 1;

  if (node->right) {
    traversalInternal(node->right, array, index);
  }
}

Node* AVLTree::lowerBoundInternal(Node* current, int value) const {
  Node* result = nullptr;

  while (current) {
    if (current->value >= value) {
      result = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }

  return result;
}