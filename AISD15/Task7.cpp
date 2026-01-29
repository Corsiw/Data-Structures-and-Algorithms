#include <cassert>
#include <vector>
#include <iostream>
#include <optional>

class HashCuckooTable {
private:
  size_t capacity_;
  std::vector<std::optional<int> > table1_;
  std::vector<std::optional<int> > table2_;

  static constexpr size_t kMaxKicks = 32;

  size_t Hash1(const int key) const {
    return key % capacity_;
  }

  size_t Hash2(const int key) const {
    return (key / capacity_) % capacity_;
  }

  void Rehash() {
    const size_t old_capacity = capacity_;
    capacity_ *= 2;

    const std::vector<std::optional<int> > old_table1 = std::move(table1_);
    const std::vector<std::optional<int> > old_table2 = std::move(table2_);

    table1_.assign(capacity_, std::nullopt);
    table2_.assign(capacity_, std::nullopt);

    for (size_t i = 0; i < old_capacity; ++i) {
      if (old_table1[i].has_value()) {
        Insert(old_table1[i].value());
      }
      if (old_table2[i].has_value()) {
        Insert(old_table2[i].value());
      }
    }
  }

public:
  explicit HashCuckooTable(const size_t capacity)
    : capacity_(capacity),
      table1_(capacity),
      table2_(capacity) {
  }

  bool Find(const int key) const {
    const size_t h1 = Hash1(key);
    const size_t h2 = Hash2(key);

    return (table1_[h1].has_value() && table1_[h1].value() == key) ||
           (table2_[h2].has_value() && table2_[h2].value() == key);
  }

  bool Insert(const int key) {
    if (Find(key)) {
      return true;
    }

    int cur = key;
    bool in_table1 = true;

    for (size_t kick = 0; kick < kMaxKicks; ++kick) {
      if (in_table1) {
        const size_t h = Hash1(cur);
        if (!table1_[h].has_value()) {
          table1_[h] = cur;
          return true;
        }
        std::swap(cur, table1_[h].value());
      } else {
        const size_t h = Hash2(cur);
        if (!table2_[h].has_value()) {
          table2_[h] = cur;
          return true;
        }
        std::swap(cur, table2_[h].value());
      }
      in_table1 = !in_table1;
    }

    Rehash();
    return Insert(cur);
  }

  void Dump() const {
    std::cout << "Table 1:\n";
    for (size_t i = 0; i < capacity_; ++i) {
      std::cout << i << ": " << (table1_[i] ? std::to_string(*table1_[i]) : "-") << '\n';
    }

    std::cout << "\nTable 2:\n";
    for (size_t i = 0; i < capacity_; ++i) {
      std::cout << i << ": " << (table2_[i] ? std::to_string(*table2_[i]) : "-") << '\n';
    }
  }
};

int main() {
  HashCuckooTable table(11);

  // Тут хеш-функции и значения как в пошаговом разборе, который кидали в тг
  // Возникает цикл - нужен Rehash.
  // Он успешно проходит.
  const std::vector values{20, 50, 53, 75, 100, 67, 105, 3, 36, 39, 6};

  for (int i = 0; i < values.size(); i++) {
    assert(table.Insert(values[i]));
  }

  for (int i = 0; i < values.size(); i++) {
    assert(table.Find(values[i]));
  }
  assert(!table.Find(99));

  // Повторная вставка
  assert(table.Insert(10));

  std::cout << "All tests passed!\n\n";
  table.Dump();
}