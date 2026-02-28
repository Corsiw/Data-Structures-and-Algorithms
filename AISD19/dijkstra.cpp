#include <iostream>
#include <vector>
#include <queue>
#include <limits>

constexpr int kInf = std::numeric_limits<int>::max();
constexpr int kN = 5; // A, B, C, D, E

// Индексы вершин
enum { A, B, C, D, E };

// Граф в виде списка смежности
std::vector<std::vector<std::pair<int, int> > > BuildGraph() {
  std::vector<std::vector<std::pair<int, int> > > g(kN);

  auto add_edge = [&](int u, int v, int w) {
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
  };

  add_edge(A, B, 4);
  add_edge(A, C, 2);
  add_edge(B, D, 3);
  add_edge(B, E, 5);
  add_edge(C, D, 1);
  add_edge(C, E, 4);
  add_edge(D, E, 2);
  return g;
}

int DijkstraClassic(const std::vector<std::vector<std::pair<int, int> > >& g,
                    int s, int t, int& processed) {
  std::vector<int> dist(kN, kInf);
  std::vector<bool> used(kN, false);

  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int> >,
                      std::greater<std::pair<int, int> > > pq;

  dist[s] = 0;
  pq.emplace(0, s);
  processed = 0;

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();

    if (used[u]) {
      continue;
    }

    used[u] = true;
    processed++;

    if (u == t) {
      break;
    }

    for (auto [v, w] : g[u]) {
      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        pq.emplace(dist[v], v);
      }
    }
  }

  return dist[t];
}

int DijkstraBidirectional(const std::vector<std::vector<std::pair<int, int> > >& g,
                          int s, int t, int& processed) {
  std::vector<int> dist_f(kN, kInf);
  std::vector<int> dist_b(kN, kInf);
  std::vector<bool> used_f(kN, false);
  std::vector<bool> used_b(kN, false);

  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int> >,
                      std::greater<std::pair<int, int> > > pq_f;
  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int> >,
                      std::greater<std::pair<int, int> > > pq_b;

  dist_f[s] = 0;
  dist_b[t] = 0;

  pq_f.emplace(0, s);
  pq_b.emplace(0, t);

  int mu = kInf;
  processed = 0;

  while (!pq_f.empty() && !pq_b.empty()) {

    // --- Forward шаг ---
    if (!pq_f.empty()) {
      auto [d, u] = pq_f.top();
      pq_f.pop();

      if (!used_f[u]) {
        used_f[u] = true;
        processed++;

        if (used_b[u]) {
          mu = std::min(mu, dist_f[u] + dist_b[u]);
        }

        for (auto [v, w] : g[u]) {
          if (dist_f[v] > dist_f[u] + w) {
            dist_f[v] = dist_f[u] + w;
            pq_f.emplace(dist_f[v], v);
          }
        }
      }
    }

    // Backward шаг
    if (!pq_b.empty()) {
      auto [d, u] = pq_b.top();
      pq_b.pop();

      if (!used_b[u]) {
        used_b[u] = true;
        processed++;

        if (used_f[u]) {
          mu = std::min(mu, dist_f[u] + dist_b[u]);
        }

        for (auto [v, w] : g[u]) {
          if (dist_b[v] > dist_b[u] + w) {
            dist_b[v] = dist_b[u] + w;
            pq_b.emplace(dist_b[v], v);
          }
        }
      }
    }

    const int min_f = pq_f.empty() ? kInf : pq_f.top().first;
    const int min_b = pq_b.empty() ? kInf : pq_b.top().first;

    if (min_f + min_b >= mu) {
      break;
    }
  }

  return mu;
}


int main() {

  const auto graph = BuildGraph();

  int processed_classic = 0;
  int processed_bi = 0;

  const int dist_classic = DijkstraClassic(graph, A, E, processed_classic);
  const int dist_bi = DijkstraBidirectional(graph, A, E, processed_bi);

  std::cout << "Classic:\n";
  std::cout << "Shortest path = " << dist_classic << '\n';
  std::cout << "Counter = " << processed_classic << '\n';

  std::cout << "\nBidirectional:\n";
  std::cout << "Shortest path = " << dist_bi << '\n';
  std::cout << "Counter = " << processed_bi << '\n';

  return 0;
}