#include <iostream>
#include <random>
#include <string>
#include <vector>

constexpr int kSeed = 52426967;

int CharAt(const std::string& s, const int d) {
  if (d < 0 || d >= s.size()) {
    return -1;
  }
  return s[d];
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
    }
    else if (vec[j][d] > pivot) {
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

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<std::string> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  SimpleQuickSort(arr);

  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << "\n";
  }
}