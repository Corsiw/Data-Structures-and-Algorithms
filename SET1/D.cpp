#include <iostream>
#include <vector>
#include <stack>

int main() {
  int n = 0;
  std::cin >> n;
  std::vector<int> train(n);
  for (int i = 0; i < n; i++) {
    std::cin >> train[i];
  }

  std::vector<std::pair<int, int> > actions;
  std::stack<int> stack;
  int current = 1;
  int i = 0;

  while (i < n || !stack.empty()) {
    if (!stack.empty() && stack.top() == current) {
      int count = 0;
      while (!stack.empty() && stack.top() == current) {
        stack.pop();
        count++;
        current++;
      }
      actions.emplace_back(2, count);
    } else if (i < n) {
      int count = 0;
      int start = i;

      while (i < n && train[i] != current) {
        stack.push(train[i]);
        count++;
        i++;
      }

      if (i < n && train[i] == current) {
        count++;
        stack.push(train[i]);
        i++;
      }

      if (count > 0) {
        actions.emplace_back(1, count);
      }
    } else {
      std::cout << 0;
      return 0;
    }
  }

  for (const auto action : actions) {
    std::cout << action.first << " " << action.second << '\n';
  }

  return 0;
}