# Warden of the Foundations — Midterm 1 starter

This folder is a **delta pack** for Midterm 1 — *The Warden of the Foundations*. It contains everything new that Midterm 1 introduces: a `battle/` skeleton for the boss-battle code, four `warden-trials/` templates for the take-home artifacts, and the small `main.cpp` and `CMakeLists.txt` additions that wire the new command into your existing dungeon app.

Read [the public midterm page](https://hogred.github.io/comp2450/floors/warden-01/) before you start typing — it has the full rubric, the AI policy, the schedule, and the four Floor 0–3 ties you must wire into your battle code. **This file is a placement guide, not a substitute for that page.**

## Where this code lives

You drop this folder's contents into your existing **Floor 3 project** — the working dungeon app you've been building all term. After dropping it in, your tree looks like this:

```
[your project repo]/
  floor-03/
    main.cpp                       <-- you'll add a `battle warden` command here
    CMakeLists.txt                 <-- you'll add 2 lines here
    battle/
      Battle.h                     <-- skeleton (from this starter)
      Battle.cpp                   <-- skeleton (from this starter)
    bestiary/                      <-- already there, unchanged
    hero/                          <-- already there (Bag.h, BagException.h, etc.)
    data/                          <-- already there
    warden-trials/                 <-- the four take-home artifacts
      AI-USE.md                    <-- declaration (from this starter — fill in)
      trial-1-brief.md             <-- Trial I template (from this starter — fill in)
      trial-2-critique.md          <-- Trial II template (from this starter — fill in)
      encounter-notes.md           <-- Trial III notes template (from this starter — fill in)
```

If your Floor 3 project isn't compiling, talk to your instructor before midterm week starts — *not* the night before submission is due. Trial III requires a working `Bag<Item>`, `findByName<T>`, and `BagException`, and you need them in working order to integrate the battle.

## Wiring the `battle warden` command

### Step 1 — Add the include in `main.cpp`

Up where the other `#include` lines are, add:

```cpp
#include "battle/Battle.h"
```

### Step 2 — Add a branch in the command loop

In `main.cpp`, inside the big `if (cmd == "...") ... else if ...` chain, add a new branch — anywhere in the chain works, but right before the final `else` block reads naturally:

```cpp
else if (cmd == "battle") {
    // The Warden midterm: the only valid argument right now is "warden".
    // (You may add more bosses on later floors. Don't this week.)
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

### Step 3 — Add `battle warden` to `printHelp()`

Inside `printHelp()` in `main.cpp`, slot one more line into the command list — somewhere reasonable, e.g., right above `help`:

```cpp
              << "   battle warden                 — face the Warden of the Foundations\n"
```

### Step 4 — Add `battle/` to your build

Open `CMakeLists.txt` and add `battle/Battle.cpp` to the `add_executable(...)` list:

```cmake
add_executable(the_descent
    main.cpp
    battle/Battle.cpp           # <-- new
    bestiary/Bestiary.cpp
    bestiary/Search.cpp
    bestiary/Bench.cpp
    hero/Hero.cpp
    hero/Sort.cpp
    hero/Bench.cpp
)
```

### Step 5 — Confirm it builds

```
cmake -B build
cmake --build build
```

You should see your existing executable rebuild. Run it; type `battle warden`. The skeleton in `Battle.cpp` prints a placeholder message and returns `Fled`. From here, the work is yours.

## What you actually write — Trial III

Open [`battle/Battle.cpp`](battle/Battle.cpp). The TODO at the top of `runWardenBattle` is the work. The required Floor 0–3 ties are listed at the top of the file as a checklist — every one of them must end up real and load-bearing in your code, not pasted in for show.

You may decompose `runWardenBattle` however you want — extract helpers, add private types, restructure the loop. The grader cares about the public API (`BattleOutcome runWardenBattle(Hero&)`) and the four Floor ties. Internal organization is yours.

## What you also write — Trials I & II + notes

Inside [`warden-trials/`](warden-trials/) there are four template files. Open each, replace the `(your answer)` and `(paste ...)` placeholders with your work, and commit them with the rest of your submission.

| File | What it is |
|------|------------|
| `AI-USE.md`         | Per-trial declaration: which tool, what you asked for, what you did with the result. |
| `trial-1-brief.md`  | Trial I — four short answers (50–100 words each), one per floor. |
| `trial-2-critique.md` | Trial II — engage an LLM, paste prompt + reply, write a ≥150-word critique. |
| `encounter-notes.md` | Trial III notes — AI declaration, **two functions you wrote without AI**, brief reflection, ties checklist. |

## Submission

Commit and push **the whole thing** — your modified `main.cpp`, `CMakeLists.txt`, your `battle/` folder, and your `warden-trials/` folder — to your project repo by **9:00 AM Friday of Week 5**.

The Friday in-class quiz is during that day's class period. Bring a pen.
