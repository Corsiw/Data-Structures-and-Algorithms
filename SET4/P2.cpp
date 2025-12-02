#include <iostream>
#include <vector>

template <typename T>
class TreeNode {
public:
  mutable T data;
  mutable TreeNode* left = nullptr;
  TreeNode* right = nullptr;
  bool isRed = false;

  TreeNode() : data() {
  };

  explicit TreeNode(T data) : data(data), left(nullptr), right(nullptr) {
  }

  TreeNode(T data, const bool is_red) : data(data), left(nullptr), right(nullptr), isRed(is_red) {
  }

  TreeNode(
      T data,
      TreeNode* left,
      TreeNode* right,
      const bool is_red) : data(data), left(left), right(right), isRed(is_red) {
  }
};

template <typename T>
class RBTree {
  TreeNode<T>* root_ = nullptr;

public:
  RBTree() = default;

  explicit RBTree(TreeNode<T>* root) : root_(root) {
  }

  RBTree(T data, const bool is_red) : root_(new TreeNode<T>(data, is_red)) {
  }

  RBTree(T data, TreeNode<T>* left, TreeNode<T>* right,
         const bool is_red) : root_(new TreeNode<T>(data, left, right, is_red)) {
  }

  bool IsBalanced() {
    if (root_ == nullptr || root_->isRed) {
      return false;
    }
    return IsBalancedRec(root_).second;
  }

private:
  std::pair<int, bool> IsBalancedRec(TreeNode<T>* node) {
    if (node == nullptr) {
      return {1, true};
    }

    std::pair left = IsBalancedRec(node->left);
    std::pair right = IsBalancedRec(node->right);
    if (!left.second || !right.second || left.first != right.first) {
      return {-1, false};
    }

    if (node->isRed && ((node->left && node->left->isRed) || (node->right && node->right->isRed))) {
      return {-1, false};
    }

    if (node->left && node->left->data > node->data || node->right && node->data > node->right->data) {
      return {-1, false};
    }

    const int black_height = left.first + (node->isRed ? 0 : 1);
    return {black_height, true};
  }
};


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;
  if (n == 0) {
    std::cout << "NO";
    return 0;
  }

  std::vector<TreeNode<int>> nodes(n);

  int root = 0;
  std::cin >> root;

  int number = 0;
  int key = 0;
  std::string l;
  std::string r;
  char color = 0;
  for (int i = 0; i < n; i++) {
    std::cin >> number >> key >> l >> r >> color;
    auto& node = nodes[number - 1];

    node.data = key;

    if (l != "null") {
      node.left = &nodes[std::stoi(l) - 1];
    }
    if (r != "null") {
      node.right = &nodes[std::stoi(r) - 1];
    }

    node.isRed = color == 'R';
  }

  auto tree = RBTree(&nodes[root - 1]);
  const std::string result = tree.IsBalanced() ? "YES" : "NO";
  std::cout << result;
}