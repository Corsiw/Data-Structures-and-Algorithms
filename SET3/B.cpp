#include <iostream>
#include <vector>

constexpr int kMaxVal = 1000000;

void CountSort(std::vector<int>& vec) {
  if (vec.empty()) {
    return;
  }

  std::vector<int> keys(kMaxVal * 2 + 1);

  for (int i = 0; i < vec.size(); i++) {
    keys[vec[i] + kMaxVal]++;
  }

  int j = 0;
  for (int i = 0; i < keys.size(); i++) {
    while (keys[i]-- > 0) {
      vec[j++] = i - kMaxVal;
    }
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

  CountSort(vec);

  for (const int elem : vec) {
    std::cout << elem << ' ';
  }
}