# Floor 4½ — The Chain Vault, Deeper — starter

Read [the public Floor 4½ page](https://hogred.github.io/comp2450/floors/floor-04h/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 4 submission.

## What's new for Floor 4½

- `hero/Chain.h` — `Node` gains a `prev` field; `Chain` gains a `tail_` field. Floor 4's `push_front`, `size`, `head`, `~Chain`, and `clear` are kept. **New TODOs:** `tail()`, `push_back`, `pop_front`, `pop_back`, full Rule of Three (`Chain(const Chain&)` and `operator=`). `swap` and a free `swap(a, b)` are pre-built for copy-and-swap users.
- `hero/Hero.h`, `hero/Hero.cpp` — `printLog` (newest first) is the Floor 4 finished version. `printLogOldest` (oldest first) is stubbed — Monday's job, walks the chain backward via `prev`.
- `hero/ChainTests.cpp` — extended. Phase 1 is the Floor 4 leak check (unchanged). Phase 2 builds a chain, deep-copies it, lets the copy die first, then asserts the original is still walkable in both directions and that total alloc/dealloc balance at `2 × kCount`. **Provided** — you do not edit.
- `main.cpp` — two new commands wired in:
  - `log --oldest [n]` — walks from `tail()` backward via `prev`
  - `clone hero` — copy-constructs a `Hero` in a local scope, prints both event logs, then lets the copy die
- The opening banner is now **THE CHAIN VAULT, DEEPER** and Mavren's greeting reflects the Floor 4½ briefing.

Everything from Floors 0–4 + the Warden midterm is in place: `Bag<T>`, `BagException` + `at()`, `findByName<T>`, `mergeSort`/`quicksort`, every benchmark, `runWardenBattle`, and the working singly-linked `Chain<T>` you finished last week.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new commands work-but-do-nothing-useful-yet:

- `log --oldest` — prints "(printLogOldest not yet implemented)" because Monday's work isn't in.
- `clone hero` — runs, but the cloned event log will appear empty (deep copy ctor is a TODO).
- `selftest chain` Phase 2 — will report `FAIL — backward walk wrong (prev / tail_?); allocation mismatch (deep copy?);` until the right pieces are in.

By Friday, all three should be producing clean output.

## Bringing your own Floor 4 code forward

If you customized `hero/Hero.cpp`'s `printLog` or added extra `eventLog.push_front` calls in `battle/Battle.cpp` or elsewhere, drop your versions over the reference ones. The reference `printLog` here is a compact baseline — your version is almost certainly fine.

If you implemented anything beyond what Floor 4 required on your `Chain.h` (a `size_t length() const` alias, a `print()` method, etc.) — feel free to bring those across, but keep the new `prev`/`tail_` fields and the new method signatures intact, or `ChainTests` and `main.cpp` will fail to compile.

## What you write this week

| Day | File | Work |
|-----|------|------|
| **Mon** | `hero/Chain.h`, `hero/Hero.cpp` | wire `prev`/`tail_` into `push_front`; implement `push_back`, `tail()`; implement `printLogOldest` |
| **Wed** | `hero/Chain.h`                  | deep copy ctor — verify with `selftest chain` Phase 2 |
| **Fri** | `hero/Chain.h`                  | `operator=` (copy-and-swap or explicit), `pop_front`, `pop_back` |

Commit at least three times — Mon, Wed, Fri.

## Lab notes

`floor-04h/lab-notes.md` (in your project repo, not in this starter) — items 1–6 are spelled out on [the Floor 4½ page](https://hogred.github.io/comp2450/floors/floor-04h/).
