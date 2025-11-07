#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

struct Point {
  int64_t x, y;
};

long double Dist(const Point& a, const Point& b) {
  const long double dx = static_cast<long double>(a.x) - b.x;
  const long double dy = static_cast<long double>(a.y) - b.y;
  return sqrtl(dx * dx + dy * dy);
}

long double Recursion(std::vector<Point>& points, std::vector<Point>& tmp, const int l, const int r) {
  if (r - l <= 3) {
    long double d = 1e18;
    for (int i = l; i <= r; i++) {
      for (int j = i + 1; j <= r; j++) {
        d = std::min(d, Dist(points[i], points[j]));
      }
    }
    std::sort(points.begin() + l, points.begin() + r + 1,
              [](auto& a, auto& b) {
                return a.y < b.y;
              });
    return d;
  }

  const int mid = (l + r) / 2;
  const int64_t mid_x = points[mid].x;

  const auto left = Recursion(points, tmp, l, mid);
  const auto right = Recursion(points, tmp, mid + 1, r);
  long double d = std::min(left, right);

  merge(points.begin() + l, points.begin() + mid + 1,
        points.begin() + mid + 1, points.begin() + r + 1,
        tmp.begin(), [](auto& a, auto& b) {
          return a.y < b.y;
        });
  copy_n(tmp.begin(), (r - l + 1), points.begin() + l);

  int tsz = 0;
  for (int i = l; i <= r; i++) {
    if (fabsl(points[i].x - mid_x) < d) {
      tmp[tsz++] = points[i];
    }
  }

  for (int i = 0; i < tsz; i++) {
    for (int j = i + 1; j < tsz && (tmp[j].y - tmp[i].y) < d; j++) {
      d = std::min(d, Dist(tmp[i], tmp[j]));
    }
  }

  return d;
}


long double ClosestPair(std::vector<Point>& points) {
  std::sort(points.begin(), points.end(),
            [](auto& a, auto& b) {
              return a.x < b.x;
            });
  std::vector<Point> temp(points.size());
  return Recursion(points, temp, 0, points.size() - 1);
}


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::vector<Point> points;
  int64_t x = 0;
  int64_t y = 0;
  while (std::cin >> x >> y) {
    points.push_back({x, y});
  }

  long double d = ClosestPair(points);
  std::cout << static_cast<int64_t>(floorl(d)) << "\n";

  return 0;
}