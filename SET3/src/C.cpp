#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <vector>

constexpr int kBase = 256;

void RadixSortNonNegative(std::vector<int>& arr) {
  const int max = *std::ranges::max_element(arr);
  for (uint64_t exp = 1; max / exp > 0; exp *= kBase) {
    std::vector<int> result(arr.size());
    int count[kBase] = {};

    for (const int elem : arr) {
      count[(elem / exp) % kBase]++;
    }

    for (int i = 1; i < kBase; ++i) {
      count[i] += count[i - 1];
    }

    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
      const int d = static_cast<int>(arr[i] / exp) % kBase;
      result[count[d] - 1] = arr[i];
      count[d]--;
    }
    arr = std::move(result);
  }
}

void RadixSort(std::vector<int>& arr) {
  std::vector<int> neg;
  std::vector<int> pos;

  for (int i = 0; i < arr.size(); i++) {
    if (arr[i] < 0) {
      neg.push_back(-arr[i]);
    } else {
      pos.push_back(arr[i]);
    }
  }

  if (!neg.empty()) {
    RadixSortNonNegative(neg);
  }
  if (!pos.empty()) {
    RadixSortNonNegative(pos);
  }

  arr.clear();
  for (int i = static_cast<int>(neg.size()) - 1; i >= 0; --i) {
    arr.push_back(-neg[i]);
  }
  for (int x : pos) {
    arr.push_back(x);
  }

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

  RadixSort(vec);
  for (int i = 0; i < n; i++) {
    std::cout << vec[i] << " ";
  }
}