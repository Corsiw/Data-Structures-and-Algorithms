#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>


bool Bfs(const int s, const int t, const std::vector<std::vector<std::pair<int, bool> > >& graph,
         std::vector<int>& parent) {
  fill(parent.begin(), parent.end(), -1);

  std::queue<int> q;
  q.push(s);

  parent[s] = -2;

  while (!q.empty()) {
    const int u = q.front();
    q.pop();

    for (const auto& [v, cap] : graph[u]) {
      if (parent[v] == -1 && !cap) {
        parent[v] = u;

        if (v == t) {
          return true;
        }
        q.push(v);
      }
    }
  }
  return false;
}

int MaxFlow(const int s, const int t, std::vector<std::vector<std::pair<int, bool> > >& graph, const int n) {
  int cur_flow = 0;

  std::vector<int> parent(n);
  while (Bfs(s, t, graph, parent)) {

    for (int v = t; v != s; v = parent[v]) {

      for (int i = 0; i < graph[v].size(); i++) {
        if (graph[v][i].first == parent[v]) {
          graph[v][i].second = true;
          break;
        }
      }

      for (int i = 0; i < graph[parent[v]].size(); i++) {
        if (graph[parent[v]][i].first == v) {
          graph[parent[v]][i].second = true;
          break;
        }
      }
    }
    cur_flow++;
  }
  return cur_flow;
}


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

  std::vector<std::vector<std::pair<int, bool> > > graph(n);
  // DSU dsu(n);

  for (int i = 0; i < m; i++) {
    std::string s1;
    std::string s2;
    std::cin >> s1 >> s2;

    const int u = GetId(s1);
    const int v = GetId(s2);
    graph[u].emplace_back(v, false);
    graph[v].emplace_back(u, false);

    // dsu.Unite(u, v);
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

    if (it1 == id.end() || it2 == id.end()) {
      std::cout << 0 << "\n";
      continue;
    }

    for (int ii = 0; ii < graph.size(); ii++) {
      for (int jj = 0; jj < graph[ii].size(); jj++) {
        graph[ii][jj].second = false;
      }
    }

    const int flow = MaxFlow(it1->second, it2->second, graph, n);
    std::cout << flow << "\n";

    if (flow > 0) {
      c--;
    }
  }
}