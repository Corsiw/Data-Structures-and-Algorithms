#include <string>
#include <iostream>
#include <Windows.h>
#include <cstdio>
#include "splay-tree.h"

void print(Node* n, const std::string& prefix = "", bool left = true)
{
    if (!n) return;
    std::cout << prefix << (left ? "├─" : "└─") << n->key << "\n";

    print(n->left,  prefix + (left ? "│  " : "   "), true);
    print(n->right, prefix + (left ? "│  " : "   "), false);
}

void printTree(Node* root)
{
    if (!root) {
        std::cout << "(empty)\n";
        return;
    }
    print(root, "", false);
}

// -----------------------------------------------------------
//   TEST 1: Zig (simple right rotation)
// -----------------------------------------------------------
//       10                5
//      /       --->        \
//     5                    10
//
void testZigRight()
{
    std::cout << "\n==== TEST: ZIG (right) ====\n";

    SplayTree t;
    // Манипулируем вручную
    Node* n10 = new Node(10, nullptr);
    Node* n5  = new Node(5, n10);
    n10->left = n5;

    // задаём корень вручную
    *(Node**)(&t) = n10;

    std::cout << "Before:\n";
    printTree(n10);

    t.splay(n5);

    std::cout << "After:\n";
    printTree(*(Node**)(&t));

    std::cout << "Expected root = 5\n";
}

// -----------------------------------------------------------
//   TEST 2: Zig (simple left rotation)
// -----------------------------------------------------------
//   5                       10
//    \          --->       /
//    10                   5
//
void testZigLeft()
{
    std::cout << "\n==== TEST: ZIG (left) ====\n";

    SplayTree t;
    Node* n5  = new Node(5, nullptr);
    Node* n10 = new Node(10, n5);
    n5->right = n10;

    *(Node**)(&t) = n5;

    std::cout << "Before:\n";
    printTree(n5);

    t.splay(n10);

    std::cout << "After:\n";
    printTree(*(Node**)(&t));

    std::cout << "Expected root = 10\n";
}

// -----------------------------------------------------------
//   TEST 3: Zig-Zig right-right
// -----------------------------------------------------------
//       20                  10
//      /                  /    \
//     10       --->      5      20
//    /
//   5
//
void testZigZigRight()
{
    std::cout << "\n==== TEST: ZIG-ZIG (right-right) ====\n";

    SplayTree t;

    Node* n20 = new Node(20, nullptr);
    Node* n10 = new Node(10, n20);
    Node* n5  = new Node(5,  n10);

    n20->left = n10;
    n10->left = n5;

    *(Node**)(&t) = n20;

    std::cout << "Before:\n";
    printTree(n20);

    t.splay(n5);

    std::cout << "After:\n";
    printTree(*(Node**)(&t));

    std::cout << "Expected root = 5\n";
}

// -----------------------------------------------------------
//   TEST 4: Zig-Zig left-left
// -----------------------------------------------------------
//   5                       20
//    \                    /    \
//    10      --->       5      30
//      \
//      30
//
void testZigZigLeft()
{
    std::cout << "\n==== TEST: ZIG-ZIG (left-left) ====\n";

    SplayTree t;

    Node* n5  = new Node(5, nullptr);
    Node* n10 = new Node(10, n5);
    Node* n30 = new Node(30, n10);

    n5->right = n10;
    n10->right = n30;

    *(Node**)(&t) = n5;

    std::cout << "Before:\n";
    printTree(n5);

    t.splay(n30);

    std::cout << "After:\n";
    printTree(*(Node**)(&t));

    std::cout << "Expected root = 30\n";
}

// -----------------------------------------------------------
//   TEST 5: Zig-Zag left-right
// -----------------------------------------------------------
//       20                    15
//      /                    /    \
//     10       --->        10     20
//       \
//       15
//
void testZigZagLR()
{
    std::cout << "\n==== TEST: ZIG-ZAG (left-right) ====\n";

    SplayTree t;

    Node* n20 = new Node(20, nullptr);
    Node* n10 = new Node(10, n20);
    Node* n15 = new Node(15, n10);

    n20->left = n10;
    n10->right = n15;

    *(Node**)(&t) = n20;

    std::cout << "Before:\n";
    printTree(n20);

    t.splay(n15);

    std::cout << "After:\n";
    printTree(*(Node**)(&t));

    std::cout << "Expected root = 15\n";
}

// -----------------------------------------------------------
//   TEST 6: Zig-Zag right-left
// -----------------------------------------------------------
//   10                         15
//     \                      /    \
//     20        --->       10      20
//    /
//   15
//
void testZigZagRL()
{
    std::cout << "\n==== TEST: ZIG-ZAG (right-left) ====\n";

    SplayTree t;

    Node* n10 = new Node(10, nullptr);
    Node* n20 = new Node(20, n10);
    Node* n15 = new Node(15, n20);

    n10->right = n20;
    n20->left = n15;

    *(Node**)(&t) = n10;

    std::cout << "Before:\n";
    printTree(n10);

    t.splay(n15);

    std::cout << "After:\n";
    printTree(*(Node**)(&t));

    std::cout << "Expected root = 15\n";
}

// -----------------------------------------------------------

int main()
{
  SetConsoleOutputCP(CP_UTF8);

  // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
  setvbuf(stdout, nullptr, _IOFBF, 1000);

    testZigRight();
    testZigLeft();
    testZigZigRight();
    testZigZigLeft();
    testZigZagLR();
    testZigZagRL();
}
