#include <iosfwd>
#include <iostream>
#include <vector>

int CharAt(const std::string& s, int d) {
  if (d >= static_cast<int>(s.size())) {
    return -1;
  }

  return static_cast<unsigned char>(s[d]);
}

constexpr int kR = 256;

void MSDRadixSortImpl(std::vector<std::string>& a, const int l, const int r, const int d) {
  if (l >= r) {
    return;
  }
  std::vector<int> count(kR + 2, 0);
  std::vector<std::string> aux(r - l + 1);

  for (int i = l; i <= r; ++i) {
    const int c = CharAt(a[i], d);
    count[c + 2]++;
  }

  for (int i = 0; i < kR + 1; ++i) {
    count[i + 1] += count[i];
  }

  for (int i = l; i <= r; ++i) {
    const int c = CharAt(a[i], d);
    aux[count[c + 1]++] = a[i];
  }

  for (int i = l; i <= r; ++i) {
    a[i] = aux[i - l];
  }

  for (int i = 0; i < kR; ++i) {
    MSDRadixSortImpl(
        a,
        l + count[i],
        l + count[i + 1] - 1,
        d + 1
        );
  }
}

void MSDRadixSort(std::vector<std::string>& arr) {
  MSDRadixSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, 0);
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