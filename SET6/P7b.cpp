#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

struct Adjacency {
  int node, weight;
};

// Для восстановления результата - минимальной сумму, количества путешествий и самих путешествий
struct Result {
  std::vector<int> dist;
  std::vector<int> parent;
  std::vector<int> move_value;
};

// Если сейчас год X, то из него можно совершить путешествие в прошлое или будущее
// на любое положительное число лет Y,
// десятичная запись которого является подстрокой десятичной записи числа X.
// Стоимость такого путешествия равна сумме цифр числа Y.
// Необходимо за минимальную стоимость попасть из года a в год b,
// при этом все промежуточные года, которые посетит Доктор, должны быть положительными
// (нельзя переместиться в год раньше создания первой общественной машины времени) и не должны превышать n
// (в год (n+1) все такие машины времени запретили).
std::vector<Adjacency> GetY(const int x, const int n) {
  std::vector<Adjacency> result;

  const std::string s = std::to_string(x);
  const int len = static_cast<int>(s.size());

  for (int i = 0; i < len; i++) {
    int y = 0;
    int digit_sum = 0;

    for (int j = i; j < len; j++) {
      const int digit = s[j] - '0';

      y = y * 10 + digit;
      digit_sum += digit;

      if (y == 0) {
        continue; // Y должно быть положительным
      }

      if (x + y <= n) {
        result.push_back({x + y, digit_sum});
      }

      if (x - y >= 1) {
        result.push_back({x - y, digit_sum});
      }
    }
  }

  return result;
}

// Задача на поиск кратчайшего пути во взвешенном графе, без отрицательных ребёр,
// используем Дейкстру.
Result Dijkstra(int start, const int n) {
  std::vector<int> dist(n + 1, INT_MAX);
  dist[start] = 0;

  std::vector<int> parent(n + 1);
  std::vector<int> move_value(n + 1);

  std::priority_queue<
    std::pair<int, int>, // (distance, node)
    std::vector<std::pair<int, int> >,
    std::greater<std::pair<int, int> >
  > pq;
  pq.emplace(0, start);

  while (!pq.empty()) {
    auto [dist_v, v] = pq.top();
    pq.pop();

    if (dist_v != dist[v]) {
      continue;
    }

    for (auto& [to, weight] : GetY(v, n)) {

      if (dist[to] > dist[v] + weight) {
        dist[to] = dist[v] + weight;
        parent[to] = v;
        move_value[to] = weight;
        pq.emplace(dist[to], to);
      }
    }
  }

  return {dist, parent, move_value};
}


int main() {
  int n = 0;
  int a = 0;
  int b = 0;

  std::cin >> n >> a >> b;

  const auto res = Dijkstra(a, n);
  if (res.dist[b] == INT_MAX) {
    std::cout << -1 << '\n';
    exit(0);
  }

  std::cout << res.dist[b] << '\n';

  int counter = 0;
  int cur = b;
  std::vector<int> path;
  while (cur != a) {
    path.push_back(cur - res.parent[cur]);
    counter++;
    cur = res.parent[cur];
  }
  std::cout << counter << '\n';

  for (int i = 0; i < path.size(); i++) {
    if (path[path.size() - 1 - i] > 0) {
      std::cout << '+' << path[path.size() - 1 - i] << '\n';
    } else {
      std::cout << path[path.size() - 1 - i] << '\n';
    }
  }
}