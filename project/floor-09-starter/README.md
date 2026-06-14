# Floor 9 — The Halving Hall — starter

Read [the public Floor 9 page](https://hogred.github.io/comp2450/floors/floor-09/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 8 submission.

## What's new for Floor 9

One rule changes the tree this week. Floor 8's `Tree<T>` gave a node any number of children with no order among them — so finding a beast meant scanning every branch. A **binary search tree** gives a node at most two children, `left` and `right`, with an iron rule: **left is smaller, right is larger.** That ordering turns search from a scan into a descent: compare once, go one way, discard half the tree. Scrivener McCown has rebuilt his Bestiary as a `BST<Monster>` and wants it both sorted *and* fast to query.

- `hero/BST.h` — `BST<T>`, a binary search tree. **Six helper bodies are stubbed** (see the table below). The `Node` (just `data`, `left`, `right`), the access methods, `size`/`height`, and the public delegators (`insert`, `find`, `contains`, `inOrder`, `remove`) are **provided**. Two of the six you write — `destroy` (the destructor) and `clone` (the deep-copy ctor) — are the recursive **Rule of Three** you only *read* on Floor 8. A BST owns its nodes the same way; now you write the cleanup.
- `hero/BSTTests.h/.cpp` — `selftest bst`, seven phases against one fixed sample `BST<int>`. **Provided** (you don't edit). Phase 5 inserts `{1,2,3,4,5,6}` already sorted and asserts the height is **5, not 2** — degeneration, encoded as a green test and a preview of Floor 9½.
- The bestiary, now also built as a `BST<Monster>` keyed by name. The `Bag<Monster>` bestiary you've always had still works (`list`, `search`); the BST sits **alongside** it. New commands:
  - `catalogue` — print the bestiary in name order (the BST's in-order traversal).
  - `recall <name>` — look a monster up fast in the BST (O(log n), not a scan).
  - `forget <name>` — remove a monster from the BST.
  - `benchmark bst [N]` — race a linear scan against BST lookup over a synthetic bestiary.
  - `selftest bst` — runs the BSTTests harness.

Everything from Floors 0–8 is in place and **working** — `Bag`, `Chain`, the iterators, `Stack`, `Queue`, `hotPotato`, `simulate`, `lint`, `undo`, the general `Tree<T>` with `taxonomy`/`lineage`, every benchmark and self-test. Floor 8's stubs are now reference — including the `Tree` destructor and deep-copy whose BST versions you're about to write.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new commands work-but-do-nothing-useful-yet:

- `selftest bst` — Phase 1 passes (empty-tree invariants — the stubbed `size()`/`height()` return 0, which is the right answer for an empty tree). Phases 2–7 FAIL with one-line diagnostics that name the likely stubbed helper.
- `catalogue` — prints `(the tree is empty — is BST<T>::insert written yet?)` because `insert` is a no-op until Monday.
- `recall <name>` — reports "not in McCown's catalogue" for everything (`find` is stubbed to return `nullptr`).
- `benchmark bst` — runs, but the BST column shows `tree height 0` and instant lookups, because the tree is empty. That zero height is your tell that `insert` isn't built yet.

By Friday, all of these produce real output.

## What you write this week

| Day | Helpers in `hero/BST.h` | Lights up |
|-----|-------------------------|-----------|
| **Mon** | `insertInto`, `findFrom` | `recall <name>` works; `selftest bst` phases 2, 3, 5 pass. The tree is built — `catalogue` lists it after Wednesday's in-order walk. |
| **Wed** | `inOrderInto`, `destroy`, `clone` | `catalogue` prints **alphabetical**; phases 4 and 6 (the deep-copy/no-leak check) pass. `destroy` + `clone` are the Rule of Three you read on Floor 8. |
| **Fri** | `removeFrom` | `forget <name>` works; phase 7 passes. The two-children case copies the in-order successor up — the hardest method of the semester so far. |

The recurring shape for `insertInto`, `findFrom`, `inOrderInto`, `destroy`, and `clone` is the same recursion you've now written many times: **base case for `nullptr`, act on this node, recurse into `left` and/or `right`.** `removeFrom` is the exception — three cases, and the hard one reduces to an easy one.

Commit at least three times — Mon, Wed, Fri.

## Bringing your own Floor 8 code forward

If you customized `hero/Tree.h`, `hero/Taxonomy.*`, or `data/taxonomy.txt`, drop your versions over the reference ones shipped here — but the references are simple and correct, so only swap in your own if there's a reason. Same for everything earlier. Floor 9 does not change those files structurally; it only adds new ones (`hero/BST.h`, `hero/BSTTests.*`) and a few commands in `main.cpp`.

## Lab notes

`floor-09/lab-notes.md` (in your project repo, not in this starter) — the lab items are spelled out on [the Floor 9 page](https://hogred.github.io/comp2450/floors/floor-09/).
