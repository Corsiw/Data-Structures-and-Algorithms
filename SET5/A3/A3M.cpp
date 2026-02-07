#include <cmath>
#include <cstdint>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_set>


// Первый этап с проверкой хеш функции
class RandomStreamGen {
public:
  explicit RandomStreamGen(const size_t stream_size, const uint32_t seed = std::random_device{}())
    : stream_size_(stream_size), rng_(seed) {
    stream_.reserve(stream_size_);
    for (size_t i = 0; i < stream_size_; ++i) {
      stream_.push_back(GenerateRandomString());
    }
  }

  const std::vector<std::string>& GetStream() const {
    return stream_;
  }

  std::vector<std::string> GetPrefix(const double fraction) const {
    const auto count = static_cast<size_t>(stream_size_ * fraction);
    return GetPrefixByCount(count);
  }

  std::vector<std::string> GetPrefixByCount(size_t count) const {
    count = std::min(count, stream_size_);
    return {stream_.begin(), stream_.begin() + count};
  }

private:
  std::string GenerateRandomString() {
    static const std::string kAlphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-";

    std::uniform_int_distribution<int> len_dist(1, 30);
    std::uniform_int_distribution<int> char_dist(0, kAlphabet.size() - 1);

    const int len = len_dist(rng_);
    std::string s;
    s.reserve(len);

    for (int i = 0; i < len; ++i) {
      s.push_back(kAlphabet[char_dist(rng_)]);
    }
    return s;
  }

  size_t stream_size_;
  std::mt19937 rng_;
  std::vector<std::string> stream_;
};

class HashFuncGen {
public:

  explicit HashFuncGen(const uint64_t seed = 0) : seed_(seed) {}

  uint32_t operator()(const std::string& key) const {
    const auto* data = reinterpret_cast<const uint8_t*>(key.data());
    const int len = key.size();

    constexpr uint32_t kC1 = 0xcc9e2d51;
    constexpr uint32_t kC2 = 0x1b873593;

    uint32_t h = seed_;

    const int n_blocks = len / 4;
    const auto* blocks = reinterpret_cast<const uint32_t*>(data);

    for (int i = 0; i < n_blocks; i++) {
      uint32_t k = blocks[i];
      k *= kC1;
      k = (k << 15) | (k >> 17);
      k *= kC2;

      h ^= k;
      h = (h << 13) | (h >> 19);
      h = h * 5 + 0xe6546b64;
    }

    const uint8_t* tail = data + n_blocks * 4;
    uint32_t k1 = 0;

    switch (len & 3) {
      case 3:
        k1 ^= tail[2] << 16;
      case 2:
        k1 ^= tail[1] << 8;
      case 1:
        k1 ^= tail[0];
        k1 *= kC1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= kC2;
        h ^= k1;
      default: ;
    }

    // finalization
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
  }

  uint64_t seed_;
};

int LeadingZeros(const uint32_t x) {
  return __builtin_clz(x);
}

constexpr size_t kStreamSize = 1024 * 10000;

void TestLeadingZeros(const size_t stream_size, const size_t repeats, const std::string& output_file) {
  std::vector avg_counts(33, 0.0); // 0..32 ведущих нулей

  HashFuncGen hash{};
  const RandomStreamGen stream_gen(kStreamSize);
  auto stream = stream_gen.GetStream();

  for (size_t r = 0; r < repeats; ++r) {
    std::vector<size_t> counts(33, 0);
    for (size_t i = 0; i < stream_size; ++i) {
      const uint32_t h = hash(stream[i]);
      const int lz = LeadingZeros(h);
      counts[lz]++;
    }
    // усредняем
    for (size_t i = 0; i <= 32; ++i) {
      avg_counts[i] += counts[i] / static_cast<double>(repeats);
    }
  }

  // запись в CSV
  std::ofstream out(output_file);
  if (!out.is_open()) {
    std::cerr << "Cannot open file: " << output_file << "\n";
    return;
  }

  out << "LeadingZeros,AverageCount\n";
  for (size_t i = 0; i <= 32; ++i) {
    out << i << "," << std::fixed << std::setprecision(6) << avg_counts[i] << "\n";
  }

  std::cout << "Finished. Results saved to " << output_file << "\n";
}

// Второй этап
class PackedRegisters {
public:
  static constexpr uint8_t kBitsPerRegister = 5;
  static constexpr uint8_t kMaxValue = (1u << kBitsPerRegister) - 1;

  explicit PackedRegisters(size_t m)
      : m_(m),
        data_((m * kBitsPerRegister + 63) / 64, 0) {}

  uint8_t get(const size_t idx) const {
    const size_t bitPos = idx * kBitsPerRegister;
    const size_t word = bitPos / 64;
    const size_t offset = bitPos % 64;

    uint64_t value = data_[word] >> offset;
    if (offset + kBitsPerRegister > 64) {
      value |= data_[word + 1] << (64 - offset);
    }
    return static_cast<uint8_t>(value & kMaxValue);
  }

  void set_max(const size_t idx, uint8_t val) {
    val = std::min(val, kMaxValue);

    const uint8_t current = get(idx);
    if (val <= current) return;

    const size_t bitPos = idx * kBitsPerRegister;
    const size_t word = bitPos / 64;
    const size_t offset = bitPos % 64;

    const uint64_t mask = static_cast<uint64_t>(kMaxValue) << offset;
    data_[word] = (data_[word] & ~mask) | (static_cast<uint64_t>(val) << offset);

    if (offset + kBitsPerRegister > 64) {
      const size_t spillBits = offset + kBitsPerRegister - 64;
      const uint64_t spillMask = (1ULL << spillBits) - 1;
      data_[word + 1] =
          (data_[word + 1] & ~spillMask) |
          (static_cast<uint64_t>(val) >> (kBitsPerRegister - spillBits));
    }
  }

  size_t size() const { return m_; }

private:
  size_t m_;
  std::vector<uint64_t> data_;
};

class HyperLogLog {
public:
  HyperLogLog(const uint8_t b, const HashFuncGen hash) : b_(b), m_(1 << b), registers_(m_), hash_(hash) {
  }

  void Add(const std::string& s) {
    const uint32_t hash = hash_(s);
    const uint32_t idx = hash >> (32 - b_); // первые B бит для выбора регистра
    const uint32_t w = hash << b_; // оставшиеся 32-B бит
    const uint8_t zeros = LeadingZeros(w) + 1; // количество лидирующих нулей
    registers_.set_max(idx, zeros);
  }

  double Estimate() const {
    double alpha = NAN;
    if (m_ == 16) {
      alpha = 0.673;
    } else if (m_ == 32) {
      alpha = 0.697;
    } else if (m_ == 64) {
      alpha = 0.709;
    } else {
      alpha = 0.7213 / (1 + 1.079 / m_);
    }

    double z = 0.0;
    size_t V = 0;

    for (size_t i = 0; i < m_; ++i) {
      uint8_t r = registers_.get(i);
      z += 1.0 / (1ULL << r);
      if (r == 0) ++V;
    }

    double e = alpha * m_ * m_ / z;

    if (e <= 2.5 * m_ && V > 0) {
      e = m_ * std::log(static_cast<double>(m_) / V);
    }
    return e;
  }

private:
  uint8_t b_;
  size_t m_;
  PackedRegisters registers_;
  HashFuncGen hash_;

  static int LeadingZeros(const uint32_t x) {
    const auto res = __builtin_clz(x);
    return res;
  }
};

class MultiHyperLogLog {
public:
  MultiHyperLogLog(
      uint8_t b,
      const size_t k,
      const uint64_t base_seed = 0xDEADBEEF)
  {
    hlls_.reserve(k);
    for (size_t i = 0; i < k; ++i) {
      HashFuncGen hash(base_seed + i * 0x9e3779b97f4a7c15ULL);
      hlls_.emplace_back(b, hash);
    }
  }

  void Add(const std::string& s) {
    for (auto& hll : hlls_) {
      hll.Add(s);
    }
  }

  double Estimate() const {
    double sum = 0.0;
    for (const auto& hll : hlls_) {
      sum += hll.Estimate();
    }
    return sum / hlls_.size();
  }

private:
  std::vector<HyperLogLog> hlls_;
};


size_t ExactF0(const std::vector<std::string>& stream) {
  std::unordered_set<std::string> s(stream.begin(), stream.end());
  return s.size();
}

int main() {
  HashFuncGen hash;
  // Проверка на равномерность функции хеша
  // constexpr size_t kRepeats = 5;
  // const std::string output_file = "leading_zeros.csv";
  // TestLeadingZeros(kStreamSize, kRepeats, output_file);

  constexpr size_t kNumStreams = 5;
  constexpr size_t kStreamSize = 100000;
  constexpr size_t kNumFractions = 5;
  constexpr size_t kK = 4;
  uint8_t b = 10;

  std::vector<std::vector<double> > estimates(kNumFractions);
  std::vector<std::vector<size_t> > exacts(kNumFractions);

  std::ofstream out_step_csv("graph1_data.csv");
  out_step_csv << "stream_num,frac,Exact,F_HLL\n";

  for (size_t n = 0; n < kNumStreams; ++n) {
    RandomStreamGen gen(kStreamSize);

    for (size_t k = 0; k < kNumFractions; ++k) {
      double frac = 0.2 * (k + 1);
      std::vector<std::string> stream = gen.GetPrefix(frac);

      MultiHyperLogLog hll(b, kK);
      for (auto& s : stream) {
        hll.Add(s);
      }

      double est = hll.Estimate();
      size_t exact = ExactF0(stream);

      estimates[k].push_back(est);
      exacts[k].push_back(exact);

      // Запись в CSV для графика 1
      out_step_csv << n + 1 << "," << frac << "," << exact << "," << est << "\n";

      std::cout << "Stream #" << n + 1 << ", frac = " << frac
          << ", Exact F0 = " << exact
          << ", HLL estimate = " << est
          << ", d% = " << (est - exact) / exact * 100<< "\n";
    }
  }
  out_step_csv.close();

  std::ofstream out_stat_csv("graph2_data.csv");
  out_stat_csv << "frac,Mean_HLL,Std_HLL,Mean_Exact,Std_Exact\n";

  for (size_t k = 0; k < kNumFractions; ++k) {
    double mean_hll = std::accumulate(estimates[k].begin(), estimates[k].end(), 0.0) / estimates[k].size();
    double var_hll = 0.0;
    for (auto val : estimates[k]) {
      var_hll += (val - mean_hll) * (val - mean_hll);
    }
    var_hll /= (estimates[k].size() - 1); // несмещённая оценка
    double std_hll = std::sqrt(var_hll);

    double mean_exact = std::accumulate(exacts[k].begin(), exacts[k].end(), 0.0) / exacts[k].size();
    double var_exact = 0.0;
    for (auto val : exacts[k]) {
      var_exact += (val - mean_exact) * (val - mean_exact);
    }
    var_exact /= (exacts[k].size() - 1);
    double std_exact = std::sqrt(var_exact);

    out_stat_csv << 0.2 * (k + 1) << "," << mean_hll << "," << std_hll << ","
        << mean_exact << "," << std_exact << "\n";
  }
  out_stat_csv.close();
}