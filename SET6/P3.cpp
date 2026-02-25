#include<iostream>
#include<vector>

// DSU умеет добавлять ребра, но не удалять. Т.к. в задаче все ребра в конце концов удаляется,
// то можем обработать операции в обратном порядке и вывести их результат в обратном порядке,
// тогда удаление ребра - это добавление
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

struct Operation {
  int u, v;
  bool isCut;
};

int main() {
  int n = 0;
  int m = 0;
  int k = 0;
  std::cin >> n >> m >> k;

  for (int i = 0; i < m; i++) {
    int dummy = 0;
    std::cin >> dummy >> dummy;
  }

  std::vector<Operation> operations;
  for (int i = 0; i < k; i++) {
    std::string op;
    int u = 0;
    int v = 0;

    std::cin >> op >> u >> v;
    if (op == "cut") {
      operations.push_back({u - 1, v - 1, true});
    } else {
      operations.push_back({u - 1, v - 1, false});
    }
  }

  std::vector<std::string> res;

  DSU dsu(n);
  for (int i = 0; i < operations.size(); i++) {
    if (operations[k - 1 - i].isCut) {
      dsu.Unite(operations[k - 1 - i].u, operations[k - 1 - i].v);
    } else {
      auto ans = dsu.Find(operations[k - 1 - i].u) == dsu.Find(operations[k - 1 - i].v) ? "YES" : "NO";
      res.emplace_back(ans);
    }
  }

  for (int i = 0; i < res.size(); i++) {
    if (i != 0) {
      std::cout << '\n';
    }
    std::cout << res[res.size() - 1 - i];
  }
}