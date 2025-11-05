#include <algorithm>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

std::vector<int> countSort(const std::vector<int>& vec) {
  if (vec.empty()) {
    return {};
  }

  std::unordered_map<int, int> freq;
  for (int v : vec) {
    freq[v]++;
  }

  std::vector<int> keys;
  keys.reserve(freq.size());
  for (const auto& key : freq | std::views::keys) {
    keys.push_back(key);
  }
  std::ranges::sort(keys);

  std::vector<int> result;
  result.reserve(vec.size());
  for (int key : keys) {
    int count = freq[key];
    while (count-- > 0) {
      result.push_back(key);
    }
  }

  return result;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int> vec(n);
  for (int i = 0; i < n; i++) {
    std::cin >> vec[i];
  }

  const auto res = countSort(vec);
  for (const int elem : res) {
    std::cout << elem << ' ';
  }
}