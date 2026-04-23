#include <iostream>
#include <string>
#include <vector>


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string input;
  std::cin >> input;

  const size_t n = input.length();

  std::vector<size_t> pf(n);
  for (size_t i = 1; i < n; i++) {
    size_t j = pf[i - 1];

    while (j > 0 && input[j] != input[i]) {
      j = pf[j - 1];
    }

    if (input[j] == input[i]) {
      j++;
    }

    pf[i] = j;
  }

  std::cout << n - pf[n-1] << '\n';
}