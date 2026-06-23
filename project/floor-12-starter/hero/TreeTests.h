// COMP 2450 — Floor 8 starter
// hero/TreeTests.h — the `selftest tree` command. Provided.
//
// Exercises Tree<int> through seven phases against one fixed sample tree.
// Root 1 has children 2, 3, 4; node 2 has children 5 and 6; node 4 has
// child 7. So:
//
//   size()==7, height()==2, preOrder=={1,2,5,6,3,4,7},
// levelOrder=={1,2,3,4,5,6,7}, lineage(6)=={1,2,6}.
//
// Each phase prints OK / FAIL with a one-line diagnostic naming the method
// most likely still stubbed. Phase 1 (empty tree) passes by accident
// before any work, because the stubbed size()/height() return 0 — which
// is the right answer for an empty tree. Your job is to make Phases 2–7
// pass for the right reasons.

#pragma once

namespace dungeon {

// Run the tree self-test. Returns true if every phase passes.
bool runTreeSelfTest();

}  // namespace dungeon
