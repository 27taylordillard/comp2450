# Floor 8 — The Hall of Lineages — starter

Read [the public Floor 8 page](https://hogred.github.io/comp2450/floors/floor-08/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 7 submission.

## What's new for Floor 8

The shape changes this week. A `Stack` and a `Queue` were a `Chain<T>` you only touched at the ends — still a line. A **tree** gives every node a whole `Bag` of children: a branching structure. Scrivener McCown has rebuilt his Bestiary as a *classification tree*, and your job is to build the container that holds it and the walks that query it.

- `hero/Tree.h` — `Tree<T>`, a general (n-ary) tree. **Six method bodies are stubbed** (see the table below). The `Node` struct, the root accessors, `makeRoot`, and the **recursive Rule of Three** (deep-copy ctor, copy-assign, destructor) are **provided** — read them first; your methods recurse over children the same way they do.
- `hero/TreeTests.h/.cpp` — `selftest tree` harness with seven phases against one fixed sample tree. **Provided** (you do not edit). Phase 5 vs. Phase 6 is the depth-first-vs-breadth-first assertion encoded as two expected vectors.
- `hero/Taxonomy.h/.cpp` — `loadTaxonomy()` reads `data/taxonomy.txt` (one root-to-leaf path per line) and builds a `Tree<std::string>`; `printTaxonomy()` prints it as an indented outline. **Provided framework.** `printTaxonomy`'s recursion is a worked example for the one you write in `preOrderInto`.
- `data/taxonomy.txt` — McCown's classification of the bestiary. Editable; the tree's shape is just the set of paths in this file.
- `main.cpp` — pre-wired commands:
  - `taxonomy` — prints the indented outline (works once `addChild` builds the tree).
  - `taxonomy --dfs` — lists every entry in depth-first pre-order (uses your `preOrder()`).
  - `taxonomy --bfs` — lists every entry in breadth-first level-order (uses your `levelOrder()`).
  - `lineage <name>` — prints the root-to-creature path (uses your `lineage()`).
  - `selftest tree` — runs the TreeTests harness.

Everything from Floors 0–7 is in place and **working** — `Bag`, `Chain`, the iterators, `Stack`, `Queue`, `hotPotato`, `simulate`, `lint`, `undo`, every benchmark and self-test. Floor 7's stubs are now reference.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new commands work-but-do-nothing-useful-yet:

- `selftest tree` — Phase 1 passes (empty-tree invariants — the stubbed `size()`/`height()` return 0, which is the right answer for an empty tree). Phases 2–7 FAIL with one-line diagnostics that name the likely stubbed method.
- `taxonomy` — prints just the root line (`Creatures of the Hold`) because `addChild` is a no-op until Monday, so the loader cannot hang any categories off it.
- `taxonomy --dfs` / `--bfs` — print empty lists (the traversals are stubbed).
- `lineage Lich` — prints "not in McCown's catalogue" because `findFrom` is stubbed to return `nullptr` for everything.

By Friday, all of these should be producing real output.

## What you write this week

| Day | Methods in `hero/Tree.h` | Lights up |
|-----|--------------------------|-----------|
| **Mon** | `addChild`, `countFrom` (→ `size`), `heightFrom` (→ `height`) | `taxonomy` prints the full outline; `selftest tree` phases 2–4 pass. |
| **Wed** | `preOrderInto` (→ `preOrder`), `levelOrder` | `taxonomy --dfs` and `--bfs` work; phases 5–6 pass. **`levelOrder` reuses your Floor 7 `Queue<T>`** — that is the promise Floor 7 made coming due. |
| **Fri** | `findFrom` (→ `lineage`) | `lineage <name>` works; phase 7 passes. |

The recurring shape for the recursive methods (`countFrom`, `heightFrom`, `preOrderInto`, `findFrom`) is: **handle the `nullptr` base case, act on this node, then recurse into each child.** `levelOrder` is the deliberate exception — it is *not* recursive; it uses a queue.

Commit at least three times — Mon, Wed, Fri.

## Bringing your own Floor 7 code forward

If you customized `hero/Queue.h`, `hero/Potato.cpp`, or the `simulate` dispatcher in `main.cpp`, drop your versions over the reference ones shipped here — but the references are simple and correct, so only swap in your own if there's a reason. Same for everything earlier: keep your `Chain<T>`, `Bag<T>`, `Stack<T>`, and `Battle.cpp` if you prefer. Floor 8 does not change those files structurally; it only adds new ones.

## Lab notes

`floor-08/lab-notes.md` (in your project repo, not in this starter) — the lab items are spelled out on [the Floor 8 page](https://hogred.github.io/comp2450/floors/floor-08/).
