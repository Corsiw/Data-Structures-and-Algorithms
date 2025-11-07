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

inline int64_t Dist(const Point& a, const Point& b) {
  const int64_t dx = a.x - b.x;
  const int64_t dy = a.y - b.y;
  const int64_t dz = a.z - b.z;
  return dx * dx + dy * dy + dz * dz;
}

struct KDNode {
  Point p;
  KDNode *left = nullptr, *right = nullptr;
};

KDNode* Build(std::vector<Point>& points, const int l, const int r, const int depth = 0) {
  if (l > r) {
    return nullptr;
  }

  int axis = depth % 3;
  const int mid = (l + r) / 2;

  auto cmp = [axis](const Point& a, const Point& b) {
    if (axis == 0) {
      return a.x < b.x;
    }
    if (axis == 1) {
      return a.y < b.y;
    }
    return a.z < b.z;
  };
  nth_element(points.begin() + l, points.begin() + mid, points.begin() + r + 1, cmp);
  const auto node = new KDNode{points[mid]};
  node->left = Build(points, l, mid - 1, depth + 1);
  node->right = Build(points, mid + 1, r, depth + 1);
  return node;
}

int64_t best_d = LLONG_MAX;
std::pair<int,int> best_idx;

void Nearest(const KDNode* node, const Point& q, const int depth) {
  if (!node) {
    return;
  }
  const Point& p = node->p;
  if (p.index != q.index) {
    const int64_t dist = Dist(q, p);
    if (dist < best_d) {
      best_d = dist;
      best_idx = {q.index, p.index};
    }
  }

  const int axis = depth % 3;
  int64_t dif = 0;
  if (axis == 0) {
    dif = q.x - p.x;
  } else if (axis == 1) {
    dif = q.y - p.y;
  } else {
    dif = q.z - p.z;
  }

  KDNode* first = (dif < 0 ? node->left : node->right);
  KDNode* second = (dif < 0 ? node->right : node->left);

  if (first) {
    Nearest(first, q, depth + 1);
  }
  if (second && dif * dif < best_d) {
    Nearest(second, q, depth + 1);
  }
}

void ClosestPairKd(const KDNode* root, const std::vector<Point>& points) {
  for (size_t i = 0; i < points.size(); ++i) {
    Nearest(root, points[i], 0);
  }
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

  const KDNode* root = Build(points, 0, n - 1);
  ClosestPairKd(root, points);

  std::cout << std::fixed << std::setprecision(10) << sqrtl(best_d) << "\n";
  std::cout << best_idx.first << ' ' << best_idx.second;
}