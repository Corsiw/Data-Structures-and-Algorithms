#include <cmath>
#include <cstdint>
#include <iomanip>
#include<iostream>
#include<vector>
#include<string>


class BigInteger {
public:
  using DigitType = uint16_t;
  using DoubleDigitType = uint32_t;
  static constexpr DigitType kBase = 10000;
  static constexpr size_t kBaseDigitsNumber = 4;
  static constexpr size_t kMaxDecimalDigits = 3000001;

  BigInteger() : digits_(0) {
  }

  explicit BigInteger(const std::string& decimal_str) {
    FromDecimalString(decimal_str);
    CheckOverflow();
  }

  BigInteger& operator+=(const BigInteger& other) {
    if (is_negative_ == other.is_negative_) {
      *this = AddAbs(*this, other);
      this->is_negative_ = other.is_negative_;
    } else {
      const int cmp = CompareAbs(*this, other);
      if (cmp == 0) {
        *this = SubtractAbs(*this, other);
        is_negative_ = false;
        return *this;
      }
      if (cmp > 0) {
        const bool is_neg = is_negative_;
        *this = SubtractAbs(*this, other);
        is_negative_ = is_neg;
      } else {
        *this = SubtractAbs(other, *this);
        this->is_negative_ = other.is_negative_;
      }
    }
    return *this;
  }

  BigInteger operator+(const BigInteger& other) const {
    BigInteger result = *this;
    result += other;
    return result;
  }

  BigInteger& operator-=(const BigInteger& other) {
    *this += -other;
    return *this;
  }

  BigInteger operator-(const BigInteger& other) const {
    BigInteger result = *this;
    result -= other;
    return result;
  }

  BigInteger operator-() const {
    BigInteger result = *this;
    if (*this) {
      result.is_negative_ = !is_negative_;
    }
    return result;
  }

  explicit operator bool() const {
    return digits_.size() != 1 || digits_[0] != 0;
  }

  BigInteger& operator*=(const BigInteger& other) {
    if (digits_.size() < 64 || other.digits_.size() < 64) {
      // Для маленьких чисел используем наивный алгоритм
      const size_t a_size = digits_.size();
      const size_t b_size = other.digits_.size();
      std::vector<DigitType> result(a_size + b_size, 0);

      for (size_t i = 0; i < a_size; ++i) {
        uint64_t carry = 0;
        const uint64_t a_digit = digits_[i];
        for (size_t j = 0; j < b_size || carry; ++j) {
          const uint64_t b_digit = j < b_size ? other.digits_[j] : 0;
          const uint64_t cur = result[i + j] + a_digit * b_digit + carry;
          result[i + j] = static_cast<DigitType>(cur % kBase);
          carry = cur / kBase;
        }
      }

      digits_ = std::move(result);
    } else {
      // Алгоритм Карацубы
      *this = KaratsubaMultiply(*this, other);
    }

    is_negative_ ^= other.is_negative_;
    TrimLeadingZeros();
    return *this;
  }

  BigInteger KaratsubaMultiply(const BigInteger& x, const BigInteger& y) {
    // Базовый случай
    if (x.digits_.size() < 32 || y.digits_.size() < 32) {
      return x * y;
    }

    const size_t m = (std::max(x.digits_.size(), y.digits_.size()) + 1) / 2;

    BigInteger x1;
    BigInteger x0;
    if (x.digits_.size() > m) {
      auto mid = x.digits_.begin();
      std::advance(mid, m); // Безопасное перемещение итератора
      x1.digits_.assign(mid, x.digits_.end());
      x0.digits_.assign(x.digits_.begin(), mid);
    } else {
      x0 = x;
    }

    BigInteger y1;
    BigInteger y0;
    if (y.digits_.size() > m) {
      auto mid = y.digits_.begin();
      std::advance(mid, m);
      y1.digits_.assign(mid, y.digits_.end());
      y0.digits_.assign(y.digits_.begin(), mid);
    } else {
      y0 = y;
    }

    const BigInteger z0 = KaratsubaMultiply(x0, y0);
    BigInteger z2 = KaratsubaMultiply(x1, y1);
    BigInteger z1 = KaratsubaMultiply(x0 + x1, y0 + y1) - z2 - z0;

    z2.digits_.insert(z2.digits_.begin(), 2 * m, 0);
    z1.digits_.insert(z1.digits_.begin(), m, 0);

    return z2 + z1 + z0;
  }

  BigInteger operator*(const BigInteger& other) const {
    BigInteger result = *this;
    result *= other;
    return result;
  }

  friend std::ostream& operator<<(std::ostream& os, const BigInteger& num) {
    if (!num) {
      os << '0';
      return os;
    }

    if (num.is_negative_) {
      os << '-';
    }
    auto it = num.digits_.rbegin();
    os << *it++;

    while (it != num.digits_.rend()) {
      os << std::setw(BigInteger::kBaseDigitsNumber)
          << std::setfill('0')
          << *it++;
    }
    return os;
  }

  friend std::istream& operator>>(std::istream& is, BigInteger& num) {
    std::string s;
    is >> s;

    num = BigInteger(s);
    return is;
  }

private:
  std::vector<DigitType> digits_;
  bool is_negative_ = false;

  BigInteger AddAbs(const BigInteger& a, const BigInteger& b) {
    BigInteger result;
    result.digits_.clear();
    result.digits_.reserve(std::max(a.digits_.size(), b.digits_.size()) + 1);

    const size_t n = std::max(a.digits_.size(), b.digits_.size());
    DigitType carry = 0;

    for (size_t i = 0; i < n || carry; ++i) {
      DigitType sum = carry;
      if (i < a.digits_.size()) {
        sum += a.digits_[i];
      }
      if (i < b.digits_.size()) {
        sum += b.digits_[i];
      }

      result.digits_.push_back(sum % kBase);
      carry = sum / kBase;
    }

    result.TrimLeadingZeros();
    return result;
  }

  BigInteger SubtractAbs(const BigInteger& a, const BigInteger& b) {
    BigInteger result;
    result.digits_.clear();
    result.digits_.reserve(a.digits_.size());

    DigitType borrow = 0;
    for (size_t i = 0; i < a.digits_.size(); ++i) {
      int64_t diff = static_cast<int64_t>(a.digits_[i]) -
                     (i < b.digits_.size() ? b.digits_[i] : 0) -
                     borrow;
      if (diff < 0) {
        diff += kBase;
        borrow = 1;
      } else {
        borrow = 0;
      }
      result.digits_.push_back(static_cast<DigitType>(diff));
    }

    result.TrimLeadingZeros();
    return result;
  }

  BigInteger Abs(const BigInteger& value) {
    BigInteger result = value;
    result.is_negative_ = false;
    return result;
  }

  bool AbsLess(const BigInteger& a, const BigInteger& b) {
    return CompareAbs(a, b) < 0;
  }

  void TrimLeadingZeros() {
    while (digits_.size() > 1 && digits_.back() == 0) {
      digits_.pop_back();
    }

    if (digits_.size() == 1 && digits_[0] == 0) {
      is_negative_ = false;
    }
  }

  int CompareAbs(const BigInteger& a, const BigInteger& b) {
    if (a.digits_.size() != b.digits_.size()) {
      return a.digits_.size() > b.digits_.size() ? 1 : -1;
    }
    for (size_t i = a.digits_.size(); i-- > 0;) {
      if (a.digits_[i] != b.digits_[i]) {
        return a.digits_[i] > b.digits_[i] ? 1 : -1;
      }
    }
    return 0;
  }

  void CheckOverflow() const {
    if (!*this) {
      return;
    }

    size_t digits_count = (digits_.size() - 1) * kBaseDigitsNumber;

    const DigitType highest = digits_.back();
    const size_t highest_digits = static_cast<size_t>(std::log10(highest)) + 1;

    digits_count += highest_digits;

    if (digits_count > kMaxDecimalDigits) {
      throw std::runtime_error("Превышение по длине числа.");
    }
  }

  void FromDecimalString(const std::string& str) {
    digits_.clear();
    digits_.reserve(str.size());
    is_negative_ = false;

    size_t i = 0;
    const size_t n = str.size();

    while (i < n && std::isspace(static_cast<unsigned char>(str[i]))) {
      ++i;
    }

    bool is_negative = false;
    if (i < n && (str[i] == '-' || str[i] == '+')) {
      is_negative = (str[i] == '-');
      ++i;
    }

    while (i < n && str[i] == '0') {
      ++i;
    }

    if (i == n || !std::isdigit(static_cast<unsigned char>(str[i]))) {
      digits_.push_back(0);
      is_negative_ = false;
      return;
    }

    std::vector<DigitType> temp;

    for (size_t j = str.size(); j > i;) {
      size_t start = (j >= kBaseDigitsNumber) ? j - kBaseDigitsNumber : i;
      if (start < i) {
        start = i;
      }

      std::string part = str.substr(start, j - start);
      temp.push_back(static_cast<DigitType>(std::stoul(part)));
      j = start;
    }

    digits_ = std::move(temp);

    while (digits_.size() > 1 && digits_.back() == 0) {
      digits_.pop_back();
    }
    is_negative_ = is_negative;
    if (digits_.size() == 1 && digits_.back() == 0) {
      is_negative_ = false;
    }
  }
};


int main() {
  BigInteger a;
  BigInteger b;
  std::cin >> a >> b;
  a *= b;
  std::cout << a;

}