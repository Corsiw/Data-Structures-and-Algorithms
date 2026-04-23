#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


int DamerauLevenshtein(const std::string& s1, const std::string& s2) {
  const size_t n = s1.size();
  const size_t m = s2.size();

  std::vector dp(n + 1, std::vector<int>(m + 1));

  for (int i = 0; i <= n; ++i) {
    dp[i][0] = i;
  }
  for (int j = 0; j <= m; ++j) {
    dp[0][j] = j;
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      const int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

      dp[i][j] = std::min({
          dp[i - 1][j] + 1,
          dp[i][j - 1] + 1,
          dp[i - 1][j - 1] + cost
      });

      if (i > 1 && j > 1 &&
          s1[i - 1] == s2[j - 2] &&
          s1[i - 2] == s2[j - 1]) {
        dp[i][j] = std::min(dp[i][j],
                            dp[i - 2][j - 2] + 1);
      }
    }
  }

  return dp[n][m];
}

int DamerauLevenshteinFull(const std::string& s1, const std::string& s2) {
  const size_t n = s1.size();
  const size_t m = s2.size();

  std::vector dp(n + 2, std::vector<int>(m + 2));

  dp[0][0] = static_cast<int>(n + m);
  for (int i = 0; i <= n; ++i) {
    dp[i + 1][1] = i;
    dp[i + 1][0] = static_cast<int>(n + m);
  }
  for (int j = 0; j <= m; ++j) {
    dp[1][j + 1] = j;
    dp[0][j + 1] = static_cast<int>(n + m);
  }

  std::unordered_map<char, int> last;

  for (int i = 1; i <= n; ++i) {
    int db = 0;
    for (int j = 1; j <= m; ++j) {
      const int i1 = last.contains(s2[j - 1]) ? last[s2[j - 1]] : 0;
      const int j1 = db;

      const int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
      if (cost == 0) {
        db = j;
      }

      dp[i + 1][j + 1] = std::min({
          dp[i][j] + cost,
          dp[i + 1][j] + 1,
          dp[i][j + 1] + 1,
          dp[i1][j1] + (i - i1 - 1) + 1 + (j - j1 - 1)
      });
    }
    last[s1[i - 1]] = i;
  }

  return dp[n + 1][m + 1];
}


int main() {
  std::string s1 = "BKAOC";
  std::string s2 = "KACO";
  std::cout << DamerauLevenshtein(s1, s2) << '\n';
  std::cout << DamerauLevenshteinFull(s1, s2) << '\n';

  s1 = "CA";
  s2 = "ABC";
  std::cout << DamerauLevenshtein(s1, s2) << '\n';
  std::cout << DamerauLevenshteinFull(s1, s2) << '\n';

  return 0;
}