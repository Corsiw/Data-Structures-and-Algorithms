#include "rbtree.h"

#include <algorithm>

void balanceHeight(Node* node) {
  if (!node)
    return;

  const int leftHeight = node->left ? node->left->height : 0;
  const int rightHeight = node->right ? node->right->height : 0;

  node->height = 1 + std::max(leftHeight, rightHeight);
}

void balanceSize(Node* node) {
  if (!node)
    return;

  const int leftSize = node->left ? node->left->size : 0;
  const int rightSize = node->right ? node->right->size : 0;

  node->size = 1 + leftSize + rightSize;
}

Node* rotateRight(Node* node) {
  const auto newRoot = node->left;
  const auto newRootRight = newRoot->right;

  newRoot->right = node;
  node->left = newRootRight;

  if (newRoot->right) {
    newRoot->right->parent = newRoot;
  }

  newRoot->parent = node->parent;
  node->parent = newRoot;

  balanceSize(node);
  balanceHeight(node);
  balanceSize(newRoot);
  balanceHeight(newRoot);

  return newRoot;
}

Node* rotateLeft(Node* node) {
  const auto newRoot = node->right;
  const auto newRootLeft = newRoot->left;

  newRoot->left = node;
  node->right = newRootLeft;

  if (newRoot->left) {
    newRoot->left->parent = newRoot;
  }

  newRoot->parent = node->parent;
  node->parent = newRoot;

  balanceSize(node);
  balanceHeight(node);
  balanceSize(newRoot);
  balanceHeight(newRoot);

  return newRoot;
}

// TODO FixInser FixDelete
Node* insertNode(Node* node, const int key) {
  if (!node) {
    return new Node(key);
  }

  if (key < node->key) {
    node->left = insertNode(node->left, key);
    node->left->parent = node;
  } else {
    node->right = insertNode(node->right, key);
    node->right->parent = node;
  }

  balanceSize(node);
  balanceHeight(node);
  return balanceNode(node);
}

Node* findMinNode(Node* node) {
  return node->left ? findMinNode(node->left) : node;
}

Node* removeNode(Node* node, const int key) {
  if (!node) {
    return nullptr;
  }

  if (key < node->key) {
    node->left = removeNode(node->left, key);
    if (node->left) {}
      node->left->parent = node;
  } else if (key > node->key) {
    node->right = removeNode(node->right, key);
    if (node->right)
      node->right->parent = node;
  } else {
    if (!node->left && !node->right) {
      delete node;
      return nullptr;
    }

    if (!node->left || !node->right) {
      Node* child = node->left ? node->left : node->right;
      child->parent = node->parent;
      delete node;
      return child;
    }

    const Node* minNode = findMinNode(node->right);
    node->key = minNode->key;
    node->right = removeNode(node->right, minNode->key);
    if (node->right) {
      node->right->parent = node;
    }
  }

  balanceHeight(node);
  balanceSize(node);
  return balanceNode(node);
}

Node* findNode(Node* node, const int key) {
  if (!node) {
    return nullptr;
  }
  if (node->key == key) {
    return node;
  }

  if (key < node->key) {
    return findNode(node->left, key);
  }
  return findNode(node->right, key);
}

Node* lowerBoundInternal(Node* current, const int key) {
  Node* result = nullptr;

  while (current) {
    if (current->key >= key) {
      result = current;
      current = current->left;
    } else {
      current = current->right;
    }
  }

  return result;
}

Node::Node(const int key) : key(key), height(1), size(1), left(nullptr), right(nullptr), parent(nullptr),
                            color(Color::RED) {
}

// Node::Node(int key, Color color) : key(key), height(1), size(1), left(nullptr), right(nullptr), parent(nullptr),
//                                    color(color) {
// }

RBTree::RBTree() {
}

RBTree::RBTree(const std::initializer_list<int> list) : root(nullptr) {
  for (const int x : list) {
    insert(x);
  }
}

void RBTree::insert(const int key) {
  if (root == nullptr) {
    root = new Node(key);
    root->color = Color::BLACK;
    return;
  }
  root = insertNode(root, key);
}

int* RBTree::find(const int key) {
  if (!root) {
    return nullptr;
  }
  Node* node = findNode(root, key);
  return node ? &node->key : nullptr;
}

int RBTree::size() const {
  return root ? root->size : 0;
}

int* RBTree::lowerBound(const int key) {
  const auto node = lowerBoundInternal(root, key);
  return node ? &node->key : nullptr;
}

bool RBTree::empty() const {
  return !root;
}

void RBTree::erase(const int key) {
  root = removeNode(root, key);
}

int RBTree::height() const {
  if (!root) {
    return 0;
  }
  return root->height;
}

void deleteSubtree(const Node* node) {
  if (!node)
    return;
  deleteSubtree(node->left);
  deleteSubtree(node->right);
  delete node;
}

RBTree::~RBTree() {
  deleteSubtree(root);
}