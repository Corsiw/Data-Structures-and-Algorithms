#include<iostream>
#include<vector>
#include <cstdint>
#include<string>


struct Matrix {
  size_t n;
  std::vector<int64_t> data;

  explicit Matrix(const size_t n, const int64_t val = 0) : n(n), data(n * n, val) {
  }

  int64_t& At(const size_t i, const size_t j) {
    return data[i * n + j];
  }

  const int64_t& At(const size_t i, const size_t j) const {
    return data[i * n + j];
  }
};

constexpr int kThreshold = 64;


Matrix Add(const Matrix& a, const Matrix& b) {
  Matrix ans(a.n);
  for (size_t i = 0; i < a.n * a.n; i++) {
    ans.data[i] = a.data[i] + b.data[i];
  }
  return ans;
}

Matrix Sub(const Matrix& a, const Matrix& b) {
  Matrix ans(a.n);
  for (size_t i = 0; i < a.n * a.n; i++) {
    ans.data[i] = a.data[i] - b.data[i];
  }
  return ans;
}

Matrix Naive(const Matrix& a, const Matrix& b) {
  Matrix ans(a.n, 0);
  for (size_t i = 0; i < a.n; i++) {
    for (size_t j = 0; j < a.n; j++) {
      int64_t sum = 0;
      for (size_t k = 0; k < a.n; k++) {
        sum += a.At(i, k) * b.At(k, j);
      }
      ans.At(i, j) = sum;
    }
  }
  return ans;
}

Matrix Mult(const Matrix& a, const Matrix& b) {
  if (a.n <= kThreshold) {
    return Naive(a, b);
  }

  const size_t n = a.n;
  const size_t k = n / 2;

  Matrix a11(k);
  Matrix a12(k);
  Matrix a21(k);
  Matrix a22(k);
  Matrix b11(k);
  Matrix b12(k);
  Matrix b21(k);
  Matrix b22(k);

  for (size_t i = 0; i < k; i++) {
    for (size_t j = 0; j < k; j++) {
      a11.At(i, j) = a.At(i, j);
      a12.At(i, j) = a.At(i, j + k);
      a21.At(i, j) = a.At(i + k, j);
      a22.At(i, j) = a.At(i + k, j + k);

      b11.At(i, j) = b.At(i, j);
      b12.At(i, j) = b.At(i, j + k);
      b21.At(i, j) = b.At(i + k, j);
      b22.At(i, j) = b.At(i + k, j + k);
    }
  }

  // 7 произведений по Штрассену
  Matrix m1 = Mult(Add(a11, a22), Add(b11, b22));
  Matrix m2 = Mult(Add(a21, a22), b11);
  Matrix m3 = Mult(a11, Sub(b12, b22));
  Matrix m4 = Mult(a22, Sub(b21, b11));
  Matrix m5 = Mult(Add(a11, a12), b22);
  Matrix m6 = Mult(Sub(a21, a11), Add(b11, b12));
  Matrix m7 = Mult(Sub(a12, a22), Add(b21, b22));

  // Складываем квадранты результата
  Matrix c(n);
  Matrix c11 = Add(Sub(Add(m1, m4), m5), m7);
  Matrix c12 = Add(m3, m5);
  Matrix c21 = Add(m2, m4);
  Matrix c22 = Add(Sub(Add(m1, m3), m2), m6);

  for (size_t i = 0; i < k; i++) {
    for (size_t j = 0; j < k; j++) {
      c.At(i, j) = c11.At(i, j);
      c.At(i, j + k) = c12.At(i, j);
      c.At(i + k, j) = c21.At(i, j);
      c.At(i + k, j + k) = c22.At(i, j);
    }
  }
  return c;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t n = 0;
  std::cin >> n;

  auto a = Matrix(n);
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      std::cin >> a.At(i, j);
    }
  }

  auto b = Matrix(n);
  for (size_t i = 0; i < n; i++) {
    std::vector<int64_t> row(n);
    for (size_t j = 0; j < n; j++) {
      std::cin >> b.At(i, j);
    }
  }

  const auto result = Mult(a, b);
  for (size_t i = 0; i < result.n; i++) {
    for (size_t j = 0; j < result.n; j++) {
      std::cout << result.At(i, j) << " ";
    }
    std::cout << "\n";
  }
}