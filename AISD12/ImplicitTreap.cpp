#include <random>
#include <cstdint>
#include <iostream>
#include <vector>

constexpr int kSeed = 6942067;

class ImplicitTreap {
  struct Node {
    int value;
    int64_t sum;
    int size;

    int minv, maxv;

    int lazy_add;
    bool lazy_rev;

    int prior;
    Node* left;
    Node* right;

    explicit Node(const int value)
      : value(value), sum(value), size(1),
        minv(value), maxv(value),
        lazy_add(0), lazy_rev(false),
        prior(static_cast<int>(rng())),
        left(nullptr), right(nullptr) {
    }
  };

  static int GetSize(const Node* t) {
    return t ? t->size : 0;
  }

  static int64_t GetSum(const Node* t) {
    return t ? t->sum : 0;
  }

  static int GetMin(const Node* t) {
    return t ? t->minv : INT32_MAX;
  }

  static int GetMax(const Node* t) {
    return t ? t->maxv : INT32_MIN;
  }

  static void ApplyAdd(Node* t, const int val) {
    if (!t) {
      return;
    }
    t->value += val;
    t->sum += 1LL * val * t->size;
    t->minv += val;
    t->maxv += val;
    t->lazy_add += val;
  }

  static void ApplyReverse(Node* t) {
    if (!t) {
      return;
    }
    t->lazy_rev ^= 1;
    std::swap(t->left, t->right);
  }

  static void Push(Node* t) {
    if (!t) {
      return;
    }

    if (t->lazy_add != 0) {
      if (t->left) {
        ApplyAdd(t->left, t->lazy_add);
      }
      if (t->right) {
        ApplyAdd(t->right, t->lazy_add);
      }
      t->lazy_add = 0;
    }

    if (t->lazy_rev) {
      if (t->left) {
        ApplyReverse(t->left);
      }
      if (t->right) {
        ApplyReverse(t->right);
      }
      t->lazy_rev = false;
    }
  }

  static void Pull(Node* t) {
    if (!t) {
      return;
    }
    t->size = 1 + GetSize(t->left) + GetSize(t->right);
    t->sum = t->value + GetSum(t->left) + GetSum(t->right);
    t->minv = std::min(t->value, std::min(GetMin(t->left), GetMin(t->right)));
    t->maxv = std::max(t->value, std::max(GetMax(t->left), GetMax(t->right)));
  }

  static void Split(Node* t, const int k, Node*& l, Node*& r) {
    if (!t) {
      l = r = nullptr;
      return;
    }
    Push(t);
    if (GetSize(t->left) >= k) {
      Split(t->left, k, l, t->left);
      r = t;
      Pull(r);
    } else {
      Split(t->right, k - GetSize(t->left) - 1, t->right, r);
      l = t;
      Pull(l);
    }
  }

  static Node* Merge(Node* l, Node* r) {
    if (!l) {
      return r;
    }
    if (!r) {
      return l;
    }
    if (l->prior > r->prior) {
      Push(l);
      l->right = Merge(l->right, r);
      Pull(l);
      return l;
    }
    Push(r);
    r->left = Merge(l, r->left);
    Pull(r);
    return r;
  }

  Node* root_ = nullptr;

public:
  static std::mt19937 rng;

  // Insert value at position pos
  void Insert(const int pos, const int value) {
    Node* l = nullptr;
    Node* r = nullptr;
    Split(root_, pos, l, r);
    root_ = Merge(Merge(l, new Node(value)), r);
  }

  // Remove element at position pos
  void Erase(const int pos) {
    Node* l = nullptr;
    Node* m = nullptr;
    Node* r = nullptr;
    Split(root_, pos, l, r);
    Split(r, 1, m, r);
    delete m;
    root_ = Merge(l, r);
  }

  // Increase on range [l, r)
  void RangeAdd(const int l, const int r, const int val) {
    Node* a = nullptr;
    Node* b = nullptr;
    Node* c = nullptr;
    Split(root_, r, b, c);
    Split(b, l, a, b);
    ApplyAdd(b, val);
    root_ = Merge(Merge(a, b), c);
  }

  // Query min on range
  int RangeMin(const int l, const int r) {
    Node* a = nullptr;
    Node* b = nullptr;
    Node* c = nullptr;
    Split(root_, r, b, c);
    Split(b, l, a, b);
    const int res = GetMin(b);
    root_ = Merge(Merge(a, b), c);
    return res;
  }

  // Query max on range
  int RangeMax(const int l, const int r) {
    Node* a = nullptr;
    Node* b = nullptr;
    Node* c = nullptr;
    Split(root_, r, b, c);
    Split(b, l, a, b);
    const int res = GetMax(b);
    root_ = Merge(Merge(a, b), c);
    return res;
  }

  // Build from vector (optional)
  void Build(const std::vector<int>& v) {
    root_ = nullptr;
    for (int i = 0; i < static_cast<int>(v.size()); i++) {
      Insert(i, v[i]);
    }
  }
};

std::mt19937 ImplicitTreap::rng(kSeed);


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  ImplicitTreap tr;
  tr.Build({5, 1, 3, 9, 7});

  std::cout << tr.RangeMin(1, 4) << "\n"; // 1
  std::cout << tr.RangeMax(1, 4) << "\n"; // 9

  tr.RangeAdd(0, 3, 10); // +10 к [0..2]

  std::cout << tr.RangeMin(0, 5) << "\n"; // 7
  std::cout << tr.RangeMax(0, 5) << "\n"; // 15

  tr.Insert(2, 100);

  std::cout << tr.RangeMax(0, 5) << "\n"; // 100

  tr.Erase(5); // delete 7

  std::cout << tr.RangeMin(0, 5) << "\n"; // no 7, min only 9

  return 0;
}