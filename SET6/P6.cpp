#include <cstdint>
#include <iostream>
#include <vector>

struct Edge {
  int64_t w;
  int u, v;
};

void FloydWarshall(std::vector<std::vector<int64_t> >& dist) {

  for (int k = 0; k < dist.size(); ++k) {
    for (int i = 0; i < dist.size(); ++i) {
      for (int j = 0; j < dist.size(); ++j) {
        if (dist[i][k] != LONG_LONG_MAX && dist[k][j] != LONG_LONG_MAX) {
          dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  std::vector<std::vector<int64_t> > g;
  g.resize(n);
  for (int i = 0; i < n; i++) {
    g[i] = std::vector<int64_t>(n, LONG_LONG_MAX);
    g[i][i] = 0;
  }

  for (int i = 0; i < m; i++) {
    int u = 0;
    int v = 0;
    int64_t w = 0;
    std::cin >> u >> v >> w;
    g[u][v] = w;
  }

  FloydWarshall(g);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        continue;
      }
      if (g[i][j] != LONG_LONG_MAX) {
        std::cout << i << ' ' << j << ' ' << g[i][j] << '\n';
      } else {
        std::cout << i << ' ' << j << ' ' << -1 << '\n';
      }
    }
  }
}