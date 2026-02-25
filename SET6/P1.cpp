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

std::vector<std::vector<int> > Kosaraju(int n, std::vector<std::vector<int> >& g,
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
  int n = 0;
  int m = 0;
  std::cin >> n;
  std::cin >> m;

  std::vector<std::vector<int> > g;
  std::vector<std::vector<int> > g_reversed;
  g.resize(n);
  g_reversed.resize(n);

  for (int i = 0; i < m; i++) {
    int start = 0;
    int end = 0;
    std::cin >> start >> end;
    g[start - 1].push_back(end - 1);
    g_reversed[end - 1].push_back(start - 1);
  }

  const auto sccs = Kosaraju(n, g, g_reversed);

  std::vector<int> res(n);
  for (int i = 0; i < sccs.size(); i++) {
    for (const int v : sccs[i]) {
      res[v] = i + 1;
    }
  }

  std::cout << sccs.size() << '\n';
  for (const int v : res) {
    std::cout << v << ' ';
  }
}