#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

int64_t SolveIntervals(const std::vector<int>& slices, const int n) {
  std::vector d(n + 2, std::vector<int64_t>(n + 2));

  for (size_t length = 2; length < n + 2; length++) {
    for (size_t i = 0; i + length < n + 2; i++) {
      const size_t j = i + length;

      d[i][j] = INT64_MAX;
      for (size_t k = i + 1; k < j; k++) {
        d[i][j] = std::min(d[i][j], d[i][k] + d[k][j] + slices[j] - slices[i]);
      }

    }
  }

  return d[0][n + 1];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int l = 0;
  int n = 0;
  std::cin >> l >> n;

  std::vector<int> slices(n + 2);
  for (int i = 1; i <= n; i++) {
    std::cin >> slices[i];
  }
  slices[0] = 0;
  slices[n + 1] = l;

  std::cout << SolveIntervals(slices, n) << '\n';
}