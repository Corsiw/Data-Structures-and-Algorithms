#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int n, m, a, b;
std::vector<std::string> graph;
std::vector<std::vector<std::pair<int, int> > > match;
std::vector<std::vector<bool> > used;

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

bool Kuhn(int x, int y) {
  if (used[x][y]) {
    return false;
  }
  used[x][y] = true;

  for (int d = 0; d < 4; d++) {
    const int nx = x + dx[d];
    const int ny = y + dy[d];

    if (nx < 0 || ny < 0 || nx >= n || ny >= m) {
      continue;
    }
    if (graph[nx][ny] != '*') {
      continue;
    }

    if (match[nx][ny].first == -1 ||
        Kuhn(match[nx][ny].first, match[nx][ny].second)) {
      match[nx][ny] = {x, y};
      return true;
    }
  }
  return false;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> n >> m >> a >> b;
  graph.resize(n);
  for (int i = 0; i < n; i++) {
    std::cin >> graph[i];
  }

  int free_cells = 0;
  for (auto& row : graph) {
    for (const char c : row) {
      if (c == '*') {
        free_cells++;
      }
    }
  }

  if (a >= 2 * b) {
    std::cout << free_cells * b;
    return 0;
  }

  match.assign(n, std::vector<std::pair<int, int> >(m, {-1, -1}));

  int64_t dominos = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {

      if (graph[i][j] == '*' && (i + j) % 2 == 0) {

        used.assign(n, std::vector<bool>(m, false));
        if (Kuhn(i, j)) {
          dominos++;
        }
      }
    }
  }

  const int64_t result = dominos * a + 1LL * (free_cells - 2 * dominos) * b;
  std::cout << result;
}