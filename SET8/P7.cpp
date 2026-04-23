#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct TrieNode {
  int next[26];
  int link = 0;
  int out_link = -1;
  std::vector<int> out;

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
      const int ind = c - 'a';
      if (trie[state].next[ind] == -1) {
        trie[state].next[ind] = static_cast<int>(trie.size());
        trie.emplace_back();
      }
      state = trie[state].next[ind];
    }
    trie[state].out.push_back(i);
  }

  std::queue<int> q;

  for (int c = 0; c < 26; c++) {
    if (const int u = trie[0].next[c]; u != -1) {
      trie[u].link = 0;
      q.push(u);
    }
  }

  while (!q.empty()) {
    const int v = q.front();
    q.pop();

    for (int c = 0; c < 26; c++) {
      int u = trie[v].next[c];
      if (u == -1) {
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

      if (!trie[link].out.empty()) {
        trie[u].out_link = link;
      } else {
        trie[u].out_link = trie[link].out_link;
      }

      q.push(u);
    }
  }

  return trie;
}

std::vector<std::vector<int> > AhoCorasickSearch(
    const std::vector<TrieNode>& trie,
    const std::string& text,
    const std::vector<std::string>& words) {

  std::vector<std::vector<int> > result(words.size());

  int v = 0;

  for (int i = 0; i < static_cast<int>(text.size()); i++) {
    const int ind = text[i] - 'a';

    while (v != 0 && trie[v].next[ind] == -1) {
      v = trie[v].link;
    }

    if (trie[v].next[ind] != -1) {
      v = trie[v].next[ind];
    }

    int u = v;
    while (u != -1) {
      for (const int id : trie[u].out) {
        int pos = i - static_cast<int>(words[id].size()) + 2;
        result[id].push_back(pos);
      }
      u = trie[u].out_link;
    }
  }

  return result;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string text;
  std::cin >> text;

  int n = 0;
  std::cin >> n;

  std::vector<std::string> words(n);
  for (int i = 0; i < n; i++) {
    std::cin >> words[i];
  }

  const auto trie = PreprocessAutomaton(words);
  auto result = AhoCorasickSearch(trie, text, words);

  for (int i = 0; i < n; i++) {
    std::ranges::sort(result[i].begin(), result[i].end());
    std::cout << result[i].size() << " ";
    for (int j = 0; j < result[i].size(); j++) {
      std::cout << result[i][j] << " ";
    }
    std::cout << "\n";
  }
}