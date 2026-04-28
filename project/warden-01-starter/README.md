# Warden of the Foundations — Midterm 1 starter

A **self-contained** starter for *Midterm 1 — The Warden of the Foundations*. Drop the contents into a fresh folder and the entire dungeon app builds, with everything the four floors above were supposed to deliver already in place: function template `findByName<T>`, class template `Bag<T>`, `BagException` + `at()`, the try/catch around the command loop. This is the post-Floor-3 reference state.

Read [the public midterm page](https://hogred.github.io/comp2450/floors/warden-01/) before you start typing — it has the full rubric, the AI policy, the schedule, and the four Floor 0–3 ties you must wire into your battle code.

## Two paths to start

**Path A — your own Floor 3 project (preferred).** If your `floor-03/` folder builds and runs, work there. The midterm page already tells you to. Drop a copy of `battle/` and `warden-trials/` from this starter into your `floor-03/` folder, then add the `battle warden` command to your existing `main.cpp` (see the snippets below) and the `battle/Battle.cpp` line to your `CMakeLists.txt`.

**Path B — this starter as a fresh start.** If your Floor 3 isn't building, copy this whole folder somewhere new and work there. Everything compiles; everything runs; only the boss-battle code (`battle/Battle.cpp`'s `runWardenBattle`) is yours to write. You won't have your own Floor 1–3 commit history under it, so talk to your instructor about how that affects your project grade.

## What's in the box

```
warden-01-starter/
  main.cpp                        — full post-Floor-3 main, with the
                                    `battle warden` command already wired
                                    to runWardenBattle (which you will
                                    write).
  CMakeLists.txt                  — already includes battle/Battle.cpp.
  battle/
    Battle.h                      — public API. Don't change the contract;
                                    add helpers if you like.
    Battle.cpp                    — heavily-commented skeleton. The TODO
                                    in runWardenBattle is the work.
  bestiary/
    Monster.h                     — verbatim from Floor 1.
    Bestiary.h, Bestiary.cpp      — bestiary stored in Bag<Monster>.
    Search.h                      — Floor 1 reference searches kept for
                                    the bench; findByName<T> takes
                                    const Bag<T>&.
    Search.cpp                    — verbatim Floor 1 references.
    Bench.h, Bench.cpp            — bestiary search benchmark.
  hero/
    Item.h                        — verbatim from Floor 2.
    Hero.h, Hero.cpp              — inventory is now Bag<Item>.
    Bag.h                         — class template, all bodies filled in.
    BagException.h                — full implementation (Friday's work).
    Sort.h, Sort.cpp              — mergeSort/quicksort over Bag<Item>.
    Bench.h, Bench.cpp            — sort benchmark over Bag<Item>.
  data/
    monsters.txt                  — Floor 1 bestiary (15 monsters).
    starter-inventory.txt         — Floor 2 starting inventory.
  warden-trials/                  — the four take-home artifact templates.
    AI-USE.md
    trial-1-brief.md
    trial-2-critique.md
    encounter-notes.md
```

## Build & run

```
cmake -B build            # configure once (and any time CMakeLists.txt changes)
cmake --build build       # compile
./build/the_descent       # run (macOS / Linux)
build\the_descent.exe     # run (Windows cmd.exe)
```

You should land at the `> ` prompt with all the previous-floor commands working — `search`, `list`, `inventory`, `inspect`, `sort`, `benchmark`. Try `inspect 99` and confirm the game prints a clean error message and keeps running (that's the Floor 3 try/catch). Type `battle warden` and you'll get the placeholder message from the unwritten `runWardenBattle` skeleton.

## What you actually write — Trial III

Open [`battle/Battle.cpp`](battle/Battle.cpp). The TODO at the top of `runWardenBattle` is the work. The required Floor 0–3 ties are listed at the top of the file as a checklist — every one must end up real and load-bearing in your code, not pasted in for show.

You may decompose `runWardenBattle` however you want — extract helpers, add private types, restructure the loop. The only contract `main.cpp` depends on is the public API in `Battle.h` (`BattleOutcome runWardenBattle(Hero&)`). Internal organisation is yours.

### If you're working on Path A (your own Floor 3 project)

You'll need to merge in `battle/`, `warden-trials/`, and the wiring. The diffs are small.

**`main.cpp` — add the include:**

```cpp
#include "battle/Battle.h"
```

**`main.cpp` — add a branch in the command dispatch:**

```cpp
else if (cmd == "battle") {
    if (rest != "warden") {
        std::cout << "Usage: battle warden\n";
        continue;
    }
    BattleOutcome outcome = runWardenBattle(hero);
    switch (outcome) {
        case BattleOutcome::Victory:
            std::cout << "\nThe Warden falls. The gate opens.\n"; break;
        case BattleOutcome::Defeat:
            std::cout << "\nYou fall. The gate stays shut.\n"; break;
        case BattleOutcome::Fled:
            std::cout << "\nYou step back from the gate. It does not chase.\n"; break;
    }
}
```

**`main.cpp` — slot one line into `printHelp()`:**

```cpp
              << "   battle warden                 — face the Warden of the Foundations\n"
```

**`CMakeLists.txt` — add `battle/Battle.cpp` to the executable:**

```cmake
add_executable(the_descent
    main.cpp
    battle/Battle.cpp           # <-- new
    bestiary/Bestiary.cpp
    ...
)
```

## What you also write — Trials I & II + notes

Inside [`warden-trials/`](warden-trials/) there are four template files. Open each, replace the `(your answer)` and `(paste …)` placeholders with your work, and commit them with the rest of your submission.

| File | What it is |
|------|------------|
| `AI-USE.md`           | Per-trial declaration: which tool, what you asked for, what you did with the result. |
| `trial-1-brief.md`    | Trial I — four short answers (50–100 words each), one per floor. |
| `trial-2-critique.md` | Trial II — engage an LLM, paste prompt + reply, write a ≥150-word critique. |
| `encounter-notes.md`  | Trial III notes — AI declaration, **two functions you wrote without AI**, brief reflection, ties checklist. |

## Submission

Commit and push **the whole thing** — your modified `main.cpp`, `CMakeLists.txt`, your `battle/` folder, and your `warden-trials/` folder — to your project repo by **9:00 AM Friday of Week 5**. The Friday in-class quiz is during that day's class period. Bring a pen.
