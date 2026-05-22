#include <iostream>
#include <unordered_map>
#include <vector>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int k = 0;
  std::cin >> k;

  if (k < 1) {
    return 0;
  }

  std::unordered_map<int, std::string> dict;
  for (int i = 0; i < 128; i++) {
    dict[i] = std::string(1, static_cast<char>(i));
  }

  std::vector<int> codes(k);
  for (int i = 0; i < k; i++) {
    std::cin >> codes[i];
  }


  int next_code = 128;
  std::string last = dict[codes[0]];
  std::string res;

  res.append(last);
  for (int i = 1; i < k; i++) {
    int next = codes[i];
    std::string s;

    if (!dict.contains(next)) {
      s = last;
      s.push_back(last[0]);
    } else {
      s = dict[next];
    }

    res.append(s);

    dict[next_code++] = last + s[0];
    last = s;
  }

  std::cout << res;
}