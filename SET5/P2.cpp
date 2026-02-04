#include "bloom_filter.h"

#include <vector>


BloomFilter::BloomFilter(size_t hashes, size_t bits) : _hashes(hashes), _bits(bits), _filter(_bits, false) {
}

void BloomFilter::add(const std::string& s) {
  _raw_set.insert(s);

  _filter[std::hash<std::string>{}(s) % _bits] = true;

  std::string temp;
  temp.reserve(s.size() + 10);
  temp = s;

  for (size_t i = 1; i < _hashes; i++) {
    temp.resize(s.size());
    temp.append(std::to_string(i));
    _filter[std::hash<std::string>{}(temp) % _bits] = true;
  }
}


bool BloomFilter::verify(const std::string& s) {

  if (!_filter[std::hash<std::string>{}(s) % _bits]) {
    cnt++;
    return false;
  }

  std::string temp;
  temp.reserve(s.size() + 10);
  temp = s;

  for (size_t i = 1; i < _hashes; i++) {
    temp.resize(s.size());
    temp.append(std::to_string(i));
    if (!_filter[std::hash<std::string>{}(temp) % _bits]) {
      cnt++;
      return false;
    }
  }

  if (!_raw_set.contains(s)) {
    false_positive_cnt++;
  }
  cnt++;
  return true;
}


double BloomFilter::getFPRate() const {
  return cnt == 0 ? 0.0 : static_cast<double>(false_positive_cnt) / cnt;
}


size_t BloomFilter::numberOfHashFunctions() const {
  return _hashes;
}


size_t BloomFilter::numberOfBits() const {
  return _bits;
}


inline size_t BloomFilter::_get_hash(int n, const std::string& s) const {
  if (n > 0) {
    return std::hash<std::string>{}(s + std::to_string(n)) % _bits;
  }
  return std::hash<std::string>{}(s) % _bits;
}