#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>


struct Edge {
  int to;
  int rev;
  bool used;
};

class Graph {
public:
  int n;
  std::vector<std::vector<Edge> > adj;

  explicit Graph(const int n) : n(n), adj(n) {
  }

  void AddEdge(const int u, const int v) {
    adj[u].push_back({v, static_cast<int>(adj[v].size()), false});
    adj[v].push_back({u, static_cast<int>(adj[u].size()), false});
  }

  bool Dfs(const int u, const int t, std::vector<int>& parent, std::vector<int>& parent_edge,
           std::vector<bool>& visited) {
    if (u == t) {
      return true;
    }
    visited[u] = true;
    for (int i = 0; i < adj[u].size(); i++) {
      Edge& e = adj[u][i];

      if (!e.used && !visited[e.to]) {

        parent[e.to] = u;
        parent_edge[e.to] = i;

        if (Dfs(e.to, t, parent, parent_edge, visited)) {
          return true;
        }
      }
    }
    return false;
  }

  int MaxEdgeDisjointPaths(const int s, const int t) {
    int flow = 0;
    std::vector<int> parent(n);
    std::vector<int> parent_edge(n);
    std::vector<bool> visited(n);

    while (true) {
      std::fill(visited.begin(), visited.end(), false);
      if (!Dfs(s, t, parent, parent_edge, visited)) {
        break;
      }

      int v = t;
      while (v != s) {
        const int u = parent[v];
        const int idx = parent_edge[v];
        adj[u][idx].used = true;
        adj[v][adj[u][idx].rev].used = true;
        v = u;
      }
      flow++;
    }
    return flow;
  }

  void ResetUsed() {
    for (auto& vec : adj) {
      for (auto& e : vec) {
        e.used = false;
      }
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

  id.reserve(n * 2);
  id.max_load_factor(0.7);

  Graph graph(n);
  DSU dsu(n);

  for (int i = 0; i < m; i++) {
    std::string s1;
    std::string s2;
    std::cin >> s1 >> s2;

    const int u = GetId(s1);
    const int v = GetId(s2);

    graph.AddEdge(u, v);
    dsu.Unite(u, v);
  }

  int c = 2;
  for (int i = 0; i < k; i++) {
    std::string s1;
    std::string s2;
    std::cin >> s1 >> s2;

    if (c <= 0) {
      std::cout << 0 << "\n";
      continue;
    }

    const auto it1 = id.find(s1);
    const auto it2 = id.find(s2);

    if (dsu.Find(it1->second) != dsu.Find(it2->second)) {
      std::cout << 0 << "\n";
      continue;
    }

    graph.ResetUsed();
    const int flow = graph.MaxEdgeDisjointPaths(it1->second, it2->second);
    std::cout << flow << "\n";

    if (flow > 0) {
      c--;
    }
  }
}