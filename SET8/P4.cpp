#include <iostream>
#include <string>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string pattern;
  std::cin >> pattern;

  std::string input;
  std::cin >> input;

  const size_t m = pattern.size();
  const size_t n = input.size();

  if (!n || !m || m > n) {
    std::cout << "0\n";
    return 0;
  }

  std::vector<size_t> shift(26, m);

  for (size_t i = 0; i < m - 1; i++) {
    shift[pattern[i] - 'a'] = m - i - 1;
  }

  std::vector<size_t> res;
  size_t i = m - 1;

  while (i < n) {
    size_t j = 0;
    for (; j < m; j++) {

      if (input[i - j] != pattern[m - 1 - j]) {
        if (j == 0) {
          i += shift[input[i] - 'a'];
        } else {
          i += shift[pattern[m - 1 - j] - 'a'];
        }
        break;
      }
    }

    if (j == m) {
      res.push_back(i - j + 1);
      i += shift[input[i] - 'a'];
    }
  }

  std::cout << res.size() << '\n';
  for (size_t ii = 0; ii < res.size(); ii++) {
    std::cout << res[ii] << "\n";
  }
}