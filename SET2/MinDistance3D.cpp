#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
  int64_t x, y, z;
  int index;
};

struct Result {
  Point p1;
  Point p2;
  int64_t distance;
};

inline int64_t Dist(const Point& a, const Point& b) {
  const int64_t dx = a.x - b.x;
  const int64_t dy = a.y - b.y;
  const int64_t dz = a.z - b.z;
  return dx * dx + dy * dy + dz * dz;
}

Result Brute(std::vector<Point>& points, int l, int r) {
  int64_t best = LLONG_MAX;
  std::pair<Point, Point> ans;
  for (int i = l; i <= r; i++) {
    for (int j = i + 1; j <= r; j++) {
      const int64_t d = Dist(points[i], points[j]);
      if (d < best) {
        best = d;
        ans = {points[i], points[j]};
      }
    }
  }
  return {ans.first, ans.second, best};
}

Result Recursion(std::vector<Point>& points, std::vector<Point>& tmp, const int l, const int r) {
  if (r - l <= 3) {
    const auto res = Brute(points, l, r);

    std::sort(points.begin() + l, points.begin() + r + 1,
              [](auto& a, auto& b) {
                return a.y < b.y || (a.y == b.y && a.z < b.z);
              });
    return res;
  }

  const int mid = (l + r) / 2;
  const int64_t mid_x = points[mid].x;

  const auto left = Recursion(points, tmp, l, mid);
  const auto right = Recursion(points, tmp, mid + 1, r);
  auto best = (left.distance < right.distance) ? left : right;
  int64_t d = best.distance;

  merge(points.begin() + l, points.begin() + mid + 1,
        points.begin() + mid + 1, points.begin() + r + 1,
        tmp.begin(), [](auto& a, auto& b) {
          return a.y < b.y || (a.y == b.y && a.z < b.z);
        });
  copy_n(tmp.begin(), (r - l + 1), points.begin() + l);

  int tsz = 0;
  for (int i = l; i <= r; i++) {
    if ((points[i].x - mid_x) * (points[i].x - mid_x) < d) {
      tmp[tsz++] = points[i];
    }
  }

  for (int i = 0; i < tsz; i++) {
    for (int j = i + 1; j < tsz && j <= i + 15; j++) {
      const auto dy = (tmp[j].y - tmp[i].y);
      if (dy * dy >= d) {
        break;
      }
      const auto dz = (tmp[j].z - tmp[i].z);
      if (dz * dz >= d) {
        continue;
      }
      const int64_t d2 = Dist(tmp[i], tmp[j]);
      if (d2 < best.distance) {
        best = {tmp[i], tmp[j], d2};
        d = d2;
      }
    }
  }

  return best;
}


Result ClosestPair(std::vector<Point>& points) {
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

  int n = 0;
  std::cin >> n;

  std::vector<Point> points(n);
  for (int i = 0; i < n; i++) {
    std::cin >> points[i].x >> points[i].y >> points[i].z;
    points[i].index = i + 1;
  }

  const auto d = ClosestPair(points);
  std::cout << std::fixed << std::setprecision(10) << sqrtl(d.distance) << "\n";
  std::cout << d.p1.index << ' ' << d.p2.index;

  return 0;
}