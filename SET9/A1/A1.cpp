#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

constexpr int kSeed = 52426967;

static const std::string kAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                     "abcdefghijklmnopqrstuvwxyz"
                                     "0123456789"
                                     "!@#%:;^&*()-";

class StringGenerator {
public:
  explicit StringGenerator(const size_t sample_size,
                           const uint32_t seed = std::random_device{}())
      : sample_size_(sample_size), rng_(seed) {

    GenerateRandomSample();
    GenerateReversedSample();
    GenerateAlmostSortedSample();
  }

  std::vector<std::string> GetPrefixByCountRandom(size_t count) const {
    count = std::min(count, sample_size_);
    return {random_sample_.begin(), random_sample_.begin() + count};
  }

  std::vector<std::string> GetPrefixByCountReversed(size_t count) const {
    count = std::min(count, sample_size_);
    return {reversed_sample_.begin(), reversed_sample_.begin() + count};
  }

  std::vector<std::string> GetPrefixByCountAlmostSorted(size_t count) const {
    count = std::min(count, sample_size_);
    return {almost_sorted_sample_.begin(),
            almost_sorted_sample_.begin() + count};
  }

private:
  std::string GenerateRandomString() {
    std::uniform_int_distribution<int> len_dist(10, 200);
    std::uniform_int_distribution<int> char_dist(
        0, static_cast<int>(kAlphabet.size()) - 1);

    const int len = len_dist(rng_);
    std::string s;
    s.reserve(len);

    for (int i = 0; i < len; ++i) {
      s.push_back(kAlphabet[char_dist(rng_)]);
    }
    return s;
  }

  void GenerateRandomSample() {
    random_sample_.reserve(sample_size_);
    for (size_t i = 0; i < sample_size_; ++i) {
      random_sample_.push_back(GenerateRandomString());
    }
  }

  void GenerateReversedSample() {
    reversed_sample_.reserve(sample_size_);
    for (size_t i = 0; i < sample_size_; ++i) {
      reversed_sample_.push_back(GenerateRandomString());
    }

    std::sort(reversed_sample_.begin(), reversed_sample_.end(),
              std::greater<std::string>());
  }

  void GenerateAlmostSortedSample() {
    almost_sorted_sample_.reserve(sample_size_);
    for (size_t i = 0; i < sample_size_; ++i) {
      almost_sorted_sample_.push_back(GenerateRandomString());
    }
    std::sort(almost_sorted_sample_.begin(), almost_sorted_sample_.end());

    const size_t swaps = almost_sorted_sample_.size() / 100;
    std::uniform_int_distribution<int> dist(
        0, static_cast<int>(almost_sorted_sample_.size()) - 1);
    for (int i = 0; i < swaps; ++i) {
      const int a = dist(rng_);
      const int b = dist(rng_);
      std::swap(almost_sorted_sample_[a], almost_sorted_sample_[b]);
    }
  }

  size_t sample_size_;
  std::mt19937 rng_;
  std::vector<std::string> random_sample_;
  std::vector<std::string> reversed_sample_;
  std::vector<std::string> almost_sorted_sample_;
};

size_t g_char_comparisons = 0;

struct SortStats {
  int64_t time_ns;
  size_t char_comparisons;
};

class SortTester {
  int repeats_;

public:
  explicit SortTester(const int repeats) : repeats_(repeats) {}

  SortStats
  Measure(const std::function<void(std::vector<std::string> &)> &sorter,
          const std::vector<std::string> &base_array) const {

    std::vector<int64_t> times;
    std::vector<size_t> comparisons;

    times.reserve(repeats_);
    comparisons.reserve(repeats_);

    for (int i = 0; i < repeats_; ++i) {
      std::vector<std::string> arr = base_array;

      g_char_comparisons = 0;

      const auto start = std::chrono::high_resolution_clock::now();
      sorter(arr);
      const auto end = std::chrono::high_resolution_clock::now();

      const int64_t ns =
          std::chrono::duration<int64_t, std::nano>(end - start).count();

      times.push_back(ns);
      comparisons.push_back(g_char_comparisons);
    }

    std::sort(times.begin(), times.end());
    std::sort(comparisons.begin(), comparisons.end());

    return {times[repeats_ / 2], comparisons[repeats_ / 2]};
  }
};

bool LessOrEqual(const std::string &a, const std::string &b) {
  const size_t min_len = std::min(a.size(), b.size());

  for (size_t i = 0; i < min_len; ++i) {
    ++g_char_comparisons;

    if (a[i] < b[i]) {
      return true;
    }
    if (a[i] > b[i]) {
      return false;
    }
  }

  ++g_char_comparisons;
  return a.size() <= b.size();
}

int Partition(std::vector<std::string> &vec, const int l, const int r,
              std::mt19937 &gen) {
  std::uniform_int_distribution<int> dist(l, r);
  const int pivot_index = dist(gen);
  std::swap(vec[pivot_index], vec[r]);
  const std::string &pivot = vec[r];

  int i = l - 1;
  for (int j = l; j < r; ++j) {
    if (LessOrEqual(vec[j], pivot)) {
      ++i;
      std::swap(vec[i], vec[j]);
    }
  }
  std::swap(vec[i + 1], vec[r]);
  return i + 1;
}

void SimpleQuickSortImpl(std::vector<std::string> &arr, const int l,
                         const int r, std::mt19937 &rng) {
  if (l >= r) {
    return;
  }

  const int pivot = Partition(arr, l, r, rng);
  SimpleQuickSortImpl(arr, l, pivot - 1, rng);
  SimpleQuickSortImpl(arr, pivot + 1, r, rng);
}

void SimpleQuickSort(std::vector<std::string> &arr) {
  std::mt19937 ran(kSeed);
  SimpleQuickSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, ran);
}

void StringMerge(std::vector<std::string> &arr, const int l, const int mid,
                 const int r, std::vector<std::string> &temp) {
  int i = l;
  int j = mid + 1;
  int k = l;

  for (int x = l; x <= r; ++x) {
    temp[x] = arr[x];
  }

  while (i <= mid && j <= r) {
    if (LessOrEqual(temp[i], temp[j])) {
      arr[k++] = temp[i++];
    } else {
      arr[k++] = temp[j++];
    }
  }

  while (i <= mid) {
    arr[k++] = temp[i++];
  }
}

void SimpleMergeSortImpl(std::vector<std::string> &arr, const int left,
                         const int right, std::vector<std::string> &temp) {
  if (left >= right) {
    return;
  }

  const int mid = left + (right - left) / 2;
  SimpleMergeSortImpl(arr, left, mid, temp);
  SimpleMergeSortImpl(arr, mid + 1, right, temp);

  if (LessOrEqual(arr[mid], arr[mid + 1])) {
    return;
  }
  StringMerge(arr, left, mid, right, temp);
}

void SimpleMergeSort(std::vector<std::string> &arr) {
  if (arr.empty()) {
    return;
  }
  std::vector<std::string> temp(arr.size());
  SimpleMergeSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}

class StringLCP {
public:
  std::string str;
  int lcp;
};

std::pair<bool, int> LcpCompare(const std::string &a, const std::string &b,
                                const int k) {
  const size_t min_len = std::min(a.size(), b.size());

  int i = k;
  for (; i < min_len; ++i) {
    g_char_comparisons++;
    if (a[i] < b[i]) {
      return {true, i};
    }
    g_char_comparisons++;
    if (a[i] > b[i]) {
      return {false, i};
    }
  }

  return {a.size() <= b.size(), i};
}

void StringMerge(std::vector<StringLCP> &arr,
                                   std::vector<StringLCP> &temp, const int l,
                                   const int m, const int r) {

  std::copy(arr.begin() + l, arr.begin() + r, temp.begin() + l);

  int i = l;
  int j = m;
  int k = l;

  int ki = temp[i].lcp;
  int pj = temp[j].lcp;

  while (i < m && j < r) {

    if (ki > pj) {
      arr[k++] = temp[i++];
      if (i < m) {
        ki = temp[i].lcp;
      }
    } else if (ki < pj) {
      arr[k++] = temp[j++];
      if (j < r) {
        pj = temp[j].lcp;
      }
    } else {

      auto [is_less, h] = LcpCompare(temp[i].str, temp[j].str, ki);

      if (is_less) {
        arr[k++] = temp[i++];
        if (i < m) {
          ki = temp[i].lcp;
        }

        if (j < r) {
          temp[j].lcp = h;
          pj = h;
        }
      } else {
        arr[k++] = temp[j++];
        if (j < r) {
          pj = temp[j].lcp;
        }

        if (i < m) {
          temp[i].lcp = h;
          ki = h;
        }
      }
    }
  }

  while (i < m) {
    arr[k++] = temp[i++];
  }

  while (j < r) {
    arr[k++] = temp[j++];
  }

  if (l < r) {
    arr[l].lcp = 0;
  }
}

void StringMergeSortImpl(std::vector<StringLCP> &arr,
                         std::vector<StringLCP> &temp, const int l,
                         const int r) {

  if (r - l <= 1) {
    if (r - l == 1) {
      arr[l].lcp = 0;
    }
    return;
  }

  const int mid = l + (r - l) / 2;

  StringMergeSortImpl(arr, temp, l, mid);
  StringMergeSortImpl(arr, temp, mid, r);

  StringMerge(arr, temp, l, mid, r);
}

void StringMergeSort(std::vector<std::string> &arr) {
  if (arr.size() < 2) {
    return;
  }

  std::vector<StringLCP> data;
  data.reserve(arr.size());
  for (const auto &s : arr) {
    data.push_back({s, 0});
  }

  std::vector<StringLCP> temp(arr.size());

  StringMergeSortImpl(data, temp, 0, static_cast<int>(data.size()));
  for (int i = 0; i < data.size(); ++i) {
    arr[i] = std::move(data[i].str);
  }
}

int CharAt(const std::string &s, const int d) {
  if (d < 0 || d >= s.size()) {
    return -1;
  }
  return s[d];
}

std::pair<int, int> Partition(std::vector<std::string> &vec, const int l,
                              const int r, const int d, std::mt19937 &gen) {
  std::uniform_int_distribution<int> dist(l, r);
  const int pivot_index = dist(gen);
  const int pivot = CharAt(vec[pivot_index], d);

  int write_index_less = l;
  int write_index_greater = r;

  for (int j = l; j <= write_index_greater;) {
    g_char_comparisons++;
    if (vec[j][d] < pivot) {
      if (j == write_index_less) {
        j++;
        write_index_less++;
      } else {
        std::swap(vec[write_index_less++], vec[j++]);
      }
    } else {
      g_char_comparisons++;
      if (vec[j][d] > pivot) {
        std::swap(vec[write_index_greater--], vec[j]);
      } else {
        j++;
      }
    }
  }

  return {write_index_less, write_index_greater};
}

void StringQuickSortImpl(std::vector<std::string> &arr, const int l,
                         const int r, const int d, std::mt19937 &rng) {
  if (l >= r) {
    return;
  }

  const auto [fst, snd] = Partition(arr, l, r, d, rng);
  StringQuickSortImpl(arr, l, fst - 1, d, rng);
  StringQuickSortImpl(arr, fst, snd, d + 1, rng);
  StringQuickSortImpl(arr, snd + 1, r, d, rng);
}

void StringQuickSort(std::vector<std::string> &arr) {
  std::mt19937 ran(kSeed);
  StringQuickSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, 0, ran);
}

constexpr int kR = 256;

void MSDRadixSortImpl(std::vector<std::string> &a,
                      std::vector<std::string> &aux, int l, int r, int d) {
  if (r <= l) {
    return;
  }

  std::array<int, kR + 2> count{};
  std::array<int, kR + 2> start{};

  for (int i = l; i <= r; ++i) {
    ++count[CharAt(a[i], d) + 2];
  }

  for (int i = 0; i < kR + 1; ++i) {
    count[i + 1] += count[i];
  }

  start = count;

  for (int i = l; i <= r; ++i) {
    int c = CharAt(a[i], d);
    aux[l + count[c + 1]++] = std::move(a[i]);
  }

  for (int i = l; i <= r; ++i) {
    a[i] = std::move(aux[i]);
  }

  for (int rdx = 1; rdx <= kR; ++rdx) {
    int bucket_l = l + start[rdx - 1];
    int bucket_r = l + start[rdx] - 1;

    if (bucket_l < bucket_r) {
      MSDRadixSortImpl(a, aux, bucket_l, bucket_r, d + 1);
    }
  }
}

void MSDRadixSort(std::vector<std::string> &arr) {
  if (arr.empty()) {
    return;
  }

  std::vector<std::string> aux(arr.size());

  MSDRadixSortImpl(arr, aux, 0, static_cast<int>(arr.size()) - 1, 0);
}

constexpr int kQuickSortThreshold = 74;

void MSDRadixQuickSortImpl(std::vector<std::string>& a,
                           std::vector<std::string>& aux,
                           int l,
                           int r,
                           int d,
                           std::mt19937& rng) {
  if (r <= l) {
    return;
  }

  int size = r - l + 1;

  if (size < kQuickSortThreshold) {
    StringQuickSortImpl(
        a,
        l,
        r,
        d,
        rng);

    return;
  }

  std::array<int, kR + 2> count{};
  std::array<int, kR + 2> start{};

  for (int i = l; i <= r; ++i) {
    ++count[CharAt(a[i], d) + 2];
  }

  for (int i = 0; i < kR + 1; ++i) {
    count[i + 1] += count[i];
  }

  start = count;

  for (int i = l; i <= r; ++i) {
    int c = CharAt(a[i], d);
    aux[l + count[c + 1]++] = std::move(a[i]);
  }

  for (int i = l; i <= r; ++i) {
    a[i] = std::move(aux[i]);
  }

  for (int rdx = 1; rdx <= kR; ++rdx) {
    int bucket_l = l + start[rdx - 1];
    int bucket_r = l + start[rdx] - 1;

    if (bucket_l < bucket_r) {
      MSDRadixQuickSortImpl(
          a,
          aux,
          bucket_l,
          bucket_r,
          d + 1,
          rng);
    }
  }
}

void MSDRadixQuickSort(std::vector<std::string>& arr) {
  if (arr.empty()) {
    return;
  }

  std::vector<std::string> aux(arr.size());

  std::mt19937 rng(kSeed);

  MSDRadixQuickSortImpl(
      arr,
      aux,
      0,
      static_cast<int>(arr.size()) - 1,
      0,
      rng);
}

constexpr size_t kSampleSize = 3000;
constexpr size_t kSampleSizeLow = 100;
constexpr size_t kSampleSizeStep = 100;

void GenerateSimpleSortsResultCsv() {
  std::ofstream out("out2.csv");
  std::streambuf *out_buf = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());
  std::cout << "SampleSize,QSRandomTimeNs,QSRandomComp,QSReversedTimeNs,"
               "QSReversedComp,QSAlmostSortedTimeNs,QSAlmostSortedComp,"
               "MSRandomTimeNs,MSRandomComp,MSReversedTimeNs,MSReversedComp,"
               "MSAlmostSortedTimeNs,MSAlmostSortedComp\n";

  StringGenerator gen(kSampleSize);
  const SortTester tester(20);

  std::vector<SortStats> res(6);

  for (int i = kSampleSizeLow; i <= kSampleSize; i += kSampleSizeStep) {
    auto rand_arr = gen.GetPrefixByCountRandom(i);
    auto rev_arr = gen.GetPrefixByCountReversed(i);
    auto almost_arr = gen.GetPrefixByCountAlmostSorted(i);

    res[0] = tester.Measure(SimpleQuickSort, rand_arr);
    res[1] = tester.Measure(SimpleQuickSort, rev_arr);
    res[2] = tester.Measure(SimpleQuickSort, almost_arr);

    res[3] = tester.Measure(SimpleMergeSort, rand_arr);
    res[4] = tester.Measure(SimpleMergeSort, rev_arr);
    res[5] = tester.Measure(SimpleMergeSort, almost_arr);

    std::cout << i;
    for (const auto [time_ns, char_comparisons] : res) {
      std::cout << "," << time_ns << "," << char_comparisons;
    }
    std::cout << "\n";
  }

  std::cout.rdbuf(out_buf);
}

void GenerateStringSortsResultCsv() {
  std::ofstream out("out3.csv");
  std::streambuf *out_buf = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  std::cout << "SampleSize,"

            << "SMRandomTimeNs,SMRandomComp,"
            << "SMReversedTimeNs,SMReversedComp,"
            << "SMAlmostSortedTimeNs,SMAlmostSortedComp,"

            << "SQRandomTimeNs,SQRandomComp,"
            << "SQReversedTimeNs,SQReversedComp,"
            << "SQAlmostSortedTimeNs,SQAlmostSortedComp,"

            << "MSDRandomTimeNs,MSDRandomComp,"
            << "MSDReversedTimeNs,MSDReversedComp,"
            << "MSDAlmostSortedTimeNs,MSDAlmostSortedComp,"

            << "MSDHRandomTimeNs,MSDHRandomComp,"
            << "MSDHReversedTimeNs,MSDHReversedComp,"
            << "MSDHAlmostSortedTimeNs,MSDHAlmostSortedComp\n";

  StringGenerator gen(kSampleSize);
  const SortTester tester(20);

  std::vector<SortStats> res(12);

  for (int i = kSampleSizeLow; i <= kSampleSize; i += kSampleSizeStep) {
    auto rand_arr = gen.GetPrefixByCountRandom(i);
    auto rev_arr = gen.GetPrefixByCountReversed(i);
    auto almost_arr = gen.GetPrefixByCountAlmostSorted(i);

    res[0] = tester.Measure(StringMergeSort, rand_arr);
    res[1] = tester.Measure(StringMergeSort, rev_arr);
    res[2] = tester.Measure(StringMergeSort, almost_arr);

    res[3] = tester.Measure(StringQuickSort, rand_arr);
    res[4] = tester.Measure(StringQuickSort, rev_arr);
    res[5] = tester.Measure(StringQuickSort, almost_arr);

    res[6] = tester.Measure(MSDRadixSort, rand_arr);
    res[7] = tester.Measure(MSDRadixSort, rev_arr);
    res[8] = tester.Measure(MSDRadixSort, almost_arr);

    res[9] = tester.Measure(MSDRadixQuickSort, rand_arr);
    res[10] = tester.Measure(MSDRadixQuickSort, rev_arr);
    res[11] = tester.Measure(MSDRadixQuickSort, almost_arr);

    std::cout << i;

    for (const auto [time_ns, char_comparisons] : res) {
      std::cout << "," << time_ns << "," << char_comparisons;
    }

    std::cout << "\n";
  }

  std::cout.rdbuf(out_buf);
}
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  GenerateSimpleSortsResultCsv();
  GenerateStringSortsResultCsv();
}