// COMP 2450 — Floor 8 starter
// hero/TreeTests.cpp — the `selftest tree` harness. Provided. Do not edit.
//
// The deliberate contrast between Phase 5 and Phase 6 is the whole lesson
// of the week encoded as two assertions: pre-order visits a node before
// its siblings' subtrees (depth first); level-order visits whole rows at a
// time (breadth first). Same tree, two orders, and the only difference in
// your code is recursion (pre-order) vs. a Queue (level-order).

#include "TreeTests.h"

#include <iostream>
#include <vector>

#include "Tree.h"

namespace dungeon {

namespace {

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

bool sameVec(const std::vector<int>& a, const std::vector<int>& b) {
    return a == b;
}

// Build the fixed sample tree (see the header diagram). Returns it by
// value; Tree<T>'s provided deep-copy makes that safe.
Tree<int> buildSample() {
    Tree<int> t;
    auto* root = t.makeRoot(1);
    auto* n2 = t.addChild(root, 2);
    t.addChild(root, 3);
    auto* n4 = t.addChild(root, 4);
    // Guard: until addChild is implemented it returns nullptr; adding under
    // a null parent is a no-op in the stub, so this stays crash-safe.
    if (n2 != nullptr) { t.addChild(n2, 5); t.addChild(n2, 6); }
    if (n4 != nullptr) { t.addChild(n4, 7); }
    return t;
}

}  // anonymous namespace

bool runTreeSelfTest() {
    bool all = true;

    // --- Phase 1: empty tree invariants ---
    {
        Tree<int> t;
        const bool ok = (t.empty() && t.size() == 0 && t.height() == 0);
        result("empty Tree<int>: empty() && size==0 && height==0", ok,
               "size()/height() should already be 0 on an empty tree");
        all = all && ok;
    }

    Tree<int> t = buildSample();

    // --- Phase 2: addChild wired the structure up ---
    {
        bool ok = (!t.empty() && t.root() != nullptr);
        if (ok) ok = (t.root()->data == 1 && t.root()->children.size() == 3);
        result("addChild: root==1 with 3 children", ok,
               "addChild is stubbed — it must new a Node and push it onto parent->children");
        all = all && ok;
    }

    // --- Phase 3: size() counts every node (countFrom) ---
    {
        const bool ok = (t.size() == 7);
        result("size()==7 over the sample tree", ok,
               "countFrom does not recurse — return 1 + the count of every child subtree");
        all = all && ok;
    }

    // --- Phase 4: height() in edges (heightFrom) ---
    {
        const bool ok = (t.height() == 2);
        result("height()==2 (longest path 1->2->5)", ok,
               "heightFrom should be 0 at a leaf, else 1 + the tallest child subtree");
        all = all && ok;
    }

    // --- Phase 5: pre-order is depth-first ---
    {
        const std::vector<int> expected = {1, 2, 5, 6, 3, 4, 7};
        const bool ok = sameVec(t.preOrder(), expected);
        result("preOrder == {1,2,5,6,3,4,7} (depth-first)", ok,
               "preOrderInto must push the node BEFORE recursing into its children");
        all = all && ok;
    }

    // --- Phase 6: level-order is breadth-first (uses your Queue) ---
    {
        const std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7};
        const bool ok = sameVec(t.levelOrder(), expected);
        result("levelOrder == {1,2,3,4,5,6,7} (breadth-first)", ok,
               "levelOrder must enqueue the root, then loop dequeue/record/enqueue-children");
        all = all && ok;
    }

    // --- Phase 7: lineage walks root->node via parent pointers ---
    {
        const std::vector<int> expected = {1, 2, 6};
        bool ok = sameVec(t.lineage(6), expected);
        if (ok) ok = t.lineage(404).empty();   // not present → empty path
        result("lineage(6)=={1,2,6} and lineage(404) empty", ok,
               "findFrom must return the matching node (or nullptr); lineage walks parents up");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
