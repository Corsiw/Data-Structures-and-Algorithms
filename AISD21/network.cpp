#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <random>

struct Edge {
  int to, cap, flow;
};

class Network {
  int n_;
  std::vector<std::vector<int> > adj_;
  std::vector<Edge> edges_;

  std::vector<int> level_;
  std::vector<int> ptr_;

public:
  explicit Network(const int n)
    : n_(n), adj_(n), level_(n), ptr_(n) {
  }

  void AddEdge(const int u, const int v, const int cap) {
    edges_.push_back({v, cap, 0});
    edges_.push_back({u, 0, 0});

    adj_[u].push_back(edges_.size() - 2);
    adj_[v].push_back(edges_.size() - 1);
  }

  void ResetFlows() {
    for (auto& e : edges_)
      e.flow = 0;
  }

private:
  bool Bfs(int s, int t) {
    std::ranges::fill(level_, -1);

    std::queue<int> q;
    q.push(s);
    level_[s] = 0;

    while (!q.empty()) {
      const int v = q.front();
      q.pop();

      for (const int id : adj_[v]) {
        if (edges_[id].cap - edges_[id].flow <= 0) {
          continue;
        }

        if (int to = edges_[id].to; level_[to] == -1) {
          level_[to] = level_[v] + 1;
          q.push(to);
        }
      }
    }

    return level_[t] != -1;
  }

  int DfsDinic(const int v, const int t, const int pushed) {
    if (pushed == 0) {
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

      const int tr = DfsDinic(
          to,
          t,
          std::min(pushed, edges_[id].cap - edges_[id].flow));

      if (tr == 0) {
        continue;
      }

      edges_[id].flow += tr;
      edges_[id ^ 1].flow -= tr;

      return tr;
    }

    return 0;
  }

public:
  int Dinic(int s, int t) {

    int flow = 0;

    while (Bfs(s, t)) {

      std::ranges::fill(ptr_, 0);

      while (const int pushed = DfsDinic(s, t, INT_MAX)) {
        flow += pushed;
      }
    }

    return flow;
  }

private:
  int DfsFord(const int v, const int t, const int pushed, std::vector<int>& used) {

    if (v == t) {
      return pushed;
    }

    used[v] = 1;

    for (const int id : adj_[v]) {

      Edge& e = edges_[id];

      if (used[e.to]) {
        continue;
      }

      if (e.cap - e.flow <= 0) {
        continue;
      }

      const int tr = DfsFord(
          e.to,
          t,
          std::min(pushed, e.cap - e.flow),
          used);

      if (tr == 0) {
        continue;
      }

      e.flow += tr;
      edges_[id ^ 1].flow -= tr;

      return tr;
    }

    return 0;
  }

public:
  int FordFulkerson(const int s, const int t) {

    int flow = 0;

    while (true) {

      std::vector<int> used(n_, 0);

      const int pushed = DfsFord(s, t, INT_MAX, used);

      if (pushed == 0) {
        break;
      }

      flow += pushed;
    }

    return flow;
  }

  int MaxCapacityPath(int s, const int t) {

    int flow = 0;

    while (true) {

      std::vector<int> cap(n_, 0);
      std::vector<int> parent(n_, -1);
      std::vector<int> parent_edge(n_, -1);

      std::priority_queue<std::pair<int, int> > pq;

      cap[s] = INT_MAX;
      pq.emplace(cap[s], s);

      while (!pq.empty()) {

        auto [c, v] = pq.top();
        pq.pop();

        if (c < cap[v]) {
          continue;
        }

        for (const int id : adj_[v]) {

          Edge& e = edges_[id];

          int residual = e.cap - e.flow;

          if (residual <= 0) {
            continue;
          }

          const int new_cap = std::min(cap[v], residual);

          if (new_cap > cap[e.to]) {

            cap[e.to] = new_cap;
            parent[e.to] = v;
            parent_edge[e.to] = id;

            pq.emplace(cap[e.to], e.to);
          }
        }
      }

      if (cap[t] == 0) {
        break;
      }

      const int add = cap[t];
      flow += add;

      int v = t;

      while (v != s) {

        const int id = parent_edge[v];

        edges_[id].flow += add;
        edges_[id ^ 1].flow -= add;

        v = parent[v];
      }
    }

    return flow;
  }
};

Network GenerateGraph(const int n, const int m, const int max_cap) {

  Network g(n);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> node_dist(0, n - 1);
  std::uniform_int_distribution<> cap_dist(1, max_cap);

  for (int i = 0; i < m; i++) {

    const int u = node_dist(gen);
    const int v = node_dist(gen);

    if (u == v) {
      continue;
    }

    g.AddEdge(u, v, cap_dist(gen));
  }

  return g;
}


template <typename Func>
int64_t Measure(Func f) {

  const auto start = std::chrono::high_resolution_clock::now();

  f();

  const auto end = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}


int main() {

  const int n = 200;
  const int m = 2000;

  Network g = GenerateGraph(n, m, 100);

  const int s = 0;
  const int t = n - 1;

  const int64_t time_dinic = Measure([&]() {

    const int flow = g.Dinic(s, t);
    std::cout << "Dinic flow = " << flow << '\n';

  });

  g.ResetFlows();

  const int64_t time_ff = Measure([&]() {

    const int flow = g.FordFulkerson(s, t);
    std::cout << "Ford-Fulkerson flow = " << flow << '\n';

  });

  g.ResetFlows();

  const int64_t time_cap = Measure([&]() {

    const int flow = g.MaxCapacityPath(s, t);
    std::cout << "MaxCapacity flow = " << flow << '\n';

  });

  std::cout << std::endl;

  std::cout << "Time Dinic: " << time_dinic << " ms\n";
  std::cout << "Time Ford-Fulkerson: " << time_ff << " ms\n";
  std::cout << "Time MaxCapacity: " << time_cap << " ms\n";
}