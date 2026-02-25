#include <algorithm>
#include<iostream>
#include<vector>

struct Edge {
  int u, v, w;
  int id;
};

class DSU {
  std::vector<int> parent_;
  std::vector<int> rank_;

public:
  explicit DSU(const int n) : parent_(n), rank_(n, 1) {
    for (int i = 0; i < n; i++) {
      parent_[i] = i;
    }
  }

  int Find(const int x) {
    return parent_[x] == x ? x : parent_[x] = Find(parent_[x]);
  }

  void Unite(const int x, const int y) {
    const int p1 = Find(x);
    const int p2 = Find(y);
    if (p1 != p2) {
      if (rank_[p1] < rank_[p2]) {
        parent_[p1] = p2;
      } else if (rank_[p1] > rank_[p2]) {
        parent_[p2] = p1;
      } else {
        parent_[p2] = p1;
        rank_[p1]++;
      }
    }
  }
};

int KruskalsMst(const int n, std::vector<Edge>& edges, std::vector<int>& mst_ids, const int banned_id = -1) {
  std::ranges::sort(edges,
                    [](const Edge a, const Edge b) {
                      return a.w < b.w;
                    });

  DSU dsu(n);
  int weight = 0;
  int count = 0;

  for (auto& [u, v, w, id] : edges) {
    if (id == banned_id) {
      continue;
    }
    if (dsu.Find(u) != dsu.Find(v)) {
      dsu.Unite(u, v);
      weight += w;
      mst_ids.push_back(id);

      if (++count == n - 1) {
        break;
      }
    }
  }

  if (count == n - 1) {
    return weight;
  }
  return INT_MAX;
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  std::vector<Edge> edges;
  for (int i = 0; i < m; i++) {
    int u = 0;
    int v = 0;
    int w = 0;
    std::cin >> u >> v >> w;
    edges.push_back({u - 1, v - 1, w, i});
  }

  std::vector<int> mst_ids;
  const int cost1 = KruskalsMst(n, edges, mst_ids);

  int cost2 = INT_MAX;
  for (const auto id : mst_ids) {
    std::vector<int> dummy;
    int candidate = KruskalsMst(n, edges, dummy, id);
    cost2 = std::min(cost2, candidate);

    if (cost2 == cost1) {
      break;
    }
  }

  std::cout << cost1 << ' ' << cost2 << '\n';
}