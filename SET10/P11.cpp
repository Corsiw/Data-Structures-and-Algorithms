#include <iostream>
#include <vector>

bool used[10][10][3];

bool DFS(std::vector<std::vector<int> >& matrix, const int max_mask, const int c, const int iter) {
  if (iter >= matrix.size()) {
    return true;
  }

  std::vector<int> row(matrix[0].size());
  for (int cur = 0; cur < max_mask; cur++) {
    int mask = cur;
    for (int i = 0; i < matrix[0].size(); i++) {
      row[i] = mask % c;
      mask /= c;
    }

    bool check = true;

    for (int c1 = 0; c1 < matrix[0].size() && check; c1++) {
      for (int c2 = c1 + 1; c2 < matrix[0].size(); c2++) {
        if (row[c1] == row[c2]) {
          int color = row[c1];

          if (used[c1][c2][color]) {
            check = false;
            break;
          }
        }
      }
    }

    if (!check) {
      continue;
    }

    std::vector<std::tuple<int, int, int>> added;

    for (int c1 = 0; c1 < matrix[0].size(); ++c1) {
      for (int c2 = c1 + 1; c2 < matrix[0].size(); ++c2) {
        if (row[c1] == row[c2]) {
          const int color = row[c1];

          if (!used[c1][c2][color]) {
            used[c1][c2][color] = true;
            added.emplace_back(c1, c2, color);
          }
        }
      }
    }

    matrix[iter] = row;

    if (DFS(matrix, max_mask, c, iter + 1)) {
      return true;
    }

    for (auto [c1, c2, color] : added) {
      used[c1][c2][color] = false;
    }
  }

  return false;
}

void GetOptimalC3(std::vector<std::vector<int> >& matrix, const int n, const int m) {
  const int sol[10][10] = {
    {1,2,1,2,0,1,0,1,2,0},
    {2,1,0,2,2,0,0,1,0,1},
    {2,2,1,1,0,2,1,0,0,2},
    {2,0,0,0,0,1,1,2,2,1},
    {0,0,2,2,1,2,1,1,0,0},
    {2,0,2,1,1,0,2,0,1,1},
    {0,1,1,2,0,0,2,2,1,2},
    {0,1,2,0,1,1,0,0,2,2},
    {1,1,0,1,2,2,2,0,2,0},
    {1,2,2,0,2,0,1,2,1,0}
  };

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matrix[i][j] = sol[i][j];
    }
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  int c = 0;
  std::cin >> n >> m >> c;

  std::vector matrix(n, std::vector<int>(m));

  if (c == 3) {
    GetOptimalC3(matrix, n, m);
  } else {
    int max_mask = 1;
    for (int i = 0; i < m; i++) {
      max_mask *= c;
    }

    DFS(matrix, max_mask, c, 0);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cout << matrix[i][j] + 1 << " ";
    }
    std::cout << '\n';
  }
}