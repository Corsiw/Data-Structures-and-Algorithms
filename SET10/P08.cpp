#include <cstdint>
#include <iostream>
#include <vector>

struct Matrix {
  explicit Matrix(const int n, const bool is_e = false) : n(n) {
    a.assign(n, std::vector<int64_t>(n, 0));
    if (is_e) {
      for (int i = 0; i < n; i++) {
        a[i][i] = 1;
      }
    }
  }

  Matrix operator*(const Matrix& other) const {
    Matrix res(n);

    for (int i = 0; i < n; i++) {
      for (int k = 0; k < n; k++) {
        if (!a[i][k]) {
          continue;
        }
        const int64_t cur = a[i][k];
        for (int j = 0; j < n; j++) {
          if (!other.a[k][j]) {
            continue;
          }
          res.a[i][j] = (res.a[i][j] + cur * other.a[k][j]) % 1000000179;
        }
      }
    }
    return res;
  }

  int n;
  std::vector<std::vector<int64_t> > a;
};

std::vector<int64_t> Multiply(const std::vector<int64_t>& v, const Matrix& m) {
  std::vector<int64_t> res(m.n);

  for (int i = 0; i < m.n; i++) {
    if (!v[i]) {
      continue;
    }
    for (int j = 0; j < m.n; j++) {
      if (!m.a[i][j]) {
        continue;
      }
      res[j] = (res[j] + v[i] * m.a[i][j]) % 1000000179;
    }
  }
  return res;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int64_t n = 0;
  int k = 0;
  std::cin >> n >> k;

  if (n == 1) {
    std::cout << k << '\n';
    return 0;
  }

  Matrix matrix(2 * k);

  for (int v = 1; v <= k; v++) {
    for (int x = 1; x < v; x++) {
      matrix.a[v - 1][k + (x - 1)] = 1;
    }

    for (int x = v + 1; x <= k; x++) {
      matrix.a[k + (v - 1)][x - 1] = 1;
    }
  }

  std::vector<int64_t> state(2 * k, 0);

  for (int x = 1; x <= k; x++) {
    state[x - 1] = x - 1;
    state[k + (x - 1)] = k - x;
  }

  int64_t p = n - 2;

  Matrix cur = matrix;
  while (p > 0) {
    if (p & 1) {
      state = Multiply(state, cur);
    }
    cur = cur * cur;
    p >>= 1;
  }

  int64_t res = 0;
  for (const int64_t x : state) {
    res = (res + x) % 1000000179;
  }

  std::cout << res << '\n';
}