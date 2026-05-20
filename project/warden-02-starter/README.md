# Warden of the Middle Gates — Midterm 2 starter

A **self-contained** starter for *Midterm 2 — The Warden of the Middle Gates*. Drop the contents into a fresh folder and the entire dungeon app builds, with everything the seven floors above were supposed to deliver already in place: `findByName<T>`, `Bag<T>` + `BagException`, doubly-linked `Chain<T>` with iterators (Floor 4½ + 5), `Stack<T>` over `Chain<T>` (Floor 6), `Queue<T>` over `Chain<T>` (Floor 7), the `take`/`sort`/`undo`/`provoke`/`simulate`/`potato` commands, and Warden 1's `runWardenBattle` from Midterm 1. This is the post-Floor-7 reference state.

Read [the public midterm page](https://hogred.github.io/comp2450/floors/warden-02/) before you start typing — it has the full rubric, the AI policy, the schedule, and the four Floor 4–7 ties you must wire into your wave-survival code.

## Two paths to start

**Path A — your own Floor 7 project (preferred).** If your `floor-07/` folder builds and runs (and `selftest queue` passes), work there. The midterm page already tells you to. Drop a copy of `warden-2-trials/` from this starter into your `floor-07/` folder, then add the `battle waves N` command to your existing `main.cpp` (see the snippets below), extend your `hero/UndoAction.h` with a `playerHP` field (see below), and add the `runWaveSurvival` declaration to your `battle/Battle.h`.

**Path B — this starter as a fresh start.** If your Floor 7 isn't building (Queue.h still stubbed, simulate dispatcher unwritten, etc.), copy this whole folder somewhere new and work there. Everything compiles; everything runs; only the wave-survival code (`battle/Battle.cpp`'s `runWaveSurvival`) is yours to write. You won't have your own Floor 4–7 commit history under it, so talk to your instructor about how that affects your project grade.

## What's in the box

```
warden-02-starter/
  main.cpp                        — full post-Floor-7 main, with both the
                                    `battle warden` and `battle waves <N>`
                                    commands already wired. runWaveSurvival
                                    (which you will write) is the stub.
  CMakeLists.txt                  — already includes battle/Battle.cpp.
  battle/
    Battle.h                      — public API for BOTH wardens. Don't
                                    change the contract; add helpers if
                                    you like.
    Battle.cpp                    — runWardenBattle is FILLED (Warden 1
                                    reference). runWaveSurvival is the
                                    heavily-commented TODO.
  bestiary/                       — verbatim from Floor 1.
  hero/
    Item.h, Bag.h, BagException.h — verbatim, all bodies filled.
    Sort.h, Sort.cpp              — Floor 2.
    Chain.h                       — doubly-linked, full Rule of Three,
                                    iterator + const_iterator + reverse
                                    iterators (Floors 4 / 4½ / 5).
    Stack.h                       — adapter over Chain (Floor 6).
    Queue.h                       — adapter over Chain (Floor 7, filled).
    UndoAction.h                  — Floor 6's struct, NOW with an extra
                                    `int playerHP = 0;` field for the
                                    wave-survival undo. Default of 0
                                    keeps `take`/`sort` undo unchanged.
    Hero.h, Hero.cpp              — Hero owns Bag<Item> inventory,
                                    Chain<string> eventLog,
                                    Stack<UndoAction> undoStack,
                                    Queue<string> enemyActionQueue.
                                    printLog() function template here.
    Potato.cpp                    — Floor 7's hotPotato, filled.
    Lint.cpp, *Tests.cpp, *Bench.cpp — verbatim from prior floors.
  data/
    monsters.txt                  — Floor 1 bestiary (15 monsters).
    starter-inventory.txt         — Floor 2 starting inventory.
  warden-2-trials/                — the four take-home artifact templates.
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

You should land at the `> ` prompt with every previous-floor command working. Try `battle warden` — Midterm 1's reference encounter plays through. Try `battle waves 2` — you'll get the placeholder message from the unwritten `runWaveSurvival` stub.

## What you actually write — Trial III

Open [`battle/Battle.cpp`](battle/Battle.cpp). The TODO at the top of `runWaveSurvival` is the work. The required Floor 4–7 ties are listed at the top of that function as a checklist — every one must end up real and load-bearing in your code, not pasted in for show.

You may decompose `runWaveSurvival` however you want — `buildWave`, `printWaveMenu`, `readWaveChoice`, and `recordBattleUndo` are the four natural extractions. The only contract `main.cpp` depends on is the public API in `Battle.h` (`BattleOutcome runWaveSurvival(Hero&, int)`). Internal organisation is yours.

### If you're working on Path A (your own Floor 7 project)

You'll need to merge in `warden-2-trials/` and the wiring. The diffs are small.

**`hero/UndoAction.h` — add one field:**

```cpp
struct UndoAction {
    std::string description;
    Bag<Item>   inventorySnapshot;
    int         playerHP = 0;   // <-- new; default 0 leaves Floor 6 callers unaffected
};
```

**`battle/Battle.h` — add the declaration:**

```cpp
BattleOutcome runWaveSurvival(Hero& hero, int waves);
```

**`main.cpp` — extend the `battle` command dispatch to also handle `waves`:**

```cpp
else if (cmd == "battle") {
    std::istringstream bs(rest);
    std::string which;
    bs >> which;
    if (which == "warden") {
        BattleOutcome outcome = runWardenBattle(hero);
        switch (outcome) { /* ...existing Warden 1 banners... */ }
    }
    else if (which == "waves") {
        int n = 0;
        if (!(bs >> n) || n < 1) {
            std::cout << "Usage: battle waves <N>   (N >= 1)\n";
            continue;
        }
        BattleOutcome outcome = runWaveSurvival(hero, n);
        switch (outcome) {
            case BattleOutcome::Victory:
                std::cout << "\nThe yard quiets. The inner gate opens.\n"; break;
            case BattleOutcome::Defeat:
                std::cout << "\nYou fall in the yard. The gate stays shut.\n"; break;
            case BattleOutcome::Fled:
                std::cout << "\nYou step back from the yard. Uncleared waves remain.\n"; break;
        }
    }
    else {
        std::cout << "Usage: battle warden   |   battle waves <N>\n";
    }
}
```

**`main.cpp` — slot one line into `printHelp()`:**

```cpp
              << "   battle waves <N>              — face the Warden of the Middle Gates (N waves)\n"
```

`CMakeLists.txt` doesn't need to change — `battle/Battle.cpp` is already in your executable from Warden 1.

## What you also write — Trials I & II + notes

Inside [`warden-2-trials/`](warden-2-trials/) there are four template files. Open each, replace the `(your answer)` and `(paste …)` placeholders with your work, and commit them with the rest of your submission.

| File | What it is |
|------|------------|
| `AI-USE.md`           | Per-trial declaration: which tool, what you asked for, what you did with the result. |
| `trial-1-brief.md`    | Trial I — four short answers (50–100 words each), one for each floor span (LL, iterators, stacks, queues). |
| `trial-2-critique.md` | Trial II — engage an LLM with the `Deque<T>` prompt, paste prompt + reply, write a ≥150-word critique. |
| `encounter-notes.md`  | Trial III notes — AI declaration, **two functions you wrote without AI**, brief reflection, ties checklist. |

## Submission

Commit and push **the whole thing** — your modified `main.cpp`, `hero/UndoAction.h`, `battle/Battle.{h,cpp}`, and your `warden-2-trials/` folder — to your project repo by **9:00 AM Friday of Week 10**. The Friday in-class quiz is during that day's class period. Bring a pen.
