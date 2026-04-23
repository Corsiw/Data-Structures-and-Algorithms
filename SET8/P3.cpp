#include <iostream>
#include <string>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string pattern;
  std::cin >> pattern;

  std::string input;
  std::cin >> input;

  if (pattern.empty()) {
    std::cout << input.size() + 1 << "\n";
    for (int i = 0; i <= input.size(); i++) {
      std::cout << i << "\n";
    }
    return 0;
  }

  const size_t m = pattern.length();
  std::vector<size_t> pf(m);

  size_t j = 0;
  for (size_t i = 1; i < m; i++) {

    while (j > 0 && pattern[j] != pattern[i]) {
      j = pf[j - 1];
    }

    if (pattern[j] == pattern[i]) {
      j++;
    }

    pf[i] = j;
  }

  const size_t n = input.length();
  std::vector<size_t> res;

  j = 0;
  for (size_t i = 0; i < n; i++) {
    while (j > 0 && pattern[j] != input[i]) {
      j = pf[j - 1];
    }

    if (pattern[j] == input[i]) {
      j++;
    }

    if (j == m) {
      res.push_back(i - j + 1);
      j = pf[j - 1];
    }
  }

  std::cout << res.size() << '\n';
  for (size_t i = 0; i < res.size(); i++) {
    std::cout << res[i] << '\n';
  }
}