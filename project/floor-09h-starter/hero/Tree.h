// COMP 2450 — Floor 9 starter
// hero/Tree.h — a general (n-ary) tree template. COMPLETE Floor 8 reference.
//
// You built this on Floor 8 (The Hall of Lineages). It ships finished here
// so this floor's catalogue code can use it — and so you can read it side
// by side with the BST<T> you write this week. The two trees own their
// nodes the same way; the recursion you wrote here (and the destructor and
// deep-copy you only READ here) is the recursion you now write on BST.h.
//
//   * A LINE  (Chain): each node has at most one successor.
//   * A TREE  (this):  each node has any number of children, but exactly
//                      one parent. Exactly one node — the ROOT — has no
//                      parent. A node with no children is a LEAF.
//   * A BST   (Floor 9): a tree with AT MOST TWO children and an ordering
//                        rule — and that rule is what makes it searchable.
//
// Children live in a Bag<Node*>; each node also carries a `parent`
// back-pointer so lineage() can walk UP to the root. The Rule of Three
// (recursive deep-copy ctor, copy-assign, recursive destructor) is the
// worked example you imitate on the BST.

#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "Bag.h"
#include "Queue.h"   // levelOrder (BFS) reuses your Floor 7 Queue<T>.

namespace dungeon {

template <typename T>
class Tree {
public:
    struct Node {
        T          data;
        Node*      parent;     // nullptr for the root; set by addChild otherwise.
        Bag<Node*> children;   // owned: created with new, freed by destroy().

        explicit Node(const T& v, Node* p = nullptr) : data(v), parent(p) {}
    };

    // ----- lifetime --------------------------------------------------
    Tree() = default;
    ~Tree() { destroy(root_); }

    Tree(const Tree& other) { root_ = clone(other.root_, nullptr); }
    Tree& operator=(const Tree& other) {
        if (this != &other) {
            Tree tmp(other);
            std::swap(root_, tmp.root_);
        }
        return *this;
    }

    // ----- access ----------------------------------------------------
    Node*       root()        { return root_; }
    const Node* root()  const { return root_; }
    bool        empty() const { return root_ == nullptr; }

    // Plant the root if there isn't one yet; return it either way.
    Node* makeRoot(const T& value) {
        if (root_ == nullptr) root_ = new Node(value, nullptr);
        return root_;
    }

    // Attach a new child holding `value` under `parent`, set its parent
    // back-pointer, and return it so the caller can hang grandchildren off
    // it. O(1).
    Node* addChild(Node* parent, const T& value) {
        Node* child = new Node(value, parent);
        parent->children.push_back(child);
        return child;
    }

    // Total node count, and height in EDGES (a leaf and an empty tree are
    // both 0). Both recurse over the children.
    std::size_t size()   const { return countFrom(root_); }
    std::size_t height() const { return heightFrom(root_); }

    // Depth-first PRE-ORDER: visit a node, then recurse into its children.
    std::vector<T> preOrder() const {
        std::vector<T> out;
        preOrderInto(root_, out);
        return out;
    }

    // Breadth-first LEVEL-ORDER: closest-first, using the Floor 7 Queue.
    std::vector<T> levelOrder() const {
        std::vector<T> out;
        if (root_ == nullptr) return out;
        Queue<Node*> q;
        q.enqueue(root_);
        while (!q.empty()) {
            Node* cur = q.front();
            q.dequeue();
            out.push_back(cur->data);
            for (std::size_t i = 0; i < cur->children.size(); ++i) {
                q.enqueue(cur->children[i]);
            }
        }
        return out;
    }

    // The lineage of `value`: labels from the root down to the node holding
    // it. findFrom locates the node; the walk UP the parent pointers is the
    // payoff of the back-pointer.
    std::vector<T> lineage(const T& value) const {
        std::vector<T> path;
        const Node* n = findFrom(root_, value);
        if (n == nullptr) return path;          // not found → empty path
        for (const Node* p = n; p != nullptr; p = p->parent) path.push_back(p->data);
        std::reverse(path.begin(), path.end());  // collected leaf→root; flip it
        return path;
    }

private:
    Node* root_ = nullptr;

    // Free an entire subtree: every child first, then the node itself.
    static void destroy(Node* n) {
        if (n == nullptr) return;
        for (std::size_t i = 0; i < n->children.size(); ++i) destroy(n->children[i]);
        delete n;
    }

    // Deep-copy a subtree under a new parent. Mirrors destroy().
    static Node* clone(const Node* n, Node* parent) {
        if (n == nullptr) return nullptr;
        Node* copy = new Node(n->data, parent);
        for (std::size_t i = 0; i < n->children.size(); ++i) {
            copy->children.push_back(clone(n->children[i], copy));
        }
        return copy;
    }

    // Count this node plus every node beneath it.
    static std::size_t countFrom(const Node* n) {
        if (n == nullptr) return 0;
        std::size_t total = 1;
        for (std::size_t i = 0; i < n->children.size(); ++i) total += countFrom(n->children[i]);
        return total;
    }

    // Height (in edges) of the subtree rooted at n: 0 at a leaf, else
    // 1 + the tallest child subtree.
    static std::size_t heightFrom(const Node* n) {
        if (n == nullptr || n->children.size() == 0) return 0;
        std::size_t best = 0;
        for (std::size_t i = 0; i < n->children.size(); ++i) {
            best = std::max(best, heightFrom(n->children[i]));
        }
        return 1 + best;
    }

    // Append this node's data, then each child's subtree in order.
    static void preOrderInto(const Node* n, std::vector<T>& out) {
        if (n == nullptr) return;
        out.push_back(n->data);
        for (std::size_t i = 0; i < n->children.size(); ++i) preOrderInto(n->children[i], out);
    }

    // Recursive search: the first node whose data == value, or nullptr.
    static const Node* findFrom(const Node* n, const T& value) {
        if (n == nullptr) return nullptr;
        if (n->data == value) return n;
        for (std::size_t i = 0; i < n->children.size(); ++i) {
            if (const Node* hit = findFrom(n->children[i], value)) return hit;
        }
        return nullptr;
    }
};

}  // namespace dungeon
