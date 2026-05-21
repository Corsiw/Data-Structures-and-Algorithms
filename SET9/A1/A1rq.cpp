#include <iosfwd>
#include <iostream>
#include <random>
#include <array>
#include <vector>

const std::string kAlphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-";

constexpr int kSeed = 52426967;

int CharAt(const std::string& s, int d) {
  if (d >= static_cast<int>(s.size())) {
    return -1;
  }

  return static_cast<unsigned char>(s[d]);
}

std::pair<int, int> Partition(std::vector<std::string>& vec, const int l, const int r, const int d, std::mt19937& gen) {
  std::uniform_int_distribution<int> dist(l, r);
  const int pivot_index = dist(gen);
  const int pivot = CharAt(vec[pivot_index], d);

  int write_index_less = l;
  int write_index_greater = r;

  for (int j = l; j <= write_index_greater;) {
    if (vec[j][d] < pivot) {
      if (j == write_index_less) {
        j++;
        write_index_less++;
      } else {
        std::swap(vec[write_index_less++], vec[j++]);
      }
    } else if (vec[j][d] > pivot) {
      std::swap(vec[write_index_greater--], vec[j]);
    } else {
      j++;
    }
  }

  return {write_index_less, write_index_greater};
}

void StringQuickSortImpl(std::vector<std::string>& arr, const int l, const int r, const int d, std::mt19937& rng) {
  if (l >= r) {
    return;
  }

  const auto [fst, snd] = Partition(arr, l, r, d, rng);
  StringQuickSortImpl(arr, l, fst - 1, d, rng);
  StringQuickSortImpl(arr, fst, snd, d + 1, rng);
  StringQuickSortImpl(arr, snd + 1, r, d, rng);
}

void SimpleQuickSort(std::vector<std::string>& arr) {
  std::mt19937 ran(kSeed);
  StringQuickSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, 0, ran);
}

constexpr int kR = 256;

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

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<std::string> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  MSDRadixQuickSort(arr);

  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << "\n";
  }
}