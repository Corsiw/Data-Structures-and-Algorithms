#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

struct Node {
  char data;
  size_t freq;
  size_t height;

  Node* left;
  Node* right;
  Node* parent;
};

struct Compare {
  bool operator()(const Node* a, const Node* b) const {
    return a->freq > b->freq;
  }
};

class Trie {
public:
  Trie(): root_(nullptr) {
  }

  explicit Trie(Node* root): root_(root) {
  }

  explicit Trie(const std::vector<size_t>& freq) {
    BuildTrie(freq);
  }

  explicit Trie(const std::string& s) {
    std::vector<size_t> freq(256);
    for (size_t i = 0; i < s.size(); i++) {
      freq[s[i]]++;
    }
    BuildTrie(freq);
  }

  std::string CodeString(const std::string& s) {
    std::string res;
    res.reserve(s.size() * root_->height);

    for (size_t i = 0; i < s.size(); i++) {
      res.append(coder_[s[i]]);
    }

    return res;
  }

  size_t GetAlphaSize() const {
    return leafs_.size();
  }

  std::unordered_map<char, std::string> GetCoder() const {
    return coder_;
  }

private:
  void LeafsToStrings() {
    for (const auto node : leafs_) {
      if (root_ == node) {
        coder_[node->data] = "0";
        continue;
      }

      std::string res;
      res.reserve(root_->height);

      const Node* last = node;
      const Node* p = node->parent;
      while (p) {
        if (p->left == last) {
          res.push_back('0');
        } else {
          res.push_back('1');
        }

        last = p;
        p = p->parent;
      }

      std::ranges::reverse(res);
      coder_[node->data] = res;
    }
  }

  void BuildTrie(const std::vector<size_t>& freq) {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (size_t i = 0; i < 256; i++) {
      if (freq[i] > 0) {
        auto node = new Node(static_cast<char>(i), freq[i], 1, nullptr, nullptr, nullptr);
        pq.push(node);
        leafs_.push_back(node);
      }
    }

    while (pq.size() > 1) {
      Node* node1 = pq.top();
      pq.pop();
      Node* node2 = pq.top();
      pq.pop();

      auto parent = new Node('\0', node1->freq + node2->freq, std::max(node1->height, node2->height) + 1, node1, node2,
                             nullptr);
      node1->parent = parent;
      node2->parent = parent;

      pq.push(parent);
    }

    root_ = pq.top();
    LeafsToStrings();
  }

  Node* root_;
  std::vector<Node*> leafs_;
  std::unordered_map<char, std::string> coder_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string s;
  std::cin >> s;

  Trie trie(s);
  const std::string res = trie.CodeString(s);

  std::cout << trie.GetAlphaSize() << ' ' << res.size() << '\n';
  for (auto [fst, snd] : trie.GetCoder()) {
    std::cout << fst << ": " << snd << '\n';
  }
  std::cout << res << '\n';
}