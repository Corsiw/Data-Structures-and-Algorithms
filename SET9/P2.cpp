#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string s;
  std::getline(std::cin, s);

  std::unordered_map<std::string, int> dict;
  for (int i = 0; i < 128; i++) {
    dict[std::string(1, static_cast<char>(i))] = i;
  }

  std::vector<int> res;

  int next_code = 128;
  std::string prefix;
  for (const char c : s) {
    std::string pc = prefix + c;

    if (dict.contains(pc)) {
      prefix = pc;
    } else {
      res.push_back(dict[prefix]);
      dict[pc] = next_code++;
      prefix = std::string(1, c);
    }
  }

  if (!prefix.empty()) {
    res.push_back(dict[prefix]);
  }

  std::cout << res.size() << '\n' << res[0];
  for (int i = 1; i < res.size(); i++) {
    std::cout << ' ' << res[i];
  }
  std::cout << '\n';
}