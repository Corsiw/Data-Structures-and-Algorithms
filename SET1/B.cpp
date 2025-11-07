#include <cstdint>
#include <iostream>
#include <set>
#include <vector>

int main() {
  int n = 0;
  std::cin >> n;

  std::vector<int64_t> v;
  v.resize(n);
  for (int i = 0; i < n; i++) {
    std::cin >> v[i];
  }

  int cur_h_index = 0;
  std::set<std::vector<int64_t>> unique_rotations;

  for (int i = 0; i < n; i++) {
    int h_index = 0;
    std::vector<int64_t> rotation(n);
    for (int j = 0; j < n; j++) {
      rotation[j] = v[(i+j) % n];
      if (rotation[j] != v[j]) {
        h_index++;
      }
    }

    if (h_index > cur_h_index) {
      cur_h_index = h_index;
      unique_rotations.clear();
      unique_rotations.insert(rotation);
    } else if (cur_h_index == h_index) {
      unique_rotations.insert(rotation);
    }
  }
  std::cout << unique_rotations.size();
}