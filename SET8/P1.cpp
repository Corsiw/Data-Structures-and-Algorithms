#include <iostream>
#include <string>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string input;
  std::cin >> input;

  std::vector <size_t> pf(input.size());
  for (size_t i = 1; i < input.length(); i++) {
    size_t j = pf[i - 1];

    while (j > 0 && input[j] != input[i]) {
      j = pf[j - 1];
    }

    if (input[j] == input[i]) {
      j++;
    }

    pf[i] = j;
  }

  for (size_t i = 0; i < input.length(); i++) {
    std::cout << pf[i] << " ";
  }
}