#include <chrono>
#include <iostream>
#include <vector>


// 1) T(n) = 1, n <= 20
// T(n) = T(n-5) + T(n-8) + n^2 / 4, n > 20

// 2.1) Вычислим O(n):
// T(n) <= 2 * T(n-5) + n^2, n > 20
// Глубина рекурсии n/5
// Количество узлов 2^i
// Работа на узел (n-5i)^2
// Сумма{i=0; n/5} (n-5i)^2 * 2^i <= n^2 Сумма{i=0; n/5} 2^i = O(n^2 * 2^(n/5))
// T(n) = O(2^(n/5) * n^2)

// 2.2) Омега(n):
// T(n) >= 2 * T(n-8) + n^2 / 4, n > 20
// Глубина рекурсии n/8
// Количество узлов 2^i
// Работа на узел (n-8i)^2
// Сумма{i=0; n/8} (n/-8i)^2 * 2^i = n^2 * 2^(n/8)
// T(n) = Омега(2^(n/8) * n^2)
int64_t Algorithm1(std::vector<int>& A, const uint64_t n) {
  if (n <= 20) { // 1
    return A[n - 1]; // 1
  }
  auto x = Algorithm1(A, n - 5); // T(n-5)
  for (int i = 0; i < n / 2; i++) { // n/2
    for (int j = 0; j < n / 2; j++) { // n/2
      A[i] = A[i] - A[j]; // 1
    }
  }
  x = x + Algorithm1(A, n - 8); // T(n-8)
  return x; // 1
}


// 1) T(n) = 1, если n <= 50
// T(n) = 2T(n/4) + n/3 если n > 50

// 2) Вычислим Тета(n):
// Глубина дерева log4 n
// Количество узлов 2^i
// Работа на узел n/3 / (4)^i
// Сумма{i=0, log4 n}(n/3 * (2/4)^i) = n (1 - (1/2)^(log4 (n) + 1))
// Тета(n) = n
int64_t Algorithm2(std::vector<int>& A, const uint64_t n) {
  if (n <= 50) { // 1
    return A[n - 1]; // 1
  }
  auto x = Algorithm2(A, n / 4); // T(n/4)

  for (int i = 0; i < n / 3; i++) { // n/3
    A[i] = A[n - i] - A[i]; // 1
  }
  x = x + Algorithm2(A, n / 4); // T(n/4)
  return x; // 1
}

int main() {
    for (uint64_t i = 1; i < (1<<64)-1; i <<= 1) {
      std::vector<int> a(i, 0);
      auto start = std::chrono::high_resolution_clock::now();
      Algorithm2(a, i);
      auto stop = std::chrono::high_resolution_clock::now();
      auto duration = duration_cast<std::chrono::nanoseconds>(stop - start);

      std::cout << duration.count() << std::endl;
    }
}