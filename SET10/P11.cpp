#include <iostream>
#include <vector>


bool IterateRows(std::vector<std::vector<int> >& matrix, const int max_mask, const int c, const int iter) {
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

    matrix[iter] = row;

    bool check = true;
    for (int up = 0; up < iter; up++) {
      for (int left = 0; left < matrix[0].size(); left++) {
        for (int right = left + 1; right < matrix[0].size(); right++) {
          if (matrix[up][left] == matrix[up][right] && matrix[up][left] == matrix[iter][left] && matrix[up][right] == matrix[iter][right]) {
            check = false;
            break;
          }
        }
      }
    }

    if (check && IterateRows(matrix, max_mask, c, iter + 1)) {
      return true;
    }
  }

  return false;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  int c = 0;
  std::cin >> n >> m >> c;

  int max_mask = 1;
  for (int i = 0; i < m; i++) {
    max_mask *= c;
  }

  std::vector matrix(n ,std::vector<int>(m));
  IterateRows(matrix, max_mask, c, 0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cout << matrix[i][j] + 1 << " ";
    }
    std::cout << '\n';
  }
}