#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int l = 0;
  int r = 0;
  std::cin >> n >> l >> r;
  l--;
  r--;
  if (l > r) {
    std::swap(l, r);
  }

  std::vector<int> v(n);
  int next_val = n;
  int ll = static_cast<int>(static_cast<int64_t>((l) + r) / 2);
  int rr = ll + 1;

  while (true) {
    if (ll >= 0) {
      v[ll--] = next_val--;
    } else {
      break;
    }

    if (rr < n) {
      v[rr++] = next_val--;
    } else {
      break;
    }
  }

  while (ll >= 0) {
    v[ll--] = next_val--;
  }

  while (rr < n) {
    v[rr++] = next_val--;
  }

  for (int i = 0; i < n; i++) {
    std::cout << v[i] << " ";
  }
  std::cout << '\n';
}