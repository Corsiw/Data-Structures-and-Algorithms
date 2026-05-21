#include <iosfwd>
#include <iostream>
#include <vector>
#include <array>

int CharAt(const std::string& s, int d) {
  if (d >= static_cast<int>(s.size())) {
    return -1;
  }

  return static_cast<unsigned char>(s[d]);
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

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<std::string> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  MSDRadixSort(arr);

  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << "\n";
  }
}