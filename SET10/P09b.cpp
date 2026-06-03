#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using int64 = long long;

struct NodeResult {
  int64 base;
  std::vector<int64> diffs;
  int leaves;
};

void Dfs(int u, int parent, const std::vector<std::vector<int>>& adj,
         const std::vector<int64>& a, NodeResult& res) {
  std::vector<int> children;
  for (int v : adj[u]) {
    if (v != parent) {
      children.push_back(v);
    }
  }

  const int child_cnt = static_cast<int>(children.size());

  if (child_cnt == 0) {
    // Leaf node: can be 0 or 1
    res.base = std::abs(a[u]);
    res.diffs = {std::abs(a[u] - 1) - std::abs(a[u])};
    res.leaves = 1;
    return;
  }

  // Process all children and merge their diff arrays
  int64 child_sum = 0;
  int total_leaves = 0;
  std::vector<int64> merged;

  for (const int v : children) {
    NodeResult child;
    Dfs(v, u, adj, a, child);
    child_sum += child.base;
    total_leaves += child.leaves;
    merged.insert(merged.end(), child.diffs.begin(), child.diffs.end());
  }

  // Sort all diffs together (each child's diffs are individually sorted,
  // but concatenating and sorting once is simpler for k>=2)
  std::sort(merged.begin(), merged.end());

  res.base = child_sum + std::abs(a[u]);
  res.leaves = total_leaves;
  res.diffs = std::move(merged);

  const int L = res.leaves;
  const int64 neg =
      (a[u] > 0) ? std::min(static_cast<int64>(L), a[u]) : 0;
  for (int i = 0; i < neg; i++) {
    res.diffs[i] += -1;
  }
  for (int i = neg; i < L; i++) {
    res.diffs[i] += 1;
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int64> a(n + 1);
  for (int i = 1; i <= n; i++) {
    std::cin >> a[i];
  }

  std::vector<std::vector<int>> adj(n + 1);
  for (int i = 0; i < n - 1; i++) {
    int u = 0, v = 0;
    std::cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  NodeResult root;
  Dfs(1, -1, adj, a, root);

  // dp_root[v] = root.base + sum of v smallest diffs
  // Since diffs are sorted ascending and dp is convex,
  // minimum is at the point where diff transitions from negative to non-negative
  int64 ans = root.base;
  int64 cum = 0;
  for (const int64 diff : root.diffs) {
    cum += diff;
    if (cum < ans - root.base) {
      ans = root.base + cum;
    }
  }

  std::cout << ans << '\n';

  return 0;
}
