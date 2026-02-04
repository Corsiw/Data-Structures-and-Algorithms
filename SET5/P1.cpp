#include <cstdint>
#include <iostream>

// Элемент хеш-таблицы - список объектов с одним хешем
template <class KeyType, class ValueType>
struct Node {
  KeyType key;
  ValueType value;
  Node* next;

  Node(KeyType k, ValueType v) : key(k), value(v), next(nullptr) {
  }
};

// Хеш-таблица
template <class KeyType, class ValueType, class Func = std::hash<KeyType> >
class HashTable {
  Node<KeyType, ValueType>** table_;
  size_t table_capacity_;
  size_t table_size_;
  double load_factor_;
  Func hasher_;

  static constexpr size_t kDefaultCapacity = 100;
  static constexpr double kDefaultLoad = 0.5;

  size_t IndexFor(KeyType& key)  {
    return hasher_(key) % table_capacity_;
  }

  void SafeFree() {
    for (size_t i = 0; i < table_capacity_; ++i) {
      Node<KeyType, ValueType>* cur = table_[i];
      while (cur) {
        Node<KeyType, ValueType>* copy = cur;
        cur = cur->next;
        delete copy;
      }
    }
    delete[] table_;
  }

  void Rehash() {
    const size_t old_capacity = table_capacity_;
    table_capacity_ *= 2;

    auto** new_table =
        new Node<KeyType, ValueType>*[table_capacity_]();

    for (size_t i = 0; i < old_capacity; ++i) {
      Node<KeyType, ValueType>* cur = table_[i];

      while (cur) {
        Node<KeyType, ValueType>* next = cur->next;
        cur->next = nullptr;

        size_t idx = hasher_(cur->key) % table_capacity_;

        // Добавление в конец цепочки в новой таблице
        if (!new_table[idx]) {
          new_table[idx] = cur;
        } else {
          Node<KeyType, ValueType>* tail = new_table[idx];
          while (tail->next) {
            tail = tail->next;
          }
          tail->next = cur;
        }

        cur = next;
      }
    }

    delete[] table_;
    table_ = new_table;
  }

public:
  HashTable()
    : table_capacity_(kDefaultCapacity),
      table_size_(0),
      load_factor_(kDefaultLoad),
      hasher_(Func()) {
    table_ = new Node<KeyType, ValueType>*[table_capacity_]();
  }

  explicit HashTable(Func f)
    : table_capacity_(kDefaultCapacity),
      table_size_(0),
      load_factor_(kDefaultLoad),
      hasher_(f) {
    table_ = new Node<KeyType, ValueType>*[table_capacity_]();
  }

  HashTable(const size_t capacity, const double load, Func h = Func()) : table_capacity_(capacity), table_size_(0),
                                                                         hasher_(h) {
    if (load <= 0.0 || load > 1.0) {
      load_factor_ = kDefaultLoad;
    } else {
      load_factor_ = load;
    }

    if (table_capacity_ < 1) {
      table_capacity_ = kDefaultCapacity;
    }

    table_ = new Node<KeyType, ValueType>*[table_capacity_]();
  }

  ~HashTable() {
    SafeFree();
  }

  int size() const {
    return table_size_; // static cast?
  }

  int capacity() const {
    return table_capacity_;
  }

  void insert(KeyType key, ValueType value) {
    size_t idx = IndexFor(key);
    Node<KeyType, ValueType>* cur = table_[idx];
    Node<KeyType, ValueType>* prev = nullptr;

    while (cur) {
      if (cur->key == key) {
        cur->value = value;
        return;
      }
      prev = cur;
      cur = cur->next;
    }

    auto* node =
        new Node<KeyType, ValueType>(key, value);

    if (prev) {
      prev->next = node;
    } else {
      table_[idx] = node;
    }

    ++table_size_;

    if (static_cast<double>(table_size_) / table_capacity_ > load_factor_) {
      Rehash();
    }
  }

  void erase(KeyType key) {
    size_t index = IndexFor(key);
    Node<KeyType, ValueType>* cur = table_[index];
    Node<KeyType, ValueType>* prev = nullptr;

    while (cur) {
      if (cur->key == key) {
        if (prev) {
          prev->next = cur->next;
        } else {
          table_[index] = cur->next;
        }

        delete cur;
        --table_size_;
        return;
      }
      prev = cur;
      cur = cur->next;
    }
  }

  ValueType* find(KeyType key) {
    size_t index = IndexFor(key);
    Node<KeyType, ValueType>* cur = table_[index];

    while (cur) {
      if (cur->key == key) {
        return &cur->value;
      }
      cur = cur->next;
    }
    return nullptr;
  }

  Node<KeyType, ValueType>& operator[](uint64_t hash) {
    if (hash >= table_capacity_) {
      throw std::out_of_range("hash out of range");
    }

    return table_[hash] ? *table_[hash] : throw std::runtime_error("bucket is empty");
  }

  Node<KeyType, ValueType> at(uint64_t hash) {
    if (hash >= table_capacity_) {
      throw std::out_of_range("hash out of range");
    }

    return table_[hash] ? *table_[hash] : throw std::runtime_error("bucket is empty");
  }
};