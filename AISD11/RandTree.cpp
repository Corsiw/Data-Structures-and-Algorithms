#include <random>
#include <iostream>

class RandTree {
  struct Node {
    int key;
    int size;
    Node* left;
    Node* right;

    explicit Node(const int k) : key(k), size(1), left(nullptr), right(nullptr) {
    }
  };

  Node* root_ = nullptr;

  static int GetSize(const Node* t) {
    return t ? t->size : 0;
  }

  static void UpdateSize(Node* t) {
    if (t) {
      t->size = 1 + GetSize(t->left) + GetSize(t->right);
    }
  }

  static Node* Merge(Node* a, Node* b) {
    if (!a) {
      return b;
    }
    if (!b) {
      return a;
    }

    if (rand() % (GetSize(a) + GetSize(b)) < GetSize(a)) {
      a->right = Merge(a->right, b);
      UpdateSize(a);
      return a;
    }
    b->left = Merge(a, b->left);
    UpdateSize(b);
    return b;
  }

  static void Split(Node* t, const int key, Node*& a, Node*& b) {
    if (!t) {
      a = b = nullptr;
    } else if (key < t->key) {
      Split(t->left, key, a, t->left);
      b = t;
      UpdateSize(b);
    } else {
      Split(t->right, key, t->right, b);
      a = t;
      UpdateSize(a);
    }
  }

  // ---- вставка в корень ----
  static Node* InsertRoot(Node* t, Node* n) {
    if (!t) {
      return n;
    }

    if (n->key < t->key) {
      t->left = InsertRoot(t->left, n);
      // правый поворот
      Node* l = t->left;
      t->left = l->right;
      l->right = t;
      UpdateSize(t);
      UpdateSize(l);
      return l;
    }

    t->right = InsertRoot(t->right, n);
    // левый поворот
    Node* r = t->right;
    t->right = r->left;
    r->left = t;
    UpdateSize(t);
    UpdateSize(r);
    return r;
  }

  static Node* Insert(Node* t, Node* n) {
    if (!t) {
      return n;
    }

    // вероятность вставки в корень: 1 / (size + 1)
    if (std::uniform_int_distribution<int> dist(0, GetSize(t)); dist(ran) == 0) {
      return InsertRoot(t, n);
    }

    if (n->key < t->key) {
      t->left = Insert(t->left, n);
    } else {
      t->right = Insert(t->right, n);
    }

    UpdateSize(t);
    return t;
  }

  static Node* Erase(Node* t, const int key) {
    if (!t) {
      return nullptr;
    }

    if (t->key == key) {
      Node* res = Merge(t->left, t->right);
      delete t;
      return res;
    }

    if (key < t->key) {
      t->left = Erase(t->left, key);
    } else {
      t->right = Erase(t->right, key);
    }

    UpdateSize(t);
    return t;
  }

  static void Print(const Node* root, const int depth = 0) {
    if (!root) {
      return;
    }
    Print(root->right, depth + 1);
    std::cout << std::string(depth * 4, ' ') << root->key << "\n";
    Print(root->left, depth + 1);
  }

public:
  static std::mt19937 ran;

  RandTree() = default;

  void Insert(const int key) {
    root_ = Insert(root_, new Node(key));
  }

  void Erase(const int key) {
    root_ = Erase(root_, key);
  }

  bool Find(const int key) const {
    const Node* t = root_;
    while (t) {
      if (t->key == key) {
        return true;
      }
      t = key < t->key ? t->left : t->right;
    }
    return false;
  }

  void Print() const {
    Print(root_);
  }
};

std::mt19937 RandTree::ran(std::random_device{}());

int main() {
  auto tree = RandTree();

  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(8);
  tree.Insert(1);
  tree.Insert(4);


  tree.Erase(3);

  return 0;
}