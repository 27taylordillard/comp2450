# Floor 5 — The Eye of Scrying — starter

Read [the public Floor 5 page](https://hogred.github.io/comp2450/floors/floor-05/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 4½ submission.

## What's new for Floor 5

- `hero/Chain.h` — adds two nested types, `iterator` and `const_iterator`, plus `begin`/`end`/`cbegin`/`cend` member functions. The five iterator operators (`*`, `->`, `++`, `--`, `==`) are all TODOs with stub bodies that keep the build green. Reverse iterators (`rbegin`/`rend`/`crbegin`/`crend`) are **pre-built** using `std::reverse_iterator` and start working as soon as your underlying `++`/`--` do. Floor 4½'s doubly-linked storage and full Rule of Three are kept verbatim.
- `hero/Hero.h` — `printLog` is now a **function template**:
  ```cpp
  template <typename It>
  void printLog(It first, It last, std::size_t n,
                const char* label, std::size_t total);
  ```
  The Floor 4½ pair of functions (`printLog` head-first + `printLogOldest` tail-first) is gone — one template now handles both, and `main.cpp` calls it with `begin/end` for newest-first and `rbegin/rend` for oldest-first.
- `hero/IteratorTests.cpp` — new `selftest iterator` harness. Provided. Exercises range-for, `std::find`, `std::distance`, const-ref walks, and `std::reverse` against your iterators, with one-line diagnostics that name the likely culprit (which operator is still stubbed) on each FAIL.
- `main.cpp` — extends the `search` command to ALSO look in the event log via `std::find_if` and a substring-match lambda. Pre-wired here; starts working as soon as your `Chain<std::string>::iterator` does. New `selftest iterator` command added.

Everything from Floors 0–4½ + the Warden midterm is in place: `Bag<T>`, `BagException` + `at()`, `findByName<T>` (still the Floor 3 hand-loop — Wednesday's job to rewrite to one `std::find_if` line), `mergeSort`/`quicksort`, every benchmark, `runWardenBattle`, doubly-linked `Chain<T>` with the full Rule of Three, `clone hero`, `selftest chain`.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new commands work-but-do-nothing-useful-yet:

- `log` and `log --oldest` — print "(the chain is empty — nothing to remember yet)" because the stubbed `operator==` makes `begin() == end()` always true, so the templated `printLog` skips the loop body.
- `search "began session"` — returns "not found" against the event log for the same reason (`std::find_if` short-circuits when its iterators look already-at-end).
- `selftest iterator` — every phase FAILs with a diagnostic that points at the stubbed operator.
- `clone hero` — works (Hero copy works because Floor 4½'s deep copy is intact), but the cloned log prints empty (same iterator-stub reason as `log`).

By Friday, all four should be producing real output.

## Bringing your own Floor 4½ code forward

If you modified `hero/Hero.cpp` (`printInventory` formatting, extra event-log calls in `battle/Battle.cpp`, etc.), drop your versions over the reference ones. Floor 5 does not change those files structurally — only `Hero.cpp`'s old `printLog` / `printLogOldest` pair has been removed, replaced by the template in `Hero.h`.

If you customized anything beyond what Floor 4½ required on your `Chain.h`, feel free to bring it across — but keep the new nested `iterator` / `const_iterator` types and the `begin`/`end`/`cbegin`/`cend` signatures intact, or `main.cpp` and `IteratorTests.cpp` will fail to compile.

## What you write this week

| Day | File | Work |
|-----|------|------|
| **Mon** | `hero/Chain.h` | `iterator::operator*`, `operator->` (already done), `operator++` (pre and post), `operator==`; wire `begin()` / `end()`. After this, `log` and range-for over `eventLog` start working. |
| **Wed** | `hero/Chain.h`, `bestiary/Search.h` | `const_iterator`'s five ops; wire `cbegin()` / `cend()`. Rewrite `findByName<T>` to one `std::find_if` line. After this, `selftest iterator` Phases 1–4 pass. |
| **Fri** | `hero/Chain.h` | `iterator::operator--` and `const_iterator::operator--`. After this, `std::reverse` works, `log --oldest` walks the chain backward via the pre-wired `rbegin()`/`rend()`, and `selftest iterator` Phase 5 passes. |

Commit at least three times — Mon, Wed, Fri.

## Lab notes

`floor-05/lab-notes.md` (in your project repo, not in this starter) — items 1–6 are spelled out on [the Floor 5 page](https://hogred.github.io/comp2450/floors/floor-05/).
