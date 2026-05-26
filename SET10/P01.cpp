#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

struct Item {
  int c;
  int w;
};

bool Compare(const Item& a, const Item& b) {
  return static_cast<long double>(a.c) / a.w > static_cast<long double>(b.c) / b.w;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int w = 0;
  std::cin >> n >> w;

  std::vector<Item> items(n);
  for (int i = 0; i < n; i++) {
    int ci = 0;
    int w1i = 0;

    std::cin >> ci >> w1i;
    items[i] = {ci, w1i};
  }

  std::ranges::sort(items, Compare);

  long double res = 0;
  int rem = w;

  for (int i = 0; i < n; i++) {
    if (items[i].w <= rem) {
      res += items[i].c;
      rem -= items[i].w;
    } else {
      res += static_cast<long double>(items[i].c) * rem / items[i].w;
      break;
    }
  }

  std::cout << std::fixed << std::setprecision(3);
  std::cout << res << '\n';
}