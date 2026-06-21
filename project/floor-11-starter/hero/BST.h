// COMP 2450 — Floor 10 starter
// hero/BST.h — a binary search tree with self-balancing rotations.
// COMPLETE Floor 9½ reference (your Floor 9 BST plus the balance work).
//
// You built the BST on Floor 9 (The Halving Hall) and added balance on
// Floor 9½ (The Balanced Hall): the diagnostics that MEASURE imbalance
// (balanceFactor / isBalanced) and the rotations that REPAIR it. It ships
// finished here so this floor's bestiary code can use it, and so you can
// read it beside the std::map you reach for this week — the library's
// balanced tree, handed to you done.
//
// One convention to notice: height() counts EDGES, so an empty tree and a
// single leaf are both 0. Balance needs a finer ruler — signedHeight() makes
// an empty subtree -1, one less than a leaf — which is the only reason a
// three-node "stick" reads as unbalanced instead of balanced.

#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace dungeon {

template <typename T>
class BST {
public:
    struct Node {
        T     data;
        Node* left  = nullptr;
        Node* right = nullptr;
        explicit Node(const T& v) : data(v) {}
    };

    // ----- lifetime --------------------------------------------------
    BST() = default;
    ~BST() { destroy(root_); }
    BST(const BST& other) { root_ = clone(other.root_); }
    BST& operator=(const BST& other) {
        if (this != &other) {
            BST tmp(other);
            std::swap(root_, tmp.root_);
        }
        return *this;
    }

    // ----- access ----------------------------------------------------
    const Node* root()  const { return root_; }
    bool        empty() const { return root_ == nullptr; }
    std::size_t size()   const { return countFrom(root_); }
    std::size_t height() const { return heightFrom(root_); }   // EDGES

    // ----- Floor 9 operations ----------------------------------------
    void insert(const T& value)            { root_ = insertInto(root_, value); }
    bool contains(const T& value) const    { return findFrom(root_, value) != nullptr; }
    const Node* find(const T& value) const { return findFrom(root_, value); }
    std::vector<T> inOrder() const {
        std::vector<T> out;
        inOrderInto(root_, out);
        return out;
    }
    bool remove(const T& value) {
        bool removed = false;
        root_ = removeFrom(root_, value, removed);
        return removed;
    }

    // ===== Floor 9½ — balance delegators =============================
    int  balanceFactor() const { return balanceFactorOf(root_); }
    bool isBalanced()    const { return isBalancedFrom(root_); }
    bool rotateRootRight() {
        if (root_ == nullptr || root_->left == nullptr) return false;
        root_ = rotateRight(root_);
        return true;
    }
    bool rotateRootLeft() {
        if (root_ == nullptr || root_->right == nullptr) return false;
        root_ = rotateLeft(root_);
        return true;
    }
    bool balanceRoot() {
        const int bf = balanceFactorOf(root_);
        root_ = rebalanceAt(root_);
        return bf < -1 || bf > 1;
    }

private:
    Node* root_ = nullptr;

    // ===== size/height — PROVIDED ====================================
    static std::size_t countFrom(const Node* n) {
        if (n == nullptr) return 0;
        return 1 + countFrom(n->left) + countFrom(n->right);
    }
    static std::size_t heightFrom(const Node* n) {
        if (n == nullptr || (n->left == nullptr && n->right == nullptr)) return 0;
        return 1 + std::max(heightFrom(n->left), heightFrom(n->right));
    }
    // signedHeight — like heightFrom() but an EMPTY subtree is -1, not 0 (a
    // leaf is 0, a missing child is -1). This is the ruler the balance factor
    // measures with; see the header note.
    static int signedHeight(const Node* n) {
        if (n == nullptr) return -1;
        return 1 + std::max(signedHeight(n->left), signedHeight(n->right));
    }

    // ===== Floor 9 helpers — PROVIDED (finished last week) ===========
    static Node* insertInto(Node* n, const T& value) {
        if (n == nullptr) return new Node(value);
        if (value < n->data)      n->left  = insertInto(n->left,  value);
        else if (n->data < value) n->right = insertInto(n->right, value);
        return n;
    }
    static const Node* findFrom(const Node* n, const T& value) {
        if (n == nullptr)    return nullptr;
        if (value < n->data) return findFrom(n->left,  value);
        if (n->data < value) return findFrom(n->right, value);
        return n;
    }
    static void inOrderInto(const Node* n, std::vector<T>& out) {
        if (n == nullptr) return;
        inOrderInto(n->left, out);
        out.push_back(n->data);
        inOrderInto(n->right, out);
    }
    static void destroy(Node* n) {
        if (n == nullptr) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
    static Node* clone(const Node* n) {
        if (n == nullptr) return nullptr;
        Node* copy = new Node(n->data);
        copy->left  = clone(n->left);
        copy->right = clone(n->right);
        return copy;
    }
    static Node* removeFrom(Node* n, const T& value, bool& removed) {
        if (n == nullptr) return nullptr;
        if (value < n->data) { n->left  = removeFrom(n->left,  value, removed); return n; }
        if (n->data < value) { n->right = removeFrom(n->right, value, removed); return n; }
        removed = true;
        if (n->left == nullptr)  { Node* r = n->right; delete n; return r; }
        if (n->right == nullptr) { Node* l = n->left;  delete n; return l; }
        Node* succ = n->right;
        while (succ->left != nullptr) succ = succ->left;
        n->data = succ->data;
        bool discard = false;
        n->right = removeFrom(n->right, succ->data, discard);
        return n;
    }

    // ===== Floor 9½ helpers — balance: diagnostics + rotations =======
    // Complete reference — you wrote these on Floor 9½ (The Balanced Hall).

    // ----- diagnose imbalance ----------------------------------------

    // Balance factor of n: signedHeight(left) - signedHeight(right). A null
    // node is 0. Positive = left-heavy, negative = right-heavy.
    static int balanceFactorOf(const Node* n) {
        if (n == nullptr) return 0;
        return signedHeight(n->left) - signedHeight(n->right);
    }

    // True iff the subtree at n is height-balanced at EVERY node. Check THIS
    // node's balance factor is within [-1, 1], then require both children to
    // be balanced too.
    static bool isBalancedFrom(const Node* n) {
        if (n == nullptr) return true;
        const int bf = balanceFactorOf(n);
        if (bf < -1 || bf > 1) return false;
        return isBalancedFrom(n->left) && isBalancedFrom(n->right);
    }

    // ----- the rotation primitive ------------------------------------

    // Right rotation around n. n's LEFT child (pivot) becomes the new subtree
    // root; n becomes pivot's right child; pivot's old right subtree becomes
    // n's new left. Return the new subtree root. Precondition: n and n->left
    // non-null. A rotation does NOT change the in-order order — that is exactly
    // what makes it a safe repair.
    //
    /*
               n                 pivot
              / \                /   \
          pivot  c    =>        a     n
           / \                        / \
          a   b                      b   c
    */
    static Node* rotateRight(Node* n) {
        Node* pivot  = n->left;
        n->left      = pivot->right;
        pivot->right = n;
        return pivot;
    }

    // Left rotation around n — the mirror of rotateRight. n's RIGHT child
    // becomes the new subtree root. Precondition: n and n->right non-null.
    // Also in-order-preserving.
    static Node* rotateLeft(Node* n) {
        Node* pivot = n->right;
        n->right    = pivot->left;
        pivot->left = n;
        return pivot;
    }

    // ----- pick the rotation -----------------------------------------

    // If n is out of balance, apply the one rotation that fixes a simple
    // imbalance: rotate right if left-heavy, left if right-heavy. Return the
    // (possibly new) subtree root. A real AVL tree calls this at every node on
    // the way back up an insert/remove (and adds a second rotation for the
    // zig-zag cases) so it never becomes a stick — which is exactly the work
    // std::set / std::map do for you, and why you reach for them this floor.
    static Node* rebalanceAt(Node* n) {
        if (n == nullptr) return n;
        const int bf = balanceFactorOf(n);
        if (bf > 1)  return rotateRight(n);   // left-heavy
        if (bf < -1) return rotateLeft(n);    // right-heavy
        return n;
    }
};

}  // namespace dungeon
