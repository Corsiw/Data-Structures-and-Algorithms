#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

std::vector<int> Manacher(const std::string& s1) {
  std::string s;
  for (const char c : s1) {
    s += "#" + std::string(1, c);
  }
  s += "#";

  int r = 0;
  int l = 0;

  const int n = static_cast<int>(s.size());
  std::vector<int> p(n);

  for (int i = 0; i < n; i++) {
    if (i <= r) {
      p[i] = std::min(p[l + r - i], r - i);
    }

    while (s[i + 1 + p[i]] == s[i - 1 - p[i]]) {
      p[i]++;
    }

    if (i + p[i] > r) {
      r = i + p[i];
      l = i - p[i];
    }
  }

  return p;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string s;
  std::cin >> s;

  const std::vector<int> p = Manacher(s);

  uint64_t c = 0;
  for (int i = 0; i < p.size(); i++) {
    c += (p[i] + 1) / 2;
  }

  std::cout << c << '\n';
}