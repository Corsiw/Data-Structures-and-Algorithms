#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <functional>

constexpr int kSeed = 52426967;

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

void SimpleQuickSortImpl(std::vector<int>& arr, const int l, const int r, std::mt19937& ran) {
  if (l >= r) {
    return;
  }

  const int pivot = Partition(arr, l, r, ran);
  SimpleQuickSortImpl(arr, l, pivot - 1, ran);
  SimpleQuickSortImpl(arr, pivot + 1, r, ran);
}

void SimpleQuickSort(std::vector<int>& arr) {
  std::mt19937 ran(kSeed);
  SimpleQuickSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, ran);
}

class ArrayGenerator {
  std::vector<int> random_array_;
  std::vector<int> reversed_array_;
  std::vector<int> almost_sorted_array_;

  static constexpr int kMaxSize = 100000;
  static constexpr int kMinValue = 0;
  static constexpr int kMaxValue = 10000;

  std::mt19937 rng_;

public:
  ArrayGenerator()
    : rng_(kSeed) {
    GenerateRandomArray();
    GenerateReversedArray();
    GenerateAlmostSortedArray();
  }

  void GenerateRandomArray() {
    random_array_.resize(kMaxSize);
    std::uniform_int_distribution<int> dist(kMinValue, kMaxValue);
    for (int i = 0; i < kMaxSize; i++) {
      random_array_[i] = dist(rng_);
    }
  }

  void GenerateReversedArray() {
    reversed_array_.resize(kMaxSize);
    for (int i = 0; i < kMaxSize; ++i) {
      reversed_array_[i] = kMaxValue - (i % (kMaxValue + 1));
    }
    std::ranges::sort(reversed_array_, std::greater<int>());
  }

  void GenerateAlmostSortedArray() {
    almost_sorted_array_.resize(kMaxSize);
    for (int i = 0; i < kMaxSize; ++i) {
      almost_sorted_array_[i] = i;
    }

    constexpr int kSwaps = kMaxSize / 100;
    std::uniform_int_distribution<int> dist(0, kMaxSize - 1);
    for (int i = 0; i < kSwaps; ++i) {
      const int a = dist(rng_);
      const int b = dist(rng_);
      std::swap(almost_sorted_array_[a], almost_sorted_array_[b]);
    }
  }

  std::vector<int> GetRandomArray(const size_t size) const {
    if (size > kMaxSize) {
      throw std::out_of_range("Size exceeds maximum allowed value");
    }
    return {random_array_.begin(), random_array_.begin() + size};
  }

  std::vector<int> GetReversedArray(const size_t size) const {
    if (size > kMaxSize) {
      throw std::out_of_range("Size exceeds maximum allowed value");
    }
    return {reversed_array_.begin(), reversed_array_.begin() + size};
  }

  std::vector<int> GetAlmostSortedArray(const size_t size) const {
    if (size > kMaxSize) {
      throw std::out_of_range("Size exceeds maximum allowed value");
    }
    return {almost_sorted_array_.begin(), almost_sorted_array_.begin() + size};
  }
};

class SortTester {
  int repeats_;

public:
  explicit SortTester(const int repeats): repeats_(repeats) {
  }

  int64_t Measure(const std::function<void(std::vector<int>&)>& sorter, const std::vector<int>& base_array) const {
    std::vector<int64_t> times;
    times.reserve(repeats_);

    for (int i = 0; i < repeats_; ++i) {
      std::vector<int> arr = base_array; // копия исходных данных
      const auto start = std::chrono::high_resolution_clock::now();
      sorter(arr);
      const auto end = std::chrono::high_resolution_clock::now();

      const int64_t ns =
          std::chrono::duration<int64_t, std::nano>(end - start).count();
      times.push_back(ns);
    }

    // Сортируем и берём медиану
    std::ranges::sort(times);
    if (repeats_ % 2 == 1) {
      return times[repeats_ / 2];
    }
    return (times[repeats_ / 2 - 1] + times[repeats_ / 2]) / 2;
  }
};


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  const std::ofstream out("out.csv");
  std::streambuf* out_buf = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  ArrayGenerator gen;
  const SortTester tester(20); // 20 повторов для каждого измерения

  std::vector<int64_t> res(6);

  for (int i = 500; i <= 100000; i += 100) {
    auto rand_arr = gen.GetRandomArray(i);
    auto rev_arr = gen.GetReversedArray(i);
    auto almost_arr = gen.GetAlmostSortedArray(i);

    res[0] = tester.Measure(SimpleQuickSort, rand_arr);
    res[1] = tester.Measure(SimpleQuickSort, rev_arr);
    res[2] = tester.Measure(SimpleQuickSort, almost_arr);

    res[3] = tester.Measure(IntroSort, rand_arr);
    res[4] = tester.Measure(IntroSort, rev_arr);
    res[5] = tester.Measure(IntroSort, almost_arr);

    std::cout << i;
    for (const auto t : res) {
      std::cout << " " << (t);
    }
    std::cout << "\n";
  }

  std::cout.rdbuf(out_buf);
}