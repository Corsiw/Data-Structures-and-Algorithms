#include <iostream>
#include <set>
#include <vector>
#include <string>

bool Kuhn(const int v, std::vector<std::vector<int> >& graph, std::vector<int>& match, std::vector<bool>& used) {
  for (const int u : graph[v]) {
    if (!used[u]) {
      used[u] = true;
      if (match[u] == -1 || Kuhn(match[u], graph, match, used)) {
        match[u] = v;
        return true;
      }
    }
  }
  return false;
}

int main() {
  int n = 0;
  std::cin >> n;

  std::vector<std::set<std::string> > drinks(n);

  for (int i = 0; i < n; i++) {
    int k = 0;
    std::cin >> k;

    for (int j = 0; j < k; j++) {
      std::string component;
      std::cin >> component;
      drinks[i].insert(component);
    }
  }

  std::vector<std::vector<int> > graph(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }
      bool subset = true;
      for (const auto& comp : drinks[i]) {
        if (!drinks[j].contains(comp)) {
          subset = false;
          break;
        }
      }
      if (subset) {
        graph[i].push_back(j);
      }
    }
  }

  std::vector<int> match(n, -1);
  int matching = 0;

  for (int v = 0; v < n; v++) {
    if (std::vector used(n, false); Kuhn(v, graph, match, used)) {
      matching++;
    }
  }

  std::cout << n - matching;
}