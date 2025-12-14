#include "splay-tree.h"
#include <algorithm>

Node::Node(int k, Node* p)
  : key(k), left(nullptr), right(nullptr), parent(p) {
}

SplayTree::SplayTree() : root(nullptr) {
}

void SplayTree::insert(int key) {
  if (!root) {
    root = new Node(key, nullptr);
    return;
  }

  Node* cur = root;
  Node* parent = nullptr;

  while (cur != nullptr) {
    parent = cur;
    if (key < cur->key) {
      cur = cur->left;
    } else if (key > cur->key) {
      cur = cur->right;
    } else {
      return;
    }
  }

  const auto node = new Node(key, parent);
  if (key < parent->key) {
    parent->left = node;
  } else {
    parent->right = node;
  }
}

Node* SplayTree::find(int key) const {
  Node* cur = root;
  while (cur) {
    if (key == cur->key) {
      return cur;
    }
    if (key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  return cur;
}

void SplayTree::rotateLeft(Node* x) {
  Node* y = x->right;
  if (!y) {
    return;
  }

  x->right = y->left;
  if (y->left) {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (!x->parent) {
    root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void SplayTree::rotateRight(Node* x) {
  Node* y = x->left;
  if (!y) {
    return;
  }

  x->left = y->right;
  if (y->right) {
    y->right->parent = x;
  }

  y->parent = x->parent;

  if (!x->parent) {
    root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}

int SplayTree::splay(Node* x) {
  if (!x) {
    return 0;
  }

  int rotations = 0;

  while (x->parent != nullptr) {
    Node* p = x->parent;
    Node* g = p->parent;

    if (g == nullptr) {
      if (x == p->left) {
        rotateRight(p);
      } else {
        rotateLeft(p);
      }
      rotations += 1;

    } else if (x == p->left && p == g->left) {
      rotateRight(g);
      rotateRight(x->parent);
      rotations += 2;
    } else if (x == p->right && p == g->right) {
      rotateLeft(g);
      rotateLeft(x->parent);
      rotations += 2;
    } else if (x == p->right && p == g->left) {
      rotateLeft(p);
      rotateRight(g);
      rotations += 1;
    } else if (x == p->left && p == g->right) {
      rotateRight(p);
      rotateLeft(g);
      rotations += 1;
    }
  }

  return rotations;
}

int SplayTree::getHeight() const {
  return getHeight(root);
}

int SplayTree::getHeight(Node* n) const {
  if (!n) {
    return -1;
  }
  if (!n->left && !n->right) {
    return 0;
  }

  return 1 + std::max(getHeight(n->left), getHeight(n->right));
}

SplayTree::~SplayTree() {
  clear(root);
}

void SplayTree::clear(Node* n) {
  if (!n) {
    return;
  }
  clear(n->left);
  clear(n->right);
  delete n;
}