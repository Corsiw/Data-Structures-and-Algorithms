#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

struct Edge {
  int to, cap, flow;
};

class Dinic {
  int n_;
  std::vector<std::vector<int> > adj_;
  std::vector<Edge> edges_;
  std::vector<int> level_;
  std::vector<int> ptr_;
  std::queue<int> q_;

  bool Bfs(const int s, const int t) {
    fill(level_.begin(), level_.end(), -1);
    level_[s] = 0;
    q_ = std::queue<int>();
    q_.push(s);

    while (!q_.empty()) {
      const int v = q_.front();
      q_.pop();

      for (const int id : adj_[v]) {
        if (edges_[id].cap - edges_[id].flow > 0 &&
            level_[edges_[id].to] == -1) {
          level_[edges_[id].to] = level_[v] + 1;
          q_.push(edges_[id].to);
        }
      }
    }
    return level_[t] != -1;
  }

  int Dfs(const int v, const int t, const int pushed) {
    if (!pushed) {
      return 0;
    }
    if (v == t) {
      return pushed;
    }

    for (int& cid = ptr_[v]; cid < adj_[v].size(); cid++) {
      const int id = adj_[v][cid];
      const int to = edges_[id].to;

      if (level_[to] != level_[v] + 1 ||
          edges_[id].cap - edges_[id].flow <= 0) {
        continue;
      }

      const int tr = Dfs(to, t,
                         std::min(pushed, edges_[id].cap - edges_[id].flow));
      if (!tr) {
        continue;
      }

      edges_[id].flow += tr;
      edges_[id ^ 1].flow -= tr;
      return tr;
    }
    return 0;
  }

public:
  explicit Dinic(const int n) : n_(n), adj_(n), level_(n), ptr_(n) {
  }

  void AddEdge(const int u, const int v, const int cap) {
    edges_.push_back({v, cap, 0});
    edges_.push_back({u, cap, 0});
    adj_[u].push_back(static_cast<int>(edges_.size()) - 2);
    adj_[v].push_back(static_cast<int>(edges_.size()) - 1);
  }

  int MaxFlow(const int s, const int t) {
    int flow = 0;
    while (Bfs(s, t)) {
      fill(ptr_.begin(), ptr_.end(), 0);
      while (const int pushed = Dfs(s, t, INT_MAX)) {
        flow += pushed;
      }
    }
    return flow;
  }

  void ResetFlows() {
    for (auto& e : edges_) {
      e.flow = 0;
    }
  }
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

std::unordered_map<std::string, int> id;

int GetId(const std::string& s) {
  if (!id.contains(s)) {
    id[s] = static_cast<int>(id.size());
  }
  return id[s];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  int k = 0;
  std::cin >> n >> m >> k;

  id.reserve(n);
  std::vector<std::pair<int, int> > edges_list;
  edges_list.reserve(m);
  DSU dsu(n);

  for (int i = 0; i < m; i++) {
    std::string s1;
    std::string s2;
    std::cin >> s1 >> s2;

    int u = GetId(s1);
    int v = GetId(s2);
    edges_list.emplace_back(u, v);
    dsu.Unite(u, v);
  }

  int c = 2;
  for (int i = 0; i < k; i++) {
    std::string s1;
    std::string s2;
    std::cin >> s1 >> s2;

    if (!id.contains(s1) || !id.contains(s2) || c <= 0 || dsu.Find(id[s1]) != dsu.Find(id[s2])) {
      std::cout << 0 << "\n";
      continue;
    }

    Dinic dinic(n);
    for (auto& [fst, snd] : edges_list) {
      dinic.AddEdge(fst, snd, 1);
    }

    const int flow = dinic.MaxFlow(id[s1], id[s2]);
    std::cout << flow << "\n";

    if (flow > 0) {
      c--;
    }
  }
}