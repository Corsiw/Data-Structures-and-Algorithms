#include <iostream>
#include <vector>

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
    if (a[i] < b[i]) {
      return {true, i};
    }
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

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<std::string> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  StringMergeSort(arr);

  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << "\n";
  }
}