#include <iostream>
#include <vector>

std::vector values{30, 31, 15, 79, 55, 42, 99, 60, 80, 23, 43, 53};
constexpr int M = 15;
std::vector<int> table(M);

int prob_counter = 0;

size_t hash1(const int key) {
  return key % M;
}

size_t hash2(const int key) {
  return key / 10;
}

int main() {
  std:: cout << "Double hashing:\n";

  for (const auto value : values) {
    const size_t initial = hash1(value);
    size_t bin = initial;

    for (int i = 0; i < M; ++i) {
      bin = (initial + i * hash2(value)) % M;
      prob_counter++;
      if (!table[bin]) {
        break;
      }
    }

    table[bin] = value;
  }

  for (const auto value : table) {
    std::cout << value << ' ';
  }
  std::cout << '\n' << static_cast<double>(prob_counter) / values.size() << '\n';
  // Три кластера на 7, 3 и 2 элементов.
  std::cout << static_cast<double>(values.size()) / table.size() << '\n';

  // Линейное пробирование
  std:: cout << "Linear probing:\n";
  table = std::vector<int>(M);
  prob_counter = 0;

  for (const auto value : values) {
    const size_t initial = hash1(value);
    size_t bin = initial;

    for (int i = 0; i < M; ++i) {
      bin = (initial + i) % M;
      prob_counter++;
      if (!table[bin]) {
        break;
      }
    }

    table[bin] = value;
  }

  for (const auto value : table) {
    std::cout << value << ' ';
  }
  std::cout << '\n' << static_cast<double>(prob_counter) / values.size() << '\n';
  // Два кластера по 6 элементов
  std::cout << static_cast<double>(values.size()) / table.size();
}