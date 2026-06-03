#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Placement {
    int x, y, type;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_id;
    cin >> test_id;

    int n, m, wmax;
    cin >> n >> m >> wmax;

    vector<int> p(5);
    for (int i = 0; i < 5; i++) cin >> p[i];

    vector<vector<int>> cur(n, vector<int>(m, -1));
    vector<Placement> ops;

    auto place = [&](int i, int j, int type) {
        ops.push_back({i + 1, j + 1, type});
        cur[i][j] = type;
    };

    const int di[] = {-1, 1, 0, 0};
    const int dj[] = {0, 0, -1, 1};

    auto degree = [&](int i, int j) -> int {
        int d = 0;
        if (i > 0) d++;
        if (i < n - 1) d++;
        if (j > 0) d++;
        if (j < m - 1) d++;
        return d;
    };

    auto has_neighbor_ge = [&](int i, int j, int t) -> bool {
        for (int d = 0; d < 4; d++) {
            int ni = i + di[d], nj = j + dj[d];
            if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                if (cur[ni][nj] >= t) return true;
            }
        }
        return false;
    };

    long long total_power = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            place(i, j, 0);
            total_power += p[0];
        }
    }

    for (int t = 1; t <= 4; t++) {
        int upgrade_cost = p[t] - p[t - 1];

        bool changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (cur[i][j] != t - 1) continue;
                    if (degree(i, j) < t) continue;
                    if (!has_neighbor_ge(i, j, t - 1)) continue;
                    if (total_power + upgrade_cost > wmax) continue;

                    place(i, j, t);
                    total_power += upgrade_cost;
                    changed = true;
                }
            }
        }
    }

    cout << total_power << " " << ops.size() << "\n";
    for (auto& op : ops) {
        cout << op.x << " " << op.y << " " << op.type << "\n";
    }

    return 0;
}
