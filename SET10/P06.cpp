#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

struct Item {
  int v;
  int w;
};

int64_t SolveKnapsack(const std::vector<Item>& items, const int w1) {
  std::vector<int64_t> d(w1 + 1);

  for (int i = 0; i < items.size(); i++) {
    for (int j = w1; j >= 0; j--) {
      if (j - items[i].w >= 0) {
        d[j] = std::max(d[j], d[j - items[i].w] + items[i].v);
      }

    }
  }

  return d[w1];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int w = 0;
  std::cin >> n >> w;

  std::vector<Item> items(n);
  std::vector<int> ms(n);
  for (int i = 0; i < n; i++) {
    std::cin >> ms[i];
  }

  for (int i = 0; i < n; i++) {
    int ci = 0;

    std::cin >> ci;
    items[i] = {ci, ms[i]};
  }

  std::cout << SolveKnapsack(items, w) << '\n';
}