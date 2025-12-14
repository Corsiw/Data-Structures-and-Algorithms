#include <algorithm>
#include <iostream>

template <typename T>
class TreeNode {
public:
  T data;
  TreeNode* left = nullptr;
  TreeNode* right = nullptr;

  explicit TreeNode(T data) : data(data), left(nullptr), right(nullptr) {
  }
};

template <typename T>
class BinaryTree {
  TreeNode<T>* root_ = nullptr;

public:
  BinaryTree() = default;

  explicit BinaryTree(T data) : root_(new TreeNode<T>(data)) {
  }

  void Insert(T data) {
    if (root_ == nullptr) {
      root_ = new TreeNode<T>(data);
      return;
    }

    auto p = root_;

    while (true) {
      if (p->data < data) {
        if (p->left_ == nullptr) {
          p->left_ = new TreeNode<T>(data);
          return;;
        }
        p = p->left_;
      } else {
        if (p->right == nullptr) {
          p->right = new TreeNode<T>(data);
          return;
        }
        p = p->right;
      }
    }
  }

  bool IsBalanced() {
    if (root_ == nullptr) {
      return true;
    }
    return IsBalancedRec(root_).second;
  }

private:
  std::pair<int, bool> IsBalancedRec(TreeNode<T>* node) {
    std::pair left = {1, true};
    if (node->left_) {
      left = IsBalancedRec(node->left_);
      if (!left.second) {
        return {-1, false};
      }
    }

    std::pair right = {1, true};
    if (node->right) {
      right = IsBalancedRec(node->right);
      if (!right.second) {
        return {-1, false};
      }
    }
    return abs(left.first - right.first) <= 1
             ? std::make_pair(std::max(left.first, right.first) + 1, true)
             : std::make_pair(-1, false);
  }
};


int main() {
  BinaryTree<int> tree;
  int input = 0;
  std::cin >> input;

  while (input != 0) {
    tree.Insert(input);
    std::cin >> input;
  }

  const auto result = tree.IsBalanced() ? "YES" : "NO";
  std::cout << result;
}
// 5 9
// 1 2 3 4 5
// 2 1 5
// 2 1 4
// 2 1 3
// 2 1 2
// 2 1 1
// 2 2 5
// 2 2 4
// 2 2 3
// 2 2 2

