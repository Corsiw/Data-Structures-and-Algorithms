#include <random>
#include <cstdint>
#include <iostream>

struct Node {
  int64_t sum = 0;
  bool lazy_swap = false; // ленивый флаг
};

void Build(const int v, const int tl, const int tr, std::vector<Node>& tree, std::vector<int>& a) {
  if (tl == tr) {
    tree[v].sum = a[tl];
    return;
  }
  const int tm = (tl + tr) / 2;
  Build(v * 2, tl, tm, tree, a);
  Build(v * 2 + 1, tm + 1, tr, tree, a);
  tree[v].sum = tree[v * 2].sum + tree[v * 2 + 1].sum;
}

void ApplySwap(const int v, const int tl, const int tr, std::vector<Node>& tree) {
  tree[v].lazy_swap ^= 1;

  if (tl == tr) {
    return;
  }

  std::swap(tree[v * 2], tree[v * 2 + 1]);
}

void Push(const int v, const int tl, const int tr, std::vector<Node>& tree) {
  if (!tree[v].lazy_swap) {
    return;
  }
  const int tm = (tl + tr) / 2;

  ApplySwap(v * 2, tl, tm, tree);
  ApplySwap(v * 2 + 1, tm + 1, tr, tree);

  tree[v].lazy_swap = false;
}

void UpdateSwap(const int v, const int tl, const int tr, const int l, const int r, std::vector<Node>& tree) {
  if (l > r) {
    return;
  }

  if (l == tl && r == tr) {
    ApplySwap(v, tl, tr, tree);
    return;
  }

  Push(v, tl, tr, tree);

  const int tm = (tl + tr) / 2;

  UpdateSwap(v * 2, tl, tm, l, std::min(r, tm), tree);
  UpdateSwap(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, tree);

  tree[v].sum = tree[v * 2].sum + tree[v * 2 + 1].sum;
}

int64_t QuerySum(const int v, const int tl, const int tr, const int l, const int r, std::vector<Node>& tree) {
  if (l > r) {
    return 0;
  }

  if (l == tl && r == tr) {
    return tree[v].sum;
  }

  Push(v, tl, tr, tree);

  const int tm = (tl + tr) / 2;

  return QuerySum(v * 2, tl, tm, l, std::min(r, tm), tree) +
         QuerySum(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, tree);
}

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

    std::vector<int> vec(n);
    for (int i = 0; i < n; i++) {
      std::cin >> vec[i];
    }
    std::vector<Node> tree;
    tree.resize(4*n);
    Build(1, 0, n - 1, tree, vec);

    std::cout << "Swapper " << k++ << ":\n";
    for (int i = 0; i < m; i++) {
      int tmp = 0;
      int a = 0;
      int b = 0;
      std::cin >> tmp >> a >> b;
      a--;
      b--;

      if (tmp == 1) {
        UpdateSwap(1, 0, n-1, a, b, tree);
      } else {
        std::cout << QuerySum(1, 0, n-1, a, b, tree) << "\n";
      }
    }
  }
}