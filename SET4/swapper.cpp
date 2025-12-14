#include <random>
#include <cstdint>
#include <functional>
#include <iostream>

constexpr int kSeed = 694121241;
std::mt19937 rng(kSeed);

__attribute__((optimize("Ofast")))
struct Node {
  int64_t sum;
  int64_t prior;
  Node* left;
  Node* right;
  int value;
  int size;

  explicit Node(const int value) : sum(value), prior(rng()), left(nullptr), right(nullptr),
                                   value(value), size(1) {
  }
};

__attribute__((optimize("Ofast")))
Node* Merge(Node* l, Node* r) {
  if (!l) {
    return r;
  }
  if (!r) {
    return l;
  }

  if (l->prior > r->prior) {
    l->right = Merge(l->right, r);
    l->size = 1 + (l->left ? l->left->size : 0) + (l->right ? l->right->size : 0);
    l->sum = l->value + (l->left ? l->left->sum : 0) + (l->right ? l->right->sum : 0);
    return l;
  }

  r->left = Merge(l, r->left);
  r->size = 1 + (r->left ? r->left->size : 0) + (r->right ? r->right->size : 0);
  r->sum = r->value + (r->left ? r->left->sum : 0) + (r->right ? r->right->sum : 0);
  return r;
}

__attribute__((optimize("Ofast")))
inline int GetSize(Node* node) {
  return node ? node->size : 0;
}

__attribute__((optimize("Ofast")))
inline int64_t GetSum(Node* node) {
  return node ? node->sum : 0;
}

__attribute__((optimize("Ofast")))
inline void Update(Node* node) {
  if (!node) {
    return;
  }
  node->size = 1 + GetSize(node->left) + GetSize(node->right);
  node->sum = node->value + GetSum(node->left) + GetSum(node->right);
}

__attribute__((optimize("Ofast")))
void Split(Node* root, int key, Node*& l, Node*& r) {
  l = r = nullptr;
  Node* curr = root;
  Node* l_tail = nullptr;
  Node* r_tail = nullptr;

  std::vector<Node*> path;

  while (curr) {
    const int left_size = GetSize(curr->left);

    if (left_size + 1 <= key) {
      // Идем вправо, текущий узел идёт в левое дерево
      if (!l) {
        l = curr;
      }
      if (l_tail) {
        l_tail->right = curr;
      }
      l_tail = curr;

      path.push_back(curr);
      key -= left_size + 1;
      curr = curr->right;

      l_tail->right = nullptr;
    } else {
      // Идем влево, текущий узел идёт в правое дерево
      if (!r) {
        r = curr;
      }
      if (r_tail) {
        r_tail->left = curr;
      }
      r_tail = curr;

      path.push_back(curr);
      curr = curr->left;

      r_tail->left = nullptr;
    }
  }

  for (auto it = path.rbegin(); it != path.rend(); ++it) {
    Update(*it);
  }
}

__attribute__((optimize("Ofast")))
Node* BuildTreap(const std::vector<int>& vec) {
  std::vector<Node*> stack;
  Node* root = nullptr;

  for (const int v : vec) {
    auto curr = new Node(v);
    Node* last = nullptr;

    while (!stack.empty() && stack.back()->prior < curr->prior) {
      last = stack.back();
      stack.pop_back();
    }

    curr->left = last;

    if (!stack.empty()) {
      stack.back()->right = curr;
    } else {
      root = curr;
    }

    stack.push_back(curr);
  }

  std::function<void(Node*)> dfs = [&](Node* node) {
    if (!node) {
      return;
    }
    dfs(node->left);
    dfs(node->right);
    Update(node);
  };
  dfs(root);

  return root;
}

__attribute__((optimize("Ofast")))
void SwapPairs(Node* & even_root, Node* & odd_root, const int l, const int r) {
  const int even_l = (l + 1) / 2;
  const int even_r = r / 2;
  const int odd_l = l / 2;
  const int odd_r = (r - 1) / 2;

  Node* t1 = nullptr;
  Node* t2 = nullptr;
  Node* t3 = nullptr;

  Split(even_root, even_r + 1, t2, t3);
  Split(t2, even_l, t1, t2);

  Node* t11 = nullptr;
  Node* t22 = nullptr;
  Node* t33 = nullptr;
  Split(odd_root, odd_r + 1, t22, t33);
  Split(t22, odd_l, t11, t22);

  even_root = Merge(Merge(t1, t22), t3);
  odd_root = Merge(Merge(t11, t2), t33);
}

__attribute__((optimize("Ofast")))
int64_t Query(Node* & root, const int l, const int r) {
  Node* t1 = nullptr;
  Node* t2 = nullptr;
  Node* t3 = nullptr;
  Split(root, r + 1, t2, t3);
  Split(t2, l, t1, t2);
  const int64_t ans = t2->sum;
  root = Merge(Merge(t1, t2), t3);
  return ans;
}

__attribute__((optimize("Ofast")))
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int k = 1;
  int n = 0;
  int m = 0;
  while (std::cin >> n >> m) {

    if (!n && !m) {
      return 0;
    }

    if (k > 1) {
      std::cout << '\n';
    }

    std::vector<int> even_vec;
    std::vector<int> odd_vec;
    even_vec.reserve(n / 2 + 1);
    odd_vec.reserve(n / 2 + 1);
    for (int i = 0; i < n; i++) {
      int x = 0;
      std::cin >> x;
      if (i % 2 == 0) {
        even_vec.push_back(x);
      } else {
        odd_vec.push_back(x);
      }
    }

    Node* even_root = BuildTreap(even_vec);
    Node* odd_root = BuildTreap(odd_vec);

    std::cout << "Swapper " << k++ << ":\n";
    for (int i = 0; i < m; i++) {
      int tmp = 0;
      int a = 0;
      int b = 0;
      std::cin >> tmp >> a >> b;
      a--;
      b--;

      if (tmp == 1) {
        SwapPairs(even_root, odd_root, a, b);
      } else {
        int64_t sum = 0;

        int l = (a + 1) / 2;
        int r = b / 2;
        if (l <= r) {
          sum += Query(even_root, l, r);
        }

        l = a / 2;
        r = (b - 1) / 2;
        if (l <= r && b != 0) {
          sum += Query(odd_root, l, r);
        }
        std::cout << sum << '\n';
      }
    }
  }
}