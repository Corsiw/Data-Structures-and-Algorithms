#include <iostream>
#include <string>


void recursion(const std::string& s, size_t& pos, std::string& result) {
  while (pos < s.size() && s[pos] != ']') {
    if (s[pos] >= '0' && s[pos] <= '9') {
      const size_t k = s[pos] - '0';

      pos += 2;

      if (k == 0) {
        size_t depth = 1;
        while (pos < s.size() && depth > 0) {
          if (s[pos] == '[') {
            depth++;
          } else if (s[pos] == ']') {
            depth--;
          }
          pos++;
        }
      } else {
        const size_t start_pos = result.size();
        recursion(s, pos, result);

        const size_t segment_size = result.size() - start_pos;
        for (int j = 1; j < k; ++j) {
          result.append(result, start_pos, segment_size);
        }

        pos++;
      }
    } else {
      result += s[pos];
      pos++;
    }
  }
}

std::string unpack(const std::string& s) {
  if (s == "]") {
    return s;
  }
  std::string result;
  result.reserve(s.size() * 2);
  size_t pos = 0;
  recursion(s, pos, result);
  return result;
}

int main() {
  std::string s;
  std::getline(std::cin, s);
  const auto res = unpack(s);
  std::cout << res;
}