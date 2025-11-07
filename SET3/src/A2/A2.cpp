#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <functional>

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

void Merge(std::vector<int>& arr, const int l, const int mid, const int r, std::vector<int>& temp) {
  int i = l;
  int j = mid + 1;
  int k = l;

  for (int x = l; x <= r; ++x) {
    temp[x] = arr[x];
  }

  while (i <= mid && j <= r) {
    if (temp[i] <= temp[j]) {
      arr[k++] = temp[i++];
    } else {
      arr[k++] = temp[j++];
    }
  }

  while (i <= mid) {
    arr[k++] = temp[i++];
  }
}

void MergeSort(std::vector<int>& arr, const int left, const int right, std::vector<int>& temp) {
  if (left >= right) {
    return;
  }

  if (right - left <= 15) {
    InsertionSort(arr, left, right);
    return;
  }

  const int mid = left + (right - left) / 2;
  MergeSort(arr, left, mid, temp);
  MergeSort(arr, mid + 1, right, temp);

  if (arr[mid] <= arr[mid + 1]) {
    return;
  }
  Merge(arr, left, mid, right, temp);
}

void MergeSort(std::vector<int>& arr) {
  if (arr.empty()) {
    return;
  }
  std::vector<int> temp(arr.size());
  MergeSort(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}

void SimpleMergeSort(std::vector<int>& arr, const int left, const int right, std::vector<int>& temp) {
  if (left >= right) {
    return;
  }

  const int mid = left + (right - left) / 2;
  SimpleMergeSort(arr, left, mid, temp);
  SimpleMergeSort(arr, mid + 1, right, temp);

  if (arr[mid] <= arr[mid + 1]) {
    return;
  }
  Merge(arr, left, mid, right, temp);
}

void SimpleMergeSort(std::vector<int>& arr) {
  if (arr.empty()) {
    return;
  }
  std::vector<int> temp(arr.size());
  SimpleMergeSort(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}

constexpr int kSeed = 52426967;

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
  SortTester tester(20); // 20 повторов для каждого измерения

  std::vector<int64_t> res(6);

  for (int i = 500; i <= 100000; i += 100) {
    auto rand_arr = gen.GetRandomArray(i);
    auto rev_arr = gen.GetReversedArray(i);
    auto almost_arr = gen.GetAlmostSortedArray(i);

    res[0] = tester.Measure(static_cast<void(*)(std::vector<int>& arr)>(SimpleMergeSort), rand_arr);
    res[1] = tester.Measure(static_cast<void(*)(std::vector<int>& arr)>(SimpleMergeSort), rev_arr);
    res[2] = tester.Measure(static_cast<void(*)(std::vector<int>& arr)>(SimpleMergeSort), almost_arr);

    res[3] = tester.Measure(static_cast<void(*)(std::vector<int>& arr)>(MergeSort), rand_arr);
    res[4] = tester.Measure(static_cast<void(*)(std::vector<int>& arr)>(MergeSort), rev_arr);
    res[5] = tester.Measure(static_cast<void(*)(std::vector<int>& arr)>(MergeSort), almost_arr);

    std::cout << i;
    for (auto t : res) {
      std::cout << " " << (t);
    }
    std::cout << "\n";
  }

  std::cout.rdbuf(out_buf);
}