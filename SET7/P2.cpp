#include <iostream>
#include <vector>


bool Kuhn(const int v, std::vector<std::vector<int> >& g, std::vector<int>& matching, std::vector<bool>& visited) {
  if (visited[v]) {
    return false;
  }
  visited[v] = true;

  for (const int u : g[v]) {
    if (matching[u] == -1 || Kuhn(matching[u], g, matching, visited)) {
      matching[u] = v;
      return true;
    }
  }
  return false;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  std::vector<std::vector<int> > g(n);
  for (int i = 0; i < n; i++) {
    while (true) {
      int x = 0;
      std::cin >> x;
      if (x == 0) {
        break;
      }
      g[i].push_back(x - 1);
    }
  }

  std::vector<int> mt(m, -1);
  std::vector<bool> visited;

  int matching = 0;
  for (int i = 0; i < n; i++) {
    visited.assign(n, false);
    if (Kuhn(i, g, mt, visited)) {
      matching++;
    }
  }

  std::cout << matching << '\n';
  for (int i = 0; i < m; i++) {
    if (mt[i] != -1) {
      std::cout << mt[i] + 1 << " " << i + 1 << '\n';
    }
  }
}