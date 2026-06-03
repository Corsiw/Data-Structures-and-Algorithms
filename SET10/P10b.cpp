#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

struct Placement {
  int x, y, type;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int test_id = 0;
  std::cin >> test_id;

  int n = 0;
  int m = 0;
  int wmax = 0;
  std::cin >> n >> m >> wmax;

  std::vector<int> p(5);
  for (int i = 0; i < 5; i++) {
    std::cin >> p[i];
  }

  std::vector cur(n, std::vector<int>(m, -1));
  std::vector<Placement> ops;

  auto place = [&](int i, int j, int type) {
    ops.push_back({i + 1, j + 1, type});
    cur[i][j] = type;
  };

  constexpr int kDi[] = {-1, 1, 0, 0};
  constexpr int kDj[] = {0, 0, -1, 1};

  auto degree = [&](int i, int j) -> int {
    int d = 0;
    if (i > 0) d++;
    if (i < n - 1) d++;
    if (j > 0) d++;
    if (j < m - 1) d++;
    return d;
  };

  auto has_all_scaffolding = [&](int i, int j, int t) -> bool {
    for (int need = 0; need < t; need++) {
      bool found = false;
      for (int d = 0; d < 4; d++) {
        int ni = i + kDi[d];
        int nj = j + kDj[d];
        if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
          if (cur[ni][nj] == need) {
            found = true;
            break;
          }
        }
      }
      if (!found) return false;
    }
    return true;
  };

  int64_t total_power = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      place(i, j, 0);
      total_power += p[0];
    }
  }

  std::vector target(n, std::vector<int>(m, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int f = (i + 2 * j) % 5;
      target[i][j] = std::min(degree(i, j), f);
    }
  }

  std::vector<std::pair<int, int>> row_major;
  row_major.reserve(n * m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      row_major.push_back({i, j});
    }
  }

  std::vector<std::pair<int, int>> deg_sorted = row_major;
  std::sort(deg_sorted.begin(), deg_sorted.end(), [&](auto& a, auto& b) {
    int da = degree(a.first, a.second);
    int db = degree(b.first, b.second);
    if (da != db) return da < db;
    int fa = (a.first + 2 * a.second) % 5;
    int fb = (b.first + 2 * b.second) % 5;
    return fa > fb;
  });

  auto upgrade_pass = [&](const std::vector<std::pair<int, int>>& order) {
    for (int t = 1; t <= 4; t++) {
      int add = p[t] - p[t - 1];
      if (add > 0 && total_power + add > wmax) continue;

      bool changed = true;
      while (changed) {
        changed = false;
        for (auto& cell : order) {
          int i = cell.first;
          int j = cell.second;
          if (cur[i][j] != t - 1) continue;
          if (target[i][j] < t) continue;
          if (!has_all_scaffolding(i, j, t)) continue;
          if (add > 0 && total_power + add > wmax) continue;

          place(i, j, t);
          total_power += add;
          changed = true;
        }
      }
    }
  };

  auto cascade_pass = [&](const std::vector<std::pair<int, int>>& order) {
    for (int t = 2; t <= 4; t++) {
      int add = p[t] - p[t - 1];
      if (add > 0 && total_power + add > wmax) break;

      bool changed = true;
      while (changed) {
        changed = false;
        for (auto& cell : order) {
          int i = cell.first;
          int j = cell.second;
          if (cur[i][j] != t - 1) continue;
          if (target[i][j] < t - 1) continue;
          if (!has_all_scaffolding(i, j, t)) continue;
          if (add > 0 && total_power + add > wmax) continue;

          place(i, j, t);
          total_power += add;
          changed = true;
        }
      }
    }
  };

  // Thin grids (width=1 or 2) need row-major to preserve type-0 anchors.
  // 2D grids benefit from degree-sorted ordering (edges first).
  bool thin = (n == 1 || m == 1 || n == 2 || m == 2);

  if (thin) {
    upgrade_pass(row_major);
  } else {
    upgrade_pass(deg_sorted);
  }

  cascade_pass(row_major);

  std::vector<std::pair<int, int>> rev_row(row_major.rbegin(), row_major.rend());
  cascade_pass(rev_row);

  cascade_pass(deg_sorted);

  std::vector<std::pair<int, int>> rev_deg(deg_sorted.rbegin(), deg_sorted.rend());
  cascade_pass(rev_deg);

  // Unrestricted final pass: any cell can upgrade regardless of target,
  // subject to scaffolding and budget. This catches cascade effects where
  // a cell's neighbors reached higher types late, enabling further upgrades.
  for (int t = 2; t <= 4; t++) {
    int add = p[t] - p[t - 1];
    if (add > 0 && total_power + add > wmax) break;

    bool changed = true;
    while (changed) {
      changed = false;
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          if (cur[i][j] != t - 1) continue;
          if (!has_all_scaffolding(i, j, t)) continue;
          if (add > 0 && total_power + add > wmax) continue;

          place(i, j, t);
          total_power += add;
          changed = true;
        }
      }
    }
  }

  std::cout << total_power << " " << ops.size() << "\n";
  for (auto& op : ops) {
    std::cout << op.x << " " << op.y << " " << op.type << "\n";
  }

  return 0;
}
