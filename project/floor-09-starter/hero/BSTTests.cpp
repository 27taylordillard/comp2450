// COMP 2450 — Floor 9 starter
// hero/BSTTests.cpp — the `selftest bst` harness. Provided. Do not edit.
//
// Seven phases against one fixed sample BST<int>. The lesson is encoded in
// the order: insert builds a SORTED shape (phase 2), find goes one way
// (phase 3), an in-order walk emerges ascending (phase 4), inserting in
// sorted order degenerates into a stick (phase 5 — the reason Floor 9½
// exists), the Rule of Three you write Wednesday makes a real deep copy
// (phase 6), and remove handles all three cases (phase 7).
//
//         5
//        / \          insert order:       5 3 8 2 4 7 9
//       3   8         in-order (sorted):  2 3 4 5 7 8 9
//      / \ / \        height (edges):     2
//     2  4 7  9

#include "BSTTests.h"

#include <iostream>
#include <vector>

#include "BST.h"

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

// Fill the caller's tree with the fixed sample IN PLACE. Deliberately NOT a
// by-value `BST<int> buildSample()`: returning a BST by value invokes the
// copy ctor -> clone(), which is stubbed until Wednesday, so on any compiler
// that does not elide the copy (notably MSVC /Od Debug, which skips NRVO) the
// sample would come back EMPTY and fail insert/find for the wrong reason.
// Filling in place keeps phases 2-5 and 7 dependent only on the method each
// one tests. Phase 6 exercises clone() explicitly via a real copy.
void fillSample(BST<int>& t) {
    for (int v : {5, 3, 8, 2, 4, 7, 9}) t.insert(v);
}

}  // anonymous namespace

bool runBSTSelfTest() {
    bool all = true;

    // --- Phase 1: empty tree invariants ---
    {
        BST<int> t;
        const bool ok = (t.empty() && t.size() == 0 && t.height() == 0);
        result("empty BST<int>: empty() && size==0 && height==0", ok,
               "size()/height() should already be 0 on an empty tree");
        all = all && ok;
    }

    // --- Phase 2: insert built the sorted structure ---
    {
        BST<int> t;
        fillSample(t);
        bool ok = (!t.empty() && t.root() != nullptr);
        if (ok) ok = (t.root()->data == 5 && t.size() == 7);
        result("insert: root==5 and size==7 over the sample", ok,
               "insertInto is stubbed — it must create a node at the null slot and go left/right by value");
        all = all && ok;
    }

    // --- Phase 3: find goes one way (contains hits and misses) ---
    {
        BST<int> t;
        fillSample(t);
        const bool ok = (t.contains(7) && t.contains(2) && !t.contains(6) && !t.contains(404));
        result("contains: 7 and 2 present, 6 and 404 absent", ok,
               "findFrom must recurse LEFT when value<data, RIGHT when value>data, and stop on equal");
        all = all && ok;
    }

    // --- Phase 4: in-order traversal emerges sorted (the free sort) ---
    {
        BST<int> t;
        fillSample(t);
        const std::vector<int> expected = {2, 3, 4, 5, 7, 8, 9};
        const bool ok = sameVec(t.inOrder(), expected);
        result("inOrder == {2,3,4,5,7,8,9} (ascending)", ok,
               "inOrderInto must recurse LEFT, push the node, then recurse RIGHT");
        all = all && ok;
    }

    // --- Phase 5: sorted insertion degenerates into a stick ---
    {
        BST<int> stick;
        for (int v : {1, 2, 3, 4, 5, 6}) stick.insert(v);   // already sorted
        // Six nodes in a single right-leaning chain: height == 5 edges, not 2.
        const bool ok = (stick.size() == 6 && stick.height() == 5);
        result("degenerate: sorted inserts give height==5, not balanced", ok,
               "if this fails, insertInto isn't placing nodes by the invariant (see phase 2 first)");
        all = all && ok;
    }

    // --- Phase 6: deep copy is independent (Rule of Three) ---
    {
        BST<int> original;
        fillSample(original);
        BST<int> copy = original;          // copy ctor -> your clone()
        original.insert(99);               // mutate ONLY the original
        const std::vector<int> expected = {2, 3, 4, 5, 7, 8, 9};
        bool ok = sameVec(copy.inOrder(), expected);   // copy must be untouched
        if (ok) ok = (copy.size() == 7 && original.size() == 8);
        result("deep copy: mutating the original leaves the copy unchanged", ok,
               "clone() is stubbed — a real deep copy must rebuild every node, not share them");
        all = all && ok;
    }

    // --- Phase 7: remove handles leaf / one-child / two-children ---
    {
        BST<int> t;
        fillSample(t);
        bool ok = !t.remove(404);                      // absent -> false, no change
        ok = ok && t.remove(2)                         // leaf
                && sameVec(t.inOrder(), {3, 4, 5, 7, 8, 9});
        ok = ok && t.remove(3)                         // one child (4) after 2 is gone
                && sameVec(t.inOrder(), {4, 5, 7, 8, 9});
        ok = ok && t.remove(5)                         // two children -> successor 7
                && sameVec(t.inOrder(), {4, 7, 8, 9});
        result("remove: leaf(2), one-child(3), two-children(5) all correct", ok,
               "removeFrom must handle all three cases; two children copies the in-order successor up");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
