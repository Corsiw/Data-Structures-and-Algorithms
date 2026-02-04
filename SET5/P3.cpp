#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_map>

constexpr int kSeed = 426967;
std::mt19937_64 gen(kSeed);

std::unordered_map<int, uint64_t> hashes;

uint64_t Hash(const int key) {
  if (!hashes.contains(key)) {
    hashes[key] = gen();
  }
  return hashes[key];
}

bool IsAnagram(const int k, const std::vector<int>& a, const std::vector<int>& b, int& res_i, int& res_j) {
  if (k == 0) {
    return true;
  }

  // sum of hashes -> start index in a - i
  std::unordered_map<uint64_t, int> s;

  uint64_t cur_sum = 0;
  for (int i = 0; i < k; i++) {
    cur_sum += Hash(a[i]);
  }
  s[cur_sum] = 0;

  // sliding window
  for (int i = 0; i < a.size() - k; i++) {
    cur_sum += Hash(a[k + i]);
    cur_sum -= Hash(a[i]);
    if (!s.contains(cur_sum)) {
      s[cur_sum] = i + 1;
    }
  }

  cur_sum = 0;
  for (int i = 0; i < k; i++) {
    cur_sum += Hash(b[i]);
  }
  if (s.contains(cur_sum)) {
    res_i = s[cur_sum];
    res_j = 0;
    return true;
  }

  // sliding window
  for (int i = 0; i < b.size() - k; i++) {
    cur_sum += Hash(b[k + i]);
    cur_sum -= Hash(b[i]);
    if (s.contains(cur_sum)) {
      res_i = s[cur_sum];
      res_j = i + 1;
      return true;
    }
  }

  return false;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int> a(n);
  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }

  int m = 0;
  std::cin >> m;

  std::vector<int> b(m);
  for (int i = 0; i < m; i++) {
    std::cin >> b[i];
  }

  // binary search
  int k_res = 0;
  int i_res = -1;
  int j_res = -1;

  int l = 0;
  int r = std::min(n, m); // k can't be greater that least of a and b sizes
  while (l <= r) {
    const int mid = (l + r) / 2;
    int i = 0;
    int j = 0;
    if (IsAnagram(mid, a, b, i, j)) {
      k_res = mid;
      i_res = i;
      j_res = j;
      l = mid + 1;
    } else {
      r = mid - 1;
    }
  }

  if (k_res == 0) {
    std::cout << "0 -1 -1\n";
  } else {
    // result must 1-indexed
    std::cout << k_res << " " << i_res + 1<< " " << j_res + 1 << "\n";
  }

  return 0;
}