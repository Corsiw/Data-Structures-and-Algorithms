#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

struct Item {
  int v;
  int w1;
  int w2;
};

int64_t SolveKnapsack(const std::vector<Item>& items, const int w1, const int w2) {
  std::vector<std::vector<int64_t> > d(w1 + 1, std::vector<int64_t>(w2 + 1));

  for (int i = 0; i < items.size(); i++) {
    for (int j = w1; j >= 0; j--) {
      for (int k = w2; k >= 0; k--) {
        if (j - items[i].w1 >= 0 && k - items[i].w2 >= 0) {
          d[j][k] = std::max(d[j][k], d[j - items[i].w1][k - items[i].w2] + items[i].v);
        }
      }
    }
  }

  return d[w1][w2];
}

/*
  Тест 1
  4 5 6
  10 2 3
  8 1 2
  15 4 4
  4 1 1
  Цена 23, вес 5, объем 6

  Тест 2
  6 10 12
  8 3 2
  10 4 3
  15 5 6
  4 2 1
  12 6 5
  7 3 4
  Цена 27, вес 10, объем 9

  Тест 3
  3 4 4
  5 2 2
  6 3 2
  4 1 1
  Цена 10, вем 4, объем 3
 */

// Перебор для проверки эффективного алгоритма
int64_t SolveKnapsack(const int i, const int sm, const int w1_cur, const int w2_cur, const std::vector<Item>& items, const int w1, const int w2) {

  if (i >= items.size() || w1_cur + items[i].w1 > w1 || w2_cur + items[i].w2 > w2) {
    return sm;
  }

  return std::max(
    SolveKnapsack(i + 1, sm + items[i].v,  w1_cur + items[i].w1, w2_cur + items[i].w2, items, w1, w2),
    SolveKnapsack(i + 1, sm,  w1_cur, w2_cur, items, w1, w2)
  );
}

int main() {
  int n = 0;
  int w1 = 0;
  int w2 = 0;
  std::cin >> n >> w1 >> w2;

  std::vector<Item> items(n);
  for (int i = 0; i < n; i++) {
    int vi = 0;
    int w1i = 0;
    int w2i = 0;

    std::cin >> vi >> w1i >> w2i;
    items[i] = {vi, w1i, w2i};
  }

  std::cout << SolveKnapsack(items, w1, w2) << '\n';
  std::cout << SolveKnapsack(0, 0, 0, 0, items, w1, w2) << '\n';
}