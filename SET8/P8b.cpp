#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct TrieNode {
  int next[2];
  int link = -1;
  bool terminal = false;
  bool bad = false;

  TrieNode() {
    std::memset(next, -1, sizeof(next));
  }
};

std::vector<TrieNode> PreprocessAutomaton(const std::vector<std::string>& words) {
  std::vector<TrieNode> trie(1);
  trie.reserve(1e6 + 5);

  for (int i = 0; i < static_cast<int>(words.size()); i++) {
    int state = 0;
    for (const char c : words[i]) {
      const int ind = c - '0';
      if (trie[state].next[ind] == -1) {
        trie[state].next[ind] = static_cast<int>(trie.size());
        trie.emplace_back();
      }
      state = trie[state].next[ind];
    }
    trie[state].terminal = true;
  }

  std::queue<int> q;

  for (int c = 0; c < 2; c++) {
    if (const int u = trie[0].next[c]; u != -1) {
      trie[u].link = 0;
      q.push(u);
    } else {
      trie[0].next[c] = 0;
    }
  }

  while (!q.empty()) {
    const int v = q.front();
    q.pop();

    trie[v].bad = trie[v].terminal || trie[trie[v].link].bad;

    for (int c = 0; c < 2; c++) {
      int u = trie[v].next[c];
      if (u == -1) {
        trie[v].next[c] = trie[trie[v].link].next[c];
        continue;
      }

      int link = trie[v].link;

      while (link != 0 && trie[link].next[c] == -1) {
        link = trie[link].link;
      }

      if (trie[link].next[c] != -1) {
        link = trie[link].next[c];
      }

      trie[u].link = link;

      q.push(u);
    }
  }

  return trie;
}

std::vector<int> color;

bool Dfs(const int v, const std::vector<TrieNode>& trie) {
  if (trie[v].bad) {
    return false;
  }

  if (color[v] == 1) {
    return true;
  }
  if (color[v] == 2) {
    return false;
  }

  color[v] = 1;

  for (int c = 0; c < 2; c++) {
    const int u = trie[v].next[c];
    if (Dfs(u, trie)) {
      return true;
    }
  }

  color[v] = 2;
  return false;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<std::string> words(n);
  for (int i = 0; i < n; i++) {
    std::cin >> words[i];
  }

  const auto trie = PreprocessAutomaton(words);

  color.assign(trie.size(), 0);
  if (Dfs(0, trie)) {
    std::cout << "TAK\n";
  } else {
    std::cout << "NIE\n";
  }
}