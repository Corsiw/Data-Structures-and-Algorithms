#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

struct NodeResult {
  int64_t base;
  std::vector<int64_t> diffs;
  int leaves;
};

void Dfs(const int u, const int parent, const std::vector<std::vector<int> >& adj,
         const std::vector<int64_t>& a, NodeResult& res) {
  std::vector<int> children;
  for (int v : adj[u]) {
    if (v != parent) {
      children.push_back(v);
    }
  }

  if (const int child_cnt = static_cast<int>(children.size()); child_cnt == 0) {
    res.base = std::abs(a[u]);
    res.diffs = {std::abs(a[u] - 1) - std::abs(a[u])};
    res.leaves = 1;
    return;
  }

  int64_t child_sum = 0;
  int total_leaves = 0;
  std::vector<int64_t> merged;

  for (const int v : children) {
    NodeResult child;
    Dfs(v, u, adj, a, child);
    child_sum += child.base;
    total_leaves += child.leaves;
    merged.insert(merged.end(), child.diffs.begin(), child.diffs.end());
  }

  std::ranges::sort(merged);

  res.base = child_sum + std::abs(a[u]);
  res.leaves = total_leaves;
  res.diffs = std::move(merged);

  const int l = res.leaves;
  const int64_t neg =
      (a[u] > 0) ? std::min(static_cast<int64_t>(l), a[u]) : 0;
  for (int i = 0; i < neg; i++) {
    res.diffs[i] += -1;
  }
  for (int i = static_cast<int>(neg); i < l; i++) {
    res.diffs[i] += 1;
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int64_t> a(n + 1);
  for (int i = 1; i <= n; i++) {
    std::cin >> a[i];
  }

  std::vector<std::vector<int> > adj(n + 1);
  for (int i = 0; i < n - 1; i++) {
    int u = 0;
    int v = 0;
    std::cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  NodeResult root;
  Dfs(1, -1, adj, a, root);

  int64_t res = root.base;
  int64_t cum_sum = 0;
  for (const int64_t diff : root.diffs) {
    cum_sum += diff;
    if (cum_sum < res - root.base) {
      res = root.base + cum_sum;
    }
  }

  std::cout << res << '\n';
}