#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

int LevenshteinDp(const std::string& s1, const std::string& s2) {
  const size_t m = s1.length();
  const size_t n = s2.length();

  std::vector<int> cur(n + 1, 0);
  std::vector<int> prev(n + 1, 0);

  for (int i = 0; i <= n; i++) {
    prev[i] = i;
  }

  for (int i = 1; i <= m; i++) {
    cur[0] = i;

    for (int j = 1; j <= n; j++) {
      if (s1[i - 1] == s2[j - 1]) {
        cur[j] = prev[j - 1];
      } else {
        cur[j] = 1 + std::min(prev[j], std::min(cur[j - 1], prev[j - 1]));
      }
    }
    std::swap(prev, cur);
  }

  return prev[n];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  for (int i = 0; i < n; i++) {
    std::string s1;
    std::string s2;
    std::getline(std::cin, s1);
    std::getline(std::cin, s2);

    std::cout << LevenshteinDp(s1, s2) << ' ';
  }
  std::cout << '\n';
}