#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <set>
#include <set>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  uint64_t f1 = 1;
  uint64_t f2 = 2;
  for (int i = 0; i < n; i++) {
    uint64_t f = f1 + f2;
    f %= 1000000007;

    f1 = f2;
    f2 = f;
  }

  std::cout << f1 << '\n';
}