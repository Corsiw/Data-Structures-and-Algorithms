#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <set>
#include <set>
#include <vector>

struct Line {
  int l;
  int r;
};

bool Compare(const Line& a, const Line& b) {
  if (a.l != b.l) {
    return a.l < b.l;
  }

  return a.r < b.r;
}

bool CompareSet(const Line& a, const Line& b) {
  if (a.r != b.r) {
    return a.r < b.r;
  }

  return a.l < b.l;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<Line> lines(n);
  for (int i = 0; i < n; i++) {
    std::cin >> lines[i].l >> lines[i].r;
  }

  std::ranges::sort(lines, Compare);

  std::set<Line, bool (*)(const Line&, const Line&)> set(CompareSet);

  std::vector<int> res;

  for (int i = 0; i < n; i++) {
    if (set.empty()) {
      set.insert(lines[i]);
      continue;
    }

    const auto node = set.begin();

    if (lines[i].l > node->r) {
      res.push_back(node->r);
      set.clear();
      set.insert(lines[i]);
    } else {
      set.insert(lines[i]);
    }
  }

  if (!set.empty()) {
    const auto node = set.begin();
    res.push_back(node->r);
  }

  std::cout << res.size() << '\n';
  for (int i = 0; i < res.size(); i++) {
    std::cout << res[i] << " ";
  }
  std::cout << '\n';
}