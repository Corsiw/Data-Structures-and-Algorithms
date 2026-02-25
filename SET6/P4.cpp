#include <iostream>
#include <stack>
#include <vector>

void DFS1(const int u, std::vector<std::vector<int> >& g, std::vector<bool>& visited, std::stack<int>& stack) {
  visited[u] = true;
  for (const int v : g[u]) {
    if (!visited[v]) {
      DFS1(v, g, visited, stack);
    }
  }
  stack.push(u);
}

void DFS2(const int u, std::vector<std::vector<int> >& g_reversed, std::vector<bool>& visited, std::vector<int>& scc) {
  visited[u] = true;
  scc.push_back(u);
  for (const int v : g_reversed[u]) {
    if (!visited[v]) {
      DFS2(v, g_reversed, visited, scc);
    }
  }
}

std::vector<std::vector<int> > Kosaraju(const int n, std::vector<std::vector<int> >& g,
                                        std::vector<std::vector<int> >& g_reversed) {
  std::vector<bool> visited(n, false);
  std::stack<int> stack;

  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      DFS1(i, g, visited, stack);
    }
  }

  std::fill(visited.begin(), visited.end(), false);
  std::vector<std::vector<int> > sccs;

  while (!stack.empty()) {
    const int u = stack.top();
    stack.pop();

    if (!visited[u]) {
      std::vector<int> scc;
      DFS2(u, g_reversed, visited, scc);
      sccs.push_back(scc);
    }
  }

  return sccs;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;

  while (std::cin >> n >> m) {
    std::vector<std::vector<int> > g;
    std::vector<std::vector<int> > g_reversed;
    g.resize(2 * n);
    g_reversed.resize(2 * n);

    for (int i = 0; i < m; i++) {
      int i1 = 0;
      int e1 = 0;
      int i2 = 0;
      int e2 = 0;
      std::cin >> i1 >> e1 >> i2 >> e2;

      const int a = 2 * i1 + e1;
      const int b = 2 * i2 + e2;

      g[a ^ 1].push_back(b);
      g[b ^ 1].push_back(a);
      g_reversed[b].push_back(a ^ 1);
      g_reversed[a].push_back(b ^ 1);
    }

    const auto sccs = Kosaraju(2 * n, g, g_reversed);

    std::vector<int> sorted(2 * n);
    for (int i = 0; i < sccs.size(); i++) {
      for (const int v : sccs[i]) {
        sorted[v] = i;
      }
    }

    for (int i = 0; i < n; i++) {
      std::cout << (sorted[2 * i] < sorted[2 * i + 1]);
    }
    std::cout << '\n';

  }
}