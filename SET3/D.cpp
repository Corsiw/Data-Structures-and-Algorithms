#include <cstdint>
#include <iostream>
#include <ranges>
#include <vector>

class Fraction {
  int numerator_;
  int denominator_;

public:
  explicit Fraction(const int numerator = 0, const int denominator = 1) {
    if (denominator == 0) {
      throw std::invalid_argument("Denominator cannot be zero");
    }

    if (denominator < 0) {
      numerator_ = -numerator;
      denominator_ = -denominator;
    } else {
      numerator_ = numerator;
      denominator_ = denominator;
    }
  }

  bool operator<(const Fraction& other) const {
    const int64_t m1 = this->numerator_ * other.denominator_;
    const int64_t m2 = other.numerator_ * this->denominator_;
    return m1 < m2;
  }

  bool operator>(const Fraction& other) const {
    return other < *this;
  }

  bool operator<=(const Fraction& other) const {
    return !(*this > other);
  }

  bool operator>=(const Fraction& other) const {
    return !(*this < other);
  }

  friend std::ostream& operator<<(std::ostream& os, const Fraction& obj);

  friend std::istream& operator>>(std::istream& is, Fraction& obj);
};

std::ostream& operator<<(std::ostream& os, const Fraction& obj) {
  return os << obj.numerator_ << "/" << obj.denominator_;
}

std::istream& operator>>(std::istream& is, Fraction& obj) {
  int num = 0;
  int den = 1;
  char slash = 0;

  if (is >> num >> slash >> den) {
    if (slash != '/') {
      is.setstate(std::ios::failbit);
      return is;
    }
    if (den == 0) {
      is.setstate(std::ios::failbit);
      return is;
    }
    obj = Fraction(num, den);
  } else {
    is.setstate(std::ios::failbit);
  }

  return is;
}

void Merge(std::vector<Fraction>& arr, int l, int mid, int r) {
  std::vector<Fraction> left_intervals(arr.begin() + l, arr.begin() + mid + 1);
  std::vector<Fraction> right_intervals(arr.begin() + mid + 1, arr.begin() + r + 1);

  int i = 0;
  int j = 0;
  int cur = l;

  while (i < left_intervals.size() && j < right_intervals.size()) {
    if (left_intervals[i] <= right_intervals[j]) {
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

void MergeSort(std::vector<Fraction>& arr, int left, int right) {
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

  int n = 0;
  std::cin >> n;

  std::vector<Fraction> vec(n);
  for (int i = 0; i < n; i++) {
    std::cin >> vec[i];
  }

  MergeSort(vec, 0, static_cast<int>(vec.size()) - 1);
  for (int i = 0; i < n; i++) {
    std::cout << vec[i] << " ";
  }
}