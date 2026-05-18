#include <iostream>
#include <vector>

struct StringLCP {
  std::string str;
  int lcp;
};

std::pair<bool, int> LcpCompare(const std::string& a, const std::string& b, const int k) {
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

  if (a.size() <= b.size()) {
    return {true, i};
  }
  return {false, i};
}

std::vector<StringLCP> StringMerge(const std::vector<StringLCP>& P, const std::vector<StringLCP>& Q) {

  int i = 0;
  int j = 0;

  const int m = static_cast<int>(P.size());
  const int f = static_cast<int>(Q.size());

  std::vector<StringLCP> R;
  R.reserve(m + f);

  while (i < m && j < f) {
    const int ki = P[i].lcp;
    const int pj = Q[j].lcp;

    if (ki > pj) {
      R.push_back({P[i].str, ki});
      i++;
    } else if (ki < pj) {
      R.push_back({Q[j].str, pj});
      j++;
    } else {

      auto [is_less, h] =
          LcpCompare(P[i].str, Q[j].str, ki);

      if (is_less) {
        R.push_back({P[i].str, ki});
        ++i;

        if (j < f) {
          const_cast<int&>(Q[j].lcp) = h;
        }
      } else {
        R.push_back({Q[j].str, pj});
        ++j;

        if (i < m) {
          const_cast<int&>(P[i].lcp) = h;
        }
      }
    }
  }

  while (i < m) {
    R.push_back(P[i]);
    ++i;
  }

  while (j < f) {
    R.push_back(Q[j]);
    ++j;
  }

  if (!R.empty()) {
    R[0].lcp = 0;
  }

  return R;
}

std::vector<StringLCP> StringMergeSortImpl(std::vector<std::string>& arr) {
  const int n = static_cast<int>(arr.size());

  if (n == 1) {
    return
        {{arr[0], 0}};
  }

  const int mid = n / 2;
  std::vector<std::string> left(
      arr.begin(),
      arr.begin() + mid
      );

  std::vector<std::string> right(
      arr.begin() + mid,
      arr.end()
      );

  auto P = StringMergeSortImpl(left);
  auto Q = StringMergeSortImpl(right);

  return StringMerge(P, Q);
}

void StringMergeSort(std::vector<std::string>& arr) {
  if (arr.empty()) {
    return;
  }
  const auto res = StringMergeSortImpl(arr);
  for (int i = 0; i < res.size(); ++i) {
    arr[i] = res[i].str;
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