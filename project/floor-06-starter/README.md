# Floor 6 — The Hall of Echoes — starter

Read [the public Floor 6 page](https://hogred.github.io/comp2450/floors/floor-06/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 5 submission.

## What's new for Floor 6

- `hero/Stack.h` — `Stack<T>`, a LIFO adapter over `Chain<T>`. **All five method bodies are stubbed.** Your work is here Monday.
- `hero/UndoAction.h` — a tiny struct: `description` + `inventorySnapshot`. What gets pushed onto `hero.undoStack` every time a mutating command runs.
- `hero/Lint.h` and `hero/Lint.cpp` — `bool isBalanced(const std::string&)` checks that `()`, `[]`, and `{}` balance. **Body is stubbed.** Your work, Wednesday.
- `hero/StackTests.h/.cpp` — `selftest stack` harness with five phases. **Provided** (you do not edit).
- `hero/Hero.h` — `Hero` now owns a `Stack<UndoAction> undoStack`.
- `main.cpp` — pre-wired:
  - `take <name>` — adds a small item to your inventory. Records an undo snapshot.
  - `sort inventory by <key>` — same as before, but now records an undo snapshot first.
  - `undo` — body is **stubbed** (prints "not yet implemented"). Your work, Friday.
  - `lint <text>` — calls `isBalanced(text)`, prints `balanced` or `not balanced`.
  - `selftest stack` — runs the StackTests harness.

Everything from Floors 0–5 + the Warden midterm is in place: `Bag<T>`, `BagException` + `at()`, `findByName<T>` via `std::find_if`, `mergeSort`/`quicksort`, every benchmark, `runWardenBattle`, doubly-linked `Chain<T>` with the full Rule of Three, working `iterator` and `const_iterator`, `clone hero`, `selftest chain` / `selftest iterator`.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new commands work-but-do-nothing-useful-yet:

- `selftest stack` — Phase 1 passes (empty-stack invariants — the stubbed `size()` returns 0, so they accidentally agree). Phases 2–5 FAIL with one-line diagnostics that name the likely stubbed method.
- `take Lantern` — the inventory grows by one item; the snapshot push onto `undoStack` is a no-op until Stack::push works. So `undo` won't reverse it yet.
- `lint ({})` — prints "not balanced" because the stub `isBalanced` returns false unconditionally.
- `undo` — prints "(undo not yet implemented — TODO Friday in main.cpp)".

By Friday, all four should be producing real output.

## Bringing your own Floor 5 code forward

If you customized anything on `hero/Chain.h` beyond what Floor 5 required, feel free to bring it across — but keep the `iterator` / `const_iterator` shape intact (so `main.cpp`'s `search` branch keeps compiling), and keep the full Rule of Three intact (so `clone hero` keeps working).

If you modified `hero/Hero.cpp` (printInventory formatting, extra event-log calls in `battle/Battle.cpp`, etc.), drop your versions over the reference ones. Floor 6 does not change those files structurally — only `Hero.h` grew a new field (`undoStack`).

## What you write this week

| Day | File | Work |
|-----|------|------|
| **Mon** | `hero/Stack.h` | Five one-line bodies: `push`, `pop`, `top` (two overloads — same body), `size`, `empty`. After this, `selftest stack` passes every phase. |
| **Wed** | `hero/Lint.cpp` | `isBalanced` body — the classic stack-balancing algorithm using `Stack<char>`. After this, `lint <text>` returns real answers. |
| **Fri** | `main.cpp`     | The `undo` dispatcher. Read the top action, restore its inventory snapshot, pop, print. After this, `take` and `sort` are reversible. |

Commit at least three times — Mon, Wed, Fri.

## Lab notes

`floor-06/lab-notes.md` (in your project repo, not in this starter) — items 1–6 are spelled out on [the Floor 6 page](https://hogred.github.io/comp2450/floors/floor-06/).
