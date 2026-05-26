#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <set>
#include <set>
#include <vector>

struct Interval {
  long double start;
  long double end;
  long double weight;
};

int BinSearchNonIntersectingInterval(std::vector<Interval>& intervals, const int cur) {
  int low = 0;
  int high = cur - 1;

  while (low <= high) {
    if (const int mid = (low + high) / 2; intervals[mid].end <= intervals[cur].start) {
      if (mid + 1 < cur && intervals[mid+1].end <= intervals[cur].start) {
        low = mid + 1;
      } else {
        return mid;
      }
    } else {
      high = mid - 1;
    }
  }

  return -1;
}

bool Compare(const Interval& l, const Interval& r) {
    if (l.end != r.end) {
      return l.end < r.end;
    }

    return l.weight > r.weight;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<Interval> intervals(n);
  for (int i = 0; i < n; i++) {
    std::cin >> intervals[i].start >> intervals[i].end >> intervals[i].weight;
  }

  if (n == 0) {
    std::cout << 0;
    return 0;
  }

  std::ranges::sort(intervals, Compare);

  std::vector<long double> dp(n);
  dp[0] = intervals[0].weight;
  for (int i = 1; i < n; i++) {
    const int candidate = BinSearchNonIntersectingInterval(intervals, i);

    if (candidate != -1) {
      dp[i] = std::max(dp[i-1], intervals[i].weight + dp[candidate]);
    } else {
      dp[i] = std::max(dp[i-1], intervals[i].weight);
    }
  }

  std::cout << std::fixed << std::setprecision(4);
  std::cout << dp[n - 1];
}