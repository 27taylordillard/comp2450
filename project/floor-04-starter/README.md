# Floor 4 — The Chain Vault — starter

Read [the public Floor 4 page](https://hogred.github.io/comp2450/floors/floor-04/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Warden submission.

## What's new for Floor 4

- `hero/Chain.h` — singly-linked container template. **All bodies are stubbed.** Your work is here.
- `hero/Hero.h` — `Hero` now owns a `Chain<std::string> eventLog`.
- `hero/Hero.cpp` — `printLog` is stubbed and prints a placeholder. Your work, Monday.
- `main.cpp` — every command branch already calls `hero.eventLog.push_front(...)`. Three new commands wired in:
  - `log [n]` — show the last `n` entries, newest first (default 10)
  - `selftest chain` — leak-check harness
  - `benchmark log [N]` — `Chain::push_front` vs `vector::insert(begin())`
- `hero/ChainTests.cpp` — leak-check harness, **provided** (you do not edit).
- `hero/ChainBench.cpp` — prepend benchmark, **provided** (you do not edit).

Everything from Floors 0–3 + the Warden midterm is in place: `Bag<T>`, `BagException` + `at()`, `findByName<T>`, `mergeSort`/`quicksort`, every benchmark, and a working `runWardenBattle` reference implementation.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working — `search`, `list`, `inventory`, `inspect`, `sort`, `benchmark`, `battle warden`. The new commands work-but-do-nothing-yet:

- `log` — prints "(printLog not yet implemented)" because Monday's work isn't in.
- `selftest chain` — reports 0/0/0 because `push_front` is a no-op stub.
- `benchmark log 100` — runs and reports timings, but the chain side is racing to do nothing.

By Friday, all three should be producing real output.

## Bringing your own Warden battle forward

If you completed Trial III with your own `battle/Battle.cpp`, drop your file over the reference one in this starter. The reference is a compact baseline — your version is almost certainly better tuned. Just make sure you also push events to `hero.eventLog` in the spots you care about, so `log` after a battle gives you a replay.

The reference battle's event-log calls (search for `hero.eventLog.push_front`) are a reasonable template.

## What you write this week

| Day | File | Work |
|-----|------|------|
| **Mon** | `hero/Chain.h`, `hero/Hero.cpp` | `push_front`, `size`, `head()`; then `printLog` |
| **Wed** | `hero/Chain.h`                  | `~Chain()` and `clear()` — verify with `selftest chain` |
| **Fri** | `hero/Chain.h`                  | `= delete` the copy operations; capture the compiler error for the lab |

Commit at least three times — Mon, Wed, Fri.

## Lab notes

`floor-04/lab-notes.md` (in your project repo, not in this starter) — items 1–5 are spelled out on [the Floor 4 page](https://hogred.github.io/comp2450/floors/floor-04/).
