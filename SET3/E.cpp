#include <iostream>
#include <random>
#include <vector>
#include <string>

constexpr int64_t kMod = 2;
constexpr int kK = 20;
constexpr uint64_t kSeed = 111111111;


using Row = std::vector<uint64_t>;
using Matrix = std::vector<Row>;


Matrix ReadMatrix(const int n) {
  const int blocks = (n + 63) / 64;
  Matrix m(n, Row(blocks));

  for (int i = 0; i < n; ++i) {
    std::string hexs;
    std::cin >> hexs;

    std::vector<int> bits;
    bits.reserve(n);
    for (const char c : hexs) {
      const int v = (c >= '0' && c <= '9') ? c - '0' : 10 + (toupper(c) - 'A');
      for (int b = 3; b >= 0; --b) {
        bits.push_back((v >> b) & 1);
      }
    }
    bits.resize(n);

    for (int j = 0; j < n; ++j) {
      if (bits[j]) {
        m[i][j / 64] |= 1ULL << (j % 64);
      }
    }
  }
  return m;
}

std::vector<int> MatVec(const Matrix& m, const std::vector<uint64_t>& v, const int n) {
  std::vector<int> res(n);
  const int blocks = (n + 63) / 64;

  for (int i = 0; i < n; ++i) {
    uint64_t sum = 0;
    for (int b = 0; b < blocks; ++b) {
      const int parity = __builtin_popcountll(m[i][b] & v[b]) & 1;
      sum ^= parity;
    }
    res[i] = sum & 1ULL;
  }
  return res;
}


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  if (!n) {
    std::cout << "NO";
    return 0;
  }

  auto A = ReadMatrix(n);
  auto B = ReadMatrix(n);
  auto C = ReadMatrix(n);

  int blocks = (n + 63) / 64;
  std::mt19937_64 rng(kSeed);
  std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

  for (int it = 0; it < kK; ++it) {
    std::vector<uint64_t> r(blocks);
    for (auto& x : r) {
      x = dist(rng);
    }

    // t = B * r
    std::vector<int> t = MatVec(B, r, n);
    std::vector<uint64_t> tmask(blocks);
    for (int i = 0; i < n; ++i) {
      if (t[i]) {
        tmask[i / 64] |= 1ULL << (i % 64);
      }
    }

    // x = A * t
    std::vector<int> x = MatVec(A, tmask, n);
    // y = C * r
    std::vector<int> y = MatVec(C, r, n);

    if (x != y) {
      std::cout << "NO";
      return 0;
    }
  }

  std::cout << "YES";
}