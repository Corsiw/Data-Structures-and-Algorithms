#include <iostream>
#include <random>
#include <cmath>

constexpr uint64_t kSeed = 52426967;
constexpr uint64_t kNumberOfPoints = 100000;

class Circle {
public:
  double x_;
  double y_;
  double r_;

  Circle() : x_(0), y_(0), r_(0) {
  }

  Circle(const double x, const double y, const double r) : x_(x), y_(y), r_(r) {
  }

  bool IsInside(const double x, const double y) const {
    return (x_ - x) * (x_ - x) + (y_ - y) * (y_ - y) <= r_ * r_;
  }
};

// min_x, max_x, min_y, max_y
std::pair<std::pair<double, double>, std::pair<double, double> > GetDistributionRangeForBoundingBox(
    const Circle& circle1, const Circle& circle2, const Circle& circle3) {
  double min_x = NAN;
  double max_x = NAN;
  double min_y = NAN;
  double max_y = NAN;

  min_x = std::min(std::min(circle1.x_ - circle1.r_, circle2.x_ - circle2.r_), circle3.x_ - circle3.r_);
  max_x = std::max(std::max(circle1.x_ + circle1.r_, circle2.x_ + circle2.r_), circle3.x_ + circle3.r_);
  min_y = std::min(std::min(circle1.y_ - circle1.r_, circle2.y_ - circle2.r_), circle3.y_ - circle3.r_);
  max_y = std::max(std::max(circle1.y_ + circle1.r_, circle2.y_ + circle2.r_), circle3.y_ + circle3.r_);

  return std::make_pair(std::make_pair(min_x, max_x), std::make_pair(min_y, max_y));
}

std::pair<std::pair<double, double>, std::pair<double, double> > GetDistributionRangeForIntersectionBoundingBox(
    const Circle& circle1, const Circle& circle2, const Circle& circle3) {
  double min_x = NAN;
  double max_x = NAN;
  double min_y = NAN;
  double max_y = NAN;

  min_x = std::max(std::max(circle1.x_ - circle1.r_, circle2.x_ - circle2.r_), circle3.x_ - circle3.r_);
  max_x = std::min(std::min(circle1.x_ + circle1.r_, circle2.x_ + circle2.r_), circle3.x_ + circle3.r_);
  min_y = std::max(std::max(circle1.y_ - circle1.r_, circle2.y_ - circle2.r_), circle3.y_ - circle3.r_);
  max_y = std::min(std::min(circle1.y_ + circle1.r_, circle2.y_ + circle2.r_), circle3.y_ + circle3.r_);

  if (min_x >= max_x || min_y >= max_y) {
    return {{0.0, 0.0}, {0.0, 0.0}};
  }

  return std::make_pair(std::make_pair(min_x, max_x), std::make_pair(min_y, max_y));
}


double CalculateIntersectionArea(const Circle& c1, const Circle& c2, const Circle& c3) {
  std::mt19937 rng(kSeed);
  auto [fst, snd] = GetDistributionRangeForIntersectionBoundingBox(c1, c2, c3);

  if (fst == std::pair(0.0, 0.0) || snd == std::pair<double, double>(0.0, 0.0)) {
    return 0.0;
  }
  std::uniform_real_distribution<double> x_des(fst.first, fst.second);
  std::uniform_real_distribution<double> y_des(snd.first, snd.second);

  size_t points = 0;
  for (int i = 0; i < kNumberOfPoints; ++i) {
    const double x = x_des(rng);
    const double y = y_des(rng);

    if (c1.IsInside(x, y) && c2.IsInside(x, y) && c3.IsInside(x, y)) {
      points++;
    }
  }
  return static_cast<double>(points) / kNumberOfPoints * (fst.second - fst.first) * (snd.second - snd.first);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  double x = 0.0;
  double y = 0.0;
  double r = 0.0;

  std::cin >> x >> y >> r;
  const Circle circle1(x, y, r);

  std::cin >> x >> y >> r;
  const Circle circle2(x, y, r);

  std::cin >> x >> y >> r;
  const Circle circle3(x, y, r);

  std::cout << CalculateIntersectionArea(circle1, circle2, circle3);
}