#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  double l = NAN;
  int n = 0;
  if (!(std::cin >> l)) {
    return 0;
  }
  std::cin >> n;
  std::vector<double> t(n);
  std::vector<double> v(n);
  for (int i = 0; i < n; i++) {
    std::cin >> t[i] >> v[i];
  }

  std::vector<double> pref_t(n + 1, 0.0);
  std::vector<double> pref_d(n + 1, 0.0);
  for (int i = 0; i < n; i++) {
    pref_t[i + 1] = pref_t[i] + t[i];
    pref_d[i + 1] = pref_d[i] + t[i] * v[i];
  }
  const double s = pref_t[n];

  auto pos = [&](const double x) -> double {
    if (x <= 0) {
      return 0.0;
    }
    if (x >= s) {
      return pref_d[n];
    }

    int k = static_cast<int>(std::upper_bound(pref_t.begin(), pref_t.end(), x) - pref_t.begin()) - 1;
    if (k < 0) {
      k = 0;
    }
    const double res = pref_d[k] + (x - pref_t[k]) * v[k];
    return res;
  };

  auto min_diff = [&](const double t0)-> double {
    if (t0 >= s) {
      return 1e100;
    }
    const double left = 0.0;
    const double right = s - t0;
    std::vector<double> cand;
    cand.reserve(2 * (n + 2));
    cand.push_back(left);
    cand.push_back(right);
    for (int i = 0; i <= n; i++) {
      double a = pref_t[i];
      if (a >= left - 1e-12 && a <= right + 1e-12) {
        cand.push_back(std::min(std::max(a, left), right));
      }
      double b = pref_t[i] - t0;
      if (b >= left - 1e-12 && b <= right + 1e-12) {
        cand.push_back(std::min(std::max(b, left), right));
      }
    }

    std::sort(cand.begin(), cand.end());
    std::vector<double> uniq;
    constexpr double kEps = 1e-9;
    for (double x : cand) {
      if (uniq.empty() || fabs(x - uniq.back()) > kEps) {
        uniq.push_back(x);
      }
    }

    double mn = 1e100;
    for (double si : uniq) {
      const double val = pos(si + t0) - pos(si);
      if (val < mn) {
        mn = val;
      }
    }
    return mn;
  };

  double lo = 0.0;
  double hi = s;
  for (int iter = 0; iter < 80; ++iter) {
    const double mid = (lo + hi) / 2.0;
    const double md = min_diff(mid);
    if (md >= l) {
      hi = mid;
    } else {
      lo = mid;
    }
  }
  std::cout.setf(std::ios::fixed);
  std::cout << std::setprecision(3) << hi << "\n";

  return 0;
}