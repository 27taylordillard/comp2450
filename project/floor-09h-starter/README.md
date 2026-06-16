# Floor 9½ — The Balanced Hall — starter

Read [the public Floor 9½ page](https://hogred.github.io/comp2450/floors/floor-09h/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 9 submission.

## What's new for Floor 9½

You do not build a new tree this week — you **balance** the one you already have. Floor 9 ended on a catch you proved yourself: insert values in sorted order and your `BST<T>` degenerates into a **stick** — height *N-1*, lookup back to *O(n)*. The rule (`left < node < right`) was correct; it just wasn't *helping*. This week you learn to **measure** that imbalance, you learn the one structural move that **repairs** it — a *rotation* — and then you watch `std::set` shrug off the very input that flattened your tree. The lesson is *compare, don't reinvent*.

All the new work is in `hero/BST.h`, which now ships as your **complete Floor 9 BST** (insert/find/inOrder/remove + the Rule of Three — all reference code) plus **five stubbed helpers**:

- `balanceFactorOf`, `isBalancedFrom` — **Monday.** Measure balance. `balanceFactor()` is `signedHeight(left) - signedHeight(right)`; `isBalanced()` is true iff *every* node's balance factor is within `[-1, 1]` (the AVL invariant). `signedHeight()` is **provided** — it's like `height()` but an empty subtree counts as `-1`, one less than a leaf. That `-1` is the whole reason a three-node stick reads as unbalanced; if you reach for `height()` instead, a stick wrongly passes.
- `rotateRight`, `rotateLeft` — **Wednesday.** The repair primitive (ZyBook §10.2). Four lines of pointer surgery that lift a child up and push the node down. The key property: a rotation does **not** change the in-order order, which is exactly why it can never break the BST.
- `rebalanceAt` — **Friday.** The decision: if a node is left-heavy, rotate right; if right-heavy, rotate left. `balanceRoot()` applies it once at the root.

Also provided (you don't edit these):

- `hero/BalanceTests.h/.cpp` — `selftest balance`, **five phases** over `BST<int>`.
- `balance` — a command that reports the bestiary tree's height and root balance factor, and whether it's balanced.
- `benchmark balance [N]` — the payoff: the **same sorted input** built into your plain `BST` (a stick) and a `std::set` (balanced). It does **not** use this week's stubs, so it works from day one — it's the "compare" half of the floor.

Everything from Floors 0–9 is in place and **working** — including the entire Floor 9 BST. Floor 9's stubs are now reference code.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new balance work does-nothing-useful-yet:

- `selftest balance` — **Phase 2 passes** (a bushy tree really is balanced, and the stub claims *everything* is balanced, so it agrees by accident). **Phases 1, 3, 4, 5 FAIL** with one-line diagnostics naming the likely stubbed helper.
- `balance` — reports the catalogue tree as "balanced" no matter what, because `isBalanced` is stubbed to return `true`. After Monday it tells the truth.
- `benchmark balance` — **already works** (it doesn't depend on your stubs): it shows your plain BST as a tall stick next to a shallow `std::set`. This is the comparison the whole floor is built around.

By Friday the first three all produce real output.

## What you write this week

| Day | Helpers in `hero/BST.h` | Lights up |
|-----|-------------------------|-----------|
| **Mon** | `balanceFactorOf`, `isBalancedFrom` | `selftest balance` phases 1–3; `balance` now correctly calls the sorted stick **not balanced**. |
| **Wed** | `rotateRight`, `rotateLeft` | phase 4 — a rotation repairs structure **and** leaves the in-order order unchanged. |
| **Fri** | `rebalanceAt` | phase 5 — `balanceRoot` picks the correct single rotation. Then `benchmark balance` lands the lesson: you don't hand-roll balancing, you reach for `std::set`/`std::map` (next floor). |

The recurring shapes are familiar: `isBalancedFrom` is the same "base case, check this node, recurse both sides" recursion you've written all stretch; the rotations are pure pointer surgery you draw on the board first.

Commit at least three times — Mon, Wed, Fri.

## Bringing your own Floor 9 code forward

If you customized `hero/BST.h` on Floor 9, **bring your six helper bodies across** — this starter ships the reference versions, so drop your own `insertInto`/`findFrom`/`inOrderInto`/`destroy`/`clone`/`removeFrom` over them if you'd rather build on your code. The five new balance helpers are stubbed regardless. Everything earlier (Floors 0–8) is unchanged; swap in your versions only if you customized them.

## Lab notes

`floor-09h/lab-notes.md` (in your project repo, not in this starter) — the lab items are spelled out on [the Floor 9½ page](https://hogred.github.io/comp2450/floors/floor-09h/).
