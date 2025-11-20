#include <iostream>
#include <vector>

// Рекурсивный обход in-order для дерева,
// хранящегося по уровням в массиве.
// Т.к. сделали in-order обход, то получили индексы, отсортированных элементов
// самый большой - третий с конца
void Inorder(const int idx, const int n, std::vector<int>& order) {
  if (idx >= n) {
    return;
  }
  Inorder(2 * idx + 1, n, order); // левый
  order.push_back(idx); // корень
  Inorder(2 * idx + 2, n, order); // правый
}

int main() {
  constexpr int kN = 1000;
  std::vector<int> order;
  order.reserve(kN);

  Inorder(0, kN, order);

  const int third_largest_index = order[kN - 3];

  std::cout << "Index of third largest element = "
      << third_largest_index << '\n';

  return 0;
}