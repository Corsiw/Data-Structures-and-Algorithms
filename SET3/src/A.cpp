#include <iostream>
#include <vector>

void heapify(std::vector<int>& vec, const int n, const int i) {
  int largest = i;
  const int l = 2 * i + 1;
  const int r = 2 * i + 2;

  if (l < n && vec[l] > vec[largest]) {
    largest = l;
  }
  if (r < n && vec[r] > vec[largest]) {
    largest = r;
  }

  if (largest != i) {
    std::swap(vec[i], vec[largest]);
    heapify(vec, n, largest);
  }
}

void buildMaxHeap(std::vector<int>& vec, const int n) {
  for (int i = n / 2 - 1; i >= 0; i--) {
    heapify(vec, n, i);
  }
}

void heapSort(std::vector<int>& vec) {
  buildMaxHeap(vec, static_cast<int>(vec.size()));
  for (int i = static_cast<int>(vec.size()) - 1; i > 0; i--) {
    std::swap(vec[0], vec[i]);
    heapify(vec, i, 0);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int> vec(n);
  for (int i = 0; i < n; i++) {
    std::cin >> vec[i];
  }

  heapSort(vec);

  for (int i = 0; i < n; i++) {
    std::cout << vec[i] << " ";
  }
}