#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

constexpr int kSeed = 52426967;
double T = 30.0;
double cooling_rate = 0.99;
int max_steps = 1000000;

int Attacks(const int k) {
  return k * (k - 1) / 2;
}

int CountAttacks(const std::vector<int>& col_count,
                 const std::vector<int>& diag1,
                 const std::vector<int>& diag2) {
  int res = 0;
  for (const int x : col_count) {
    res += Attacks(x);
  }
  for (const int x : diag1) {
    res += Attacks(x);
  }
  for (const int x : diag2) {
    res += Attacks(x);
  }
  return res;
}

void SwapAndUpdate(std::vector<int>& cols,
                   std::vector<int>& col_count,
                   std::vector<int>& diag1,
                   std::vector<int>& diag2,
                   const int r1, const int r2) {
  const int n = static_cast<int>(cols.size());
  int c1 = cols[r1];
  int c2 = cols[r2];

  col_count[c1]--;
  col_count[c2]--;
  diag1[r1 - c1 + n - 1]--;
  diag1[r2 - c2 + n - 1]--;
  diag2[r1 + c1]--;
  diag2[r2 + c2]--;

  std::ranges::swap(cols[r1], cols[r2]);
  c1 = cols[r1];
  c2 = cols[r2];

  col_count[c1]++;
  col_count[c2]++;
  diag1[r1 - c1 + n - 1]++;
  diag1[r2 - c2 + n - 1]++;
  diag2[r1 + c1]++;
  diag2[r2 + c2]++;
}

std::pair<std::vector<int>, int> QueensGambit(const int n) {
  std::mt19937 rng(kSeed);
  std::uniform_int_distribution<int> dist(0, n - 1);
  std::uniform_real_distribution<double> uni(0.0, 1.0);

  std::vector<int> cols(n);
  iota(cols.begin(), cols.end(), 0);
  std::ranges::shuffle(cols.begin(), cols.end(), rng);

  std::vector<int> col_count(n, 0);
  std::vector<int> diag1(2 * n - 1, 0);
  std::vector<int> diag2(2 * n - 1, 0);
  for (int r = 0; r < n; ++r) {
    const int c = cols[r];
    col_count[c]++;
    diag1[r - c + n - 1]++;
    diag2[r + c]++;
  }

  int attacks = CountAttacks(col_count, diag1, diag2);

  for (int step = 0; step < max_steps && attacks > 0; ++step) {
    int r1 = dist(rng);
    int r2 = dist(rng);
    if (r1 == r2) {
      continue;
    }
    // const int dF = CountDeltaAttacks(cols, col_count, diag1, diag2, r1, r2);

    SwapAndUpdate(cols, col_count, diag1, diag2, r1, r2);
    const auto new_F = CountAttacks(col_count, diag1, diag2);
    const int dF = new_F - attacks;

    if (dF < 0 || uni(rng) < exp(-dF / T)) {
      attacks += dF;
    } else {
      SwapAndUpdate(cols, col_count, diag1, diag2, r1, r2);
    }

    T *= cooling_rate;
  }

  return {cols, attacks};
}

int main() {
  int n = 0;
  std::cin >> n;

  const auto res = QueensGambit(n);
  for (int i = 0; i < n; i++) {
    std::cout << res.first[i] + 1 << " ";
  }
}