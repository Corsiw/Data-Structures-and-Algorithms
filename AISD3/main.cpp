#include <iostream>
#include <vector>
#include <climits>

// n и k - переменные. Внутренний цикл работает за O(k).
// Внешний цикл работает за O(n-k+1)
// Все остальное работает за константу.
// Тогда в целом алгоритм работает за (n-k+1) * k <= n * k
// То есть асимптотика O(n*k)
int LongFindMaxSum(const std::vector<int>& arr, int k) {
  int n = arr.size();
  int max_sum = INT_MIN;

  for (int i = 0; i <= n - k; ++i) {
    int current_sum = 0;
    for (int j = i; j < i + k; ++j) {
      current_sum += arr[j];
    }
    max_sum = std::max(max_sum, current_sum);
  }

  return max_sum;
}

// Используем Sliding Window.
// Первый цикл выполняется за k итераций
// Второй цикл за n-k итераций
// Все остальное выполняется за константу
// Тогда Алгоритм выполняется за k + (n - k) = n
// Тогда асимптотика O(n)
int LongFindMaxSumOptimized(const std::vector<int>& arr, const int k) {
  const size_t n = arr.size();
  if (n < k || k <= 0) {
    return INT_MIN;
  }

  int current_sum = 0;
  for (size_t i = 0; i < k; ++i) {
    current_sum += arr[i];
  }

  int max_sum = current_sum;
  for (size_t i = k; i < n; ++i) {
    current_sum = current_sum + arr[i] - arr[i - k];
    max_sum = std::max(max_sum, current_sum);
  }

  return max_sum;
}

int main() {
  std::vector<int> arr = {1, 2, 3, 4, 5, 6};
  std::cout << LongFindMaxSum(arr, 3) << " " << LongFindMaxSumOptimized(arr, 3);
}