#include <iostream>
#include "btree.h"

int main() {
  BTree tree(2);

  tree.insert(1);
  tree.insert(2);
  tree.insert(5);
  tree.insert(4);
  tree.insert(4);
  tree.insert(5);
}