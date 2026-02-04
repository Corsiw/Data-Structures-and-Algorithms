#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>

std::vector<char> char_set{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                           'S', 'T', 'U', 'V', 'W', 'X',
                           'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                           'q', 'r', 's', 't', 'u', 'v',
                           'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
// a и b некоторые числа полученные после s[i] - 'a' + 1
// a + p * b = 0 - хотим
// p = - a / b, по условию p - целое.
// Тогда перебираем все варианты строк и находим все такие a и b, что p - целое.
// На строки большей длины можем обобщить системой линейных уравнений.

int main() {
  std::vector<int> values;
  for (const auto i : char_set) {
    values.push_back(i - 'a' + 1);
  }

  std::vector<std::tuple<int, char, char>> res;

  for (const auto i : values) {
    for (const auto j : values) {
      if (-i % j == 0 && j != 0) {
        res.push_back(std::make_tuple(-i / j, i - 1 + 'a', j - 1 + 'a'));
      }
    }
  }

  std::sort(res.begin(), res.end());
  for (auto i : res) {
    std::cout << std::get<0>(i) << ' ' << std::get<1>(i) << ' ' << std::get<2>(i) << '\n';
  }

  std::cout << std::endl;

  // Проверка
  for (auto i : res) {
    std::cout << (std::get<1>(i) - 'a' + 1) + std::get<0>(i) * (std::get<2>(i) - 'a' + 1) << ' ';
  }
}