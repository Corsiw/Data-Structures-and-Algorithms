#include <algorithm>
#include <iostream>
#include <vector>

struct Interval {
  int left;
  int right;

  size_t length() const {
    return right - left + 1;
  }

  Interval overlap(const Interval& other) {
    int l = std::max(left, other.left);
    int r = std::min(right, other.right);

    if (r < l) {
      return {0, -1};
    }
    return {l, r};
  }

  bool operator<(const Interval& other) const {
    if (left != other.left) {
      return left < other.left;
    }
    return right < other.right;
  }
};

struct Result {
  size_t length;
  Interval interval;
};

void MergeSort(std::vector<Interval>& arr, int l, int r);

bool better(const Result& a, const Result& b) {
  if (a.length != b.length) {
    return a.length > b.length;
  }
  if (a.length == 0) {
    return false;
  }

  if (a.interval.left != b.interval.left) {
    return a.interval.left < b.interval.left;
  }
  return a.interval.right < b.interval.right;
}

Result solve(std::vector<Interval>& arr, int l, int r) {
  if (l == r) {
    return {0, {0, -1}};
  }

  int mid = l + (r - l) / 2;
  const Result left_res = solve(arr, l, mid);
  const Result right_res = solve(arr, mid + 1, r);

  std::vector<Interval> left_intervals(arr.begin() + l, arr.begin() + mid + 1);
  MergeSort(left_intervals, 0, left_intervals.size() - 1);
  std::vector<Interval> right_intervals(arr.begin() + mid + 1, arr.begin() + r + 1);
  MergeSort(right_intervals, 0, right_intervals.size() - 1);

  Interval mx_overlap = Interval();
  size_t mx_len = 0;

  int i = 0;
  int j = 0;
  while (i < left_intervals.size() && j < right_intervals.size()) {
    auto ov = left_intervals[i].overlap(right_intervals[j]);
    if (ov.length() > mx_len) {
      mx_overlap = ov;
      mx_len = ov.length();
    } else if (mx_len == ov.length() && ov < mx_overlap) {
      mx_overlap = ov;
    }

    if (left_intervals[i].right >= right_intervals[j].right) {
      j++;
    } else {
      i++;
    }
  }

  const Result mid_res = {mx_len, mx_overlap};

  Result best = left_res;
  if (better(right_res, best)) {
    best = right_res;
  }
  if (better(mid_res, best)) {
    best = mid_res;
  }
  return best;
}

void Merge(std::vector<Interval>& arr, int l, int mid, int r) {
  std::vector<Interval> left_intervals(arr.begin() + l, arr.begin() + mid + 1);
  std::vector<Interval> right_intervals(arr.begin() + mid + 1, arr.begin() + r + 1);

  int i = 0;
  int j = 0;
  int cur = l;

  while (i < left_intervals.size() && j < right_intervals.size()) {
    if (left_intervals[i] < right_intervals[j]) {
      arr[cur++] = left_intervals[i++];
    } else {
      arr[cur++] = right_intervals[j++];
    }
  }

  while (i < left_intervals.size()) {
    arr[cur++] = left_intervals[i++];
  }
  while (j < right_intervals.size()) {
    arr[cur++] = right_intervals[j++];
  }
}

void MergeSort(std::vector<Interval>& arr, int left, int right) {
  if (left >= right) {
    return;
  }
  const int mid = left + (right - left) / 2;
  MergeSort(arr, left, mid);
  MergeSort(arr, mid + 1, right);
  Merge(arr, left, mid, right);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t n = 0;
  std::cin >> n;

  std::vector<Interval> intervals(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> intervals[i].left >> intervals[i].right;
  }

  if (n <= 1) {
    std::cout << 0;
    return 0;
  }

  const Result ans = solve(intervals, 0, n - 1);

  std::cout << ans.length << '\n';
  if (ans.length > 0) {
    std::cout << ans.interval.left << ' ' << ans.interval.right;
  }
}