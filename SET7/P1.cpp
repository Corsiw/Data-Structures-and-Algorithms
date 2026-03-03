#include <cstdint>
#include <deque>
#include <iostream>
#include <queue>
#include <vector>

bool Bfs(std::vector<int>& parent, std::vector<bool>& visited, const std::vector<std::vector<int> >& residual,
         const int s, const int t) {
  std::queue<int> q;
  q.push(s);
  visited[s] = true;

  while (!q.empty()) {
    const int u = q.front();
    q.pop();

    for (int i = 0; i < residual.size(); i++) {
      if (!visited[i] && residual[u][i] > 0) {
        visited[i] = true;
        parent[i] = u;

        if (i == t) {
          return true;
        }
        q.push(i);
      }
    }
  }
  return false;
}

int64_t EdmondsKarp(std::vector<std::vector<int> >& graph, const int source, const int sink) {
  int64_t result = 0;
  std::vector<int> parent(graph.size(), -1);
  std::vector<bool> visited(graph.size(), false);

  std::vector<std::vector<int> > residual(graph.size(), std::vector<int>(graph.size()));
  for (int i = 0; i < graph.size(); i++) {
    std::copy(graph[i].begin(), graph[i].end(), residual[i].begin());
  }

  while (Bfs(parent, visited, residual, source, sink)) {
    int path_flow = INT_MAX;
    int cur = sink;
    while (cur != source) {
      const int p = parent[cur];
      path_flow = std::min(path_flow, residual[p][cur]);
      cur = p;
    }

    cur = sink;
    while (cur != source) {
      const int p = parent[cur];
      residual[p][cur] -= path_flow;
      residual[cur][p] += path_flow;
      cur = p;
    }

    result += path_flow;
    std::fill(visited.begin(), visited.end(), false);
    std::fill(parent.begin(), parent.end(), -1);
  }
  return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  std::vector<std::vector<int> > graph(n, std::vector<int>(n));
  for (int i = 0; i < m; i++) {
    int u = 0;
    int v = 0;
    int w = 0;
    std::cin >> u >> v >> w;
    graph[u - 1][v - 1] += w;
  }

  std::cout << EdmondsKarp(graph, 0, n - 1) << '\n';
}