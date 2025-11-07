#include <iostream>
#include <vector>
#include <deque>

int main() {
  int first = 10;
  int second = -3;
  std::cout<< first / second << " " << first %  second << std::endl;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t = 0;
  std::cin >> t;
  std::deque<int> left;
  std::deque<int> right;

  while (t--) {
    char action = 0;
    std::cin >> action;

    if (action == '-') {
      std::cout << left.front() << '\n';
      left.pop_front();

    } else if (action == '+') {
      int i = 0;
      std::cin >> i;
      right.push_back(i);

    } else {
      int i = 0;
      std::cin >> i;
      right.push_front(i);
    }
    if (right.size() > left.size()) {
      left.push_back(right.front());
      right.pop_front();
    }
  }
}