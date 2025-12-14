#include "rbtree.h"

#include <algorithm>

void BalanceHeight(Node* node) {
  while (node) {
    int left_h = node->left ? node->left->height : 1;
    int right_h = node->right ? node->right->height : 1;

    if (node->color == Color::BLACK) {
      node->height = 1 + std::max(left_h, right_h);
    } else {
      node->height = std::max(left_h, right_h);
    }

    node = node->parent;
  }
}

void BalanceSize(Node* node) {
  if (!node) {
    return;
  }

  const int left_size = node->left ? node->left->size : 0;
  const int right_size = node->right ? node->right->size : 0;

  node->size = 1 + left_size + right_size;
  BalanceSize(node->parent);
}

Node* RotateRight(Node* node) {
  if (!node || !node->left) {
    return node;
  }

  Node* new_root = node->left;
  Node* new_root_right = new_root->right;
  Node* parent = node->parent;

  new_root->right = node;
  node->left = new_root_right;

  if (new_root_right) {
    new_root_right->parent = node;
  }

  new_root->parent = parent;
  node->parent = new_root;

  if (parent) {
    if (parent->left == node) {
      parent->left = new_root;
    } else {
      parent->right = new_root;
    }
  }

  BalanceSize(node);
  BalanceHeight(node);
  BalanceSize(new_root);
  BalanceHeight(new_root);

  return new_root;
}

Node* RotateLeft(Node* node) {
  if (!node || !node->right) {
    return node;
  }

  Node* new_root = node->right;
  Node* new_root_left = new_root->left;
  Node* parent = node->parent;

  new_root->left = node;
  node->right = new_root_left;

  if (new_root_left) {
    new_root_left->parent = node;
  }

  new_root->parent = parent;
  node->parent = new_root;

  if (parent) {
    if (parent->left == node) {
      parent->left = new_root;
    } else {
      parent->right = new_root;
    }
  }

  BalanceSize(node);
  BalanceHeight(node);
  BalanceSize(new_root);
  BalanceHeight(new_root);

  return new_root;
}

Node* Grandparent(const Node* n) {
  return n && n->parent ? n->parent->parent : nullptr;
}

Node* Uncle(const Node* n) {
  const Node* node = Grandparent(n);
  if (!node) {
    return nullptr;
  }
  return n->parent == node->left ? node->right : node->left;
}

void InsertFix(RBTree* tree, Node* node) {
  while (node->parent && node->parent->color == Color::RED) {
    Node* gp = Grandparent(node);
    if (!gp) {
      break;
    }

    Node* unc = Uncle(node);
    if (unc && unc->color == Color::RED) {
      node->parent->color = Color::BLACK;
      unc->color = Color::BLACK;
      gp->color = Color::RED;
      node = gp;
    } else {
      if (node->parent == gp->left) {
        if (node == node->parent->right) {
          node = node->parent;
          RotateLeft(node);
        }
        node->parent->color = Color::BLACK;
        gp->color = Color::RED;
        RotateRight(gp);
      } else {
        if (node == node->parent->left) {
          node = node->parent;
          RotateRight(node);
        }
        node->parent->color = Color::BLACK;
        gp->color = Color::RED;
        RotateLeft(gp);
      }
    }
  }

  Node* r = tree->root;
  while (r && r->parent) {
    r = r->parent;
  }
  if (r) {
    tree->root = r;
    tree->root->color = Color::BLACK;
  }
}

Node* FindMinNode(Node* node) {
  return node->left ? FindMinNode(node->left) : node;
}

Node* FindNode(Node* node, const int key) {
  if (!node) {
    return nullptr;
  }
  if (node->key == key) {
    return node;
  }

  if (key < node->key) {
    return FindNode(node->left, key);
  }
  return FindNode(node->right, key);
}

Node* LowerBoundInternal(Node* current, const int key) {
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

RBTree::RBTree() = default;

RBTree::RBTree(const std::initializer_list<int> list) : root(nullptr) {
  for (const int x : list) {
    insert(x);
  }
}

void RBTree::insert(const int key) {
  if (find(key)) {
    return;
  }
  Node* new_node = new Node(key);
  Node* cur = root;
  Node* parent = nullptr;

  while (cur != nullptr) {
    parent = cur;
    if (new_node->key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  if (parent == nullptr) {
    root = new_node;
    root->color = Color::BLACK;
    return;
  }

  new_node->parent = parent;
  if (new_node->key < parent->key) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }

  InsertFix(this, new_node);
  BalanceHeight(new_node);
  BalanceSize(new_node);
}

int* RBTree::find(const int key) {
  if (!root) {
    return nullptr;
  }
  Node* node = FindNode(root, key);
  return node ? &node->key : nullptr;
}

int RBTree::size() const {
  return root ? root->size : 0;
}

int* RBTree::lowerBound(const int key) {
  const auto node = LowerBoundInternal(root, key);
  return node ? &node->key : nullptr;
}

bool RBTree::empty() const {
  return !root;
}

void RBTree::erase(const int key) {
  // root = removeNode(root, key);
}

int RBTree::height() const {
  if (!root) {
    return 0;
  }
  return root->height;
}

void DeleteSubtree(Node* node) {
  if (!node) {
    return;
  }
  DeleteSubtree(node->left);
  DeleteSubtree(node->right);
  delete node;
}

RBTree::~RBTree() {
  DeleteSubtree(root);
}