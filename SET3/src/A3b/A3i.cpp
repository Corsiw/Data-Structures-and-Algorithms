#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

constexpr int64_t kSeed = 52426967;

void InsertionSort(std::vector<int>& arr, const int l, const int r) {
  for (int i = l + 1; i <= r; ++i) {
    const int key = arr[i];
    int j = i - 1;
    while (j >= l && arr[j] > key) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}

void Heapify(std::vector<int>& vec, const int n, const int i, const int offset) {
  int largest = i;
  const int l = 2 * (i - offset) + 1 + offset;
  const int r = 2 * (i - offset) + 2 + offset;

  if (l < offset + n && vec[l] > vec[largest]) {
    largest = l;
  }
  if (r < offset + n && vec[r] > vec[largest]) {
    largest = r;
  }

  if (largest != i) {
    std::swap(vec[i], vec[largest]);
    Heapify(vec, n, largest, offset);
  }
}

void BuildMaxHeap(std::vector<int>& vec, const int l, const int r) {
  const int n = r - l + 1;
  for (int i = l + n / 2 - 1; i >= l; i--) {
    Heapify(vec, n, i, l);
  }
}

void HeapSort(std::vector<int>& vec, const int l, const int r) {
  BuildMaxHeap(vec, l, r);
  for (int i = r; i > l; i--) {
    std::swap(vec[l], vec[i]);
    Heapify(vec, i - l, l, l);
  }
}

int Partition(std::vector<int>& vec, const int l, const int r, std::mt19937& gen) {
  std::uniform_int_distribution<int> dist(l, r);
  const int pivot_index = dist(gen);
  std::swap(vec[pivot_index], vec[r]);
  const int& pivot = vec[r];

  int i = l - 1;
  for (int j = l; j < r; ++j) {
    if (vec[j] <= pivot) {
      ++i;
      std::swap(vec[i], vec[j]);
    }
  }
  std::swap(vec[i + 1], vec[r]);
  return i + 1;
}

void IntroSortImpl(std::vector<int>& arr, const int l, const int r, const int depth_limit, std::mt19937& ran) {
  const int size = r - l + 1;
  if (size <= 1) {
    return;
  }
  if (size < 16) {
    InsertionSort(arr, l, r);
    return;
  }
  if (depth_limit == 0) {
    HeapSort(arr, l, r);
    return;
  }

  const int pivot = Partition(arr, l, r, ran);
  IntroSortImpl(arr, l, pivot - 1, depth_limit - 1, ran);
  IntroSortImpl(arr, pivot + 1, r, depth_limit - 1, ran);
}

void IntroSort(std::vector<int>& arr) {
  std::mt19937 ran(kSeed);
  const int max_depth = static_cast<int>(2 * log2(static_cast<double>(arr.size())));
  IntroSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, max_depth, ran);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int> arr(n);
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  IntroSort(arr);

  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
}