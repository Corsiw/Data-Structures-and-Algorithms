#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

struct Edge {
  int to, cap, flow, rev;
};

class Dinic {
  int n_;
  std::vector<std::vector<int> > adj_;
  std::vector<Edge> edges_;
  std::vector<int> level_;
  std::vector<int> ptr_;
  std::queue<int> q_;

  bool Bfs(const int s, const int t) {
    std::ranges::fill(level_, -1);
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
    const Edge a = {v, cap, 0, static_cast<int>(edges_.size()) + 1};
    const Edge b = {u, 0, 0, static_cast<int>(edges_.size())};
    adj_[u].push_back(static_cast<int>(edges_.size()));
    edges_.push_back(a);
    adj_[v].push_back(static_cast<int>(edges_.size()));
    edges_.push_back(b);
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

  bool FindPath(const int s, const int t, std::vector<int>& path) {
    path.clear();
    std::stack<int> st;
    st.push(s);
    std::vector<int> parent(n_, -1);
    std::vector<int> edge_used(n_, -1);
    std::vector<bool> visited(n_, false);
    visited[s] = true;

    while (!st.empty()) {
      const int v = st.top();
      st.pop();
      if (v == t) {
        break;
      }
      for (const int id : adj_[v]) {
        auto& [to, cap, flow, rev] = edges_[id];
        if (flow > 0 && !visited[to]) {
          visited[to] = true;
          st.push(to);
          parent[to] = v;
          edge_used[to] = id;
        }
      }
    }

    if (!visited[t]) {
      return false;
    }

    int cur = t;
    while (cur != s) {
      path.push_back(cur);
      const int id = edge_used[cur];
      edges_[id].flow--;
      edges_[edges_[id].rev].flow++;
      cur = parent[cur];
    }
    path.push_back(s);
    std::ranges::reverse(path.begin(), path.end());
    return true;
  }
};

int main() {
  int n = 0;
  int m = 0;
  int a = 0;
  int h = 0;

  std::cin >> n >> m >> a >> h;

  a--;
  h--;

  Dinic dinic(n);
  for (int i = 0; i < m; i++) {
    int x = 0;
    int y = 0;
    std::cin >> x >> y;

    dinic.AddEdge(x - 1, y - 1, 1);
  }

  const auto res = dinic.MaxFlow(a, h) >= 2 ? "YES\n" : "NO\n";

  std::cout << res;
  if (res == "NO\n") {
    exit(0);
  }

  for (int i = 0; i < 2; i++) {
    std::vector<int> path;
    dinic.FindPath(a, h, path);
    for (size_t j = 0; j < path.size(); j++) {
      std::cout << path[j] + 1 << (j + 1 == path.size() ? '\n' : ' ');
    }
  }
}