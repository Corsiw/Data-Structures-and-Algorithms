#include <iostream>
#include <queue>
#include <vector>

struct Edge {
  int u, v, w;
};

std::pair<std::vector<int>, std::vector<bool>> BellmanFord(const int n, std::vector<Edge>& edges, const int src, const std::vector<std::vector<int>>& graph) {
  std::vector<int> dist(n, 1e9);
  dist[src] = 0;

  for (int i = 0; i < n - 1; i++) {
    for (auto [u, v, w] : edges) {
      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
      }
    }
  }

  std::vector<int> bad;
  for (auto [u, v, w] : edges) {
    if (dist[u] < 1e9 && dist[v] > dist[u] + w) {
      bad.push_back(v);
    }
  }

  std::queue<int> q;
  std::vector<bool> in_neg(n, false);

  for (int v : bad) {
    q.push(v);
    in_neg[v] = true;
  }

  while (!q.empty()) {
    const int v = q.front(); q.pop();
    for (int to : graph[v]) {
      if (!in_neg[to]) {
        in_neg[to] = true;
        q.push(to);
      }
    }
  }

  return {dist, in_neg};
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  std::vector<std::vector<int> > g;
  g.resize(n);
  std::vector<Edge> edges;
  for (int i = 0; i < m; i++) {
    int u = 0;
    int v = 0;
    int w = 0;
    std::cin >> u >> v >> w;

    g[u].push_back(v);
    edges.push_back({u, v, w});
  }

  auto [res, in_neg] = BellmanFord(n, edges, 0, g);

  for (int i = 1; i < res.size(); i++) {
    if (in_neg[i]) {
      std::cout << "-inf\n";
    } else {
    std::cout << res[i] << "\n";
    }
  }
}