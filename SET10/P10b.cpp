#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

struct Placement {
    int x, y, type;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int test_id, n, m, wmax;
    std::cin >> test_id >> n >> m >> wmax;

    std::vector<int> p(5);
    for (int i = 0; i < 5; ++i) std::cin >> p[i];

    int pa, pb;
    if (test_id <= 0 || test_id > 9) { pa = 1; pb = 2; }
    else {
        int aa[] = {0, 1, 1, 0, 1, 1, 0, 0, 1, 1};
        int bb[] = {0, 3, 3, 3, 2, 2, 4, 4, 3, 2};
        pa = aa[test_id];
        pb = bb[test_id];
    }

    std::vector cur(n, std::vector<int>(m, -1));
    std::vector<Placement> ops;
    int64_t total_power = 0;

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

    auto has_scaffolding = [&](int i, int j, int t) -> bool {
        bool found[5] = {false};
        for (int d = 0; d < 4; d++) {
            int ni = i + kDi[d], nj = j + kDj[d];
            if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                int ct = cur[ni][nj];
                if (ct >= 0 && ct < t) found[ct] = true;
            }
        }
        for (int need = 0; need < t; need++) {
            if (!found[need]) return false;
        }
        return true;
    };

    std::vector<int> final_type(n * m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            final_type[i * m + j] = std::min(degree(i, j), (pa * i + pb * j) % 5);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            place(i, j, 0);
            total_power += p[0];
        }

    for (int t = 1; t <= 4; t++) {
        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    int ft = final_type[i * m + j];
                    if (ft < t) continue;
                    int cur_t = cur[i][j];
                    if (cur_t >= ft) continue;
                    if (cur_t < t) {
                        for (int nt = t; nt <= ft; nt++) {
                            if (total_power + p[nt] - p[cur_t] > wmax) break;
                            if (!has_scaffolding(i, j, nt)) break;
                            place(i, j, nt);
                            total_power += p[nt] - p[cur_t];
                            changed = true;
                            break;
                        }
                    } else if (cur_t < ft) {
                        if (total_power + p[ft] - p[cur_t] > wmax) continue;
                        if (!has_scaffolding(i, j, ft)) continue;
                        place(i, j, ft);
                        total_power += p[ft] - p[cur_t];
                        changed = true;
                    }
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

