# Warden of the Trees — Midterm 3 starter

A **self-contained** starter for *Midterm 3 — The Warden of the Trees*. Drop the contents
into a fresh folder and the entire dungeon app builds, with everything the eleven floors
above were supposed to deliver already in place and **working**: `Bag<T>` + `BagException`,
the sorts, doubly-linked `Chain<T>` with iterators, `Stack<T>` and `Queue<T>`, the general
`Tree<T>` (`taxonomy`/`lineage`), the full Floor 9 `BST<T>` + Floor 9½ balancing
(`catalogue`/`recall`/`balance`), the Floor 10 `Registry` (`std::set`/`std::map`), the
Floor 11 `HashTable` (`whisper`/`banish`/`load`), every `selftest`, and Warden 1's
`runWardenBattle`. This is the **post-Floor-11** reference state.

Read [the public midterm page](https://hogred.github.io/comp2450/floors/warden-03/) before
you start typing — it has the full rubric, the AI policy, the schedule, and the four
Floor 8–11 ties you must wire into your Tree-warden code.

## Two paths to start

**Path A — your own Floor 11 project (preferred).** If your `floor-11/` folder builds and
runs (and `selftest whispers` passes), work there. The midterm page already tells you to.
The wiring is **smaller than Warden 2's** — see the diffs below.

**Path B — this starter as a fresh start.** If your Floor 11 isn't building (HashTable still
stubbed, etc.), copy this whole folder somewhere new and work there. Everything compiles;
everything runs; only the Tree-warden code (`battle/Battle.cpp`'s `runTreeWarden`) is yours
to write. You won't have your own Floor 8–11 commit history under it, so talk to your
instructor about how that affects your project grade.

## What's in the box

```
warden-03-starter/
  main.cpp                        — full post-Floor-11 main, with both the
                                    `battle warden` and `battle trees` commands
                                    already wired. runTreeWarden (which you
                                    will write) is the stub.
  CMakeLists.txt                  — already includes battle/Battle.cpp.
  battle/
    Battle.h                      — public API for BOTH wardens + BattleException.
                                    Don't change the contract; add helpers if
                                    you like.
    Battle.cpp                    — runWardenBattle is FILLED (Warden 1
                                    reference). runTreeWarden is the
                                    heavily-commented TODO — the four required
                                    ties, the grader's 3-phase self-check, and a
                                    suggested outline are all in the comment
                                    block above the stub.
  bestiary/                       — Monster, Bestiary, Search, Bench, and the
                                    FINISHED Registry (std::set/std::map).
  hero/                           — Bag, Chain (with eraseFirst), Stack, Queue,
                                    Tree, BST + balance, the FINISHED HashTable,
                                    Hero (printLog), plus every *Tests / *Bench.
  data/
    monsters.txt                  — the 15-monster bestiary.
    taxonomy.txt                  — McCown's classification tree (Goblin absent).
    starter-inventory.txt         — starting inventory.
  warden-3-trials/                — the four take-home artifact templates.
    AI-USE.md
    trial-1-brief.md
    trial-2-critique.md
    encounter-notes.md
```

## Build & run

```
cmake -B build            # configure once (and any time CMakeLists.txt changes)
cmake --build build       # compile
build\the_descent.exe     # run (Windows cmd.exe)
./build/the_descent       # run (macOS / Linux)
```

You should land at the `> ` prompt with every previous-floor command working. Try
`battle warden` — Midterm 1's reference encounter plays through. Try `battle trees` — you'll
get the placeholder message from the unwritten `runTreeWarden` stub.

## What you actually write — Trial III

Open [`battle/Battle.cpp`](battle/Battle.cpp). The TODO at the top of `runTreeWarden` is the
work. The four required Floor 8–11 ties are listed at the top of that function as a checklist
— every one must end up **real and load-bearing** in your code, not pasted in for show:

- **[F8]** `taxonomy.levelOrder()` marshals the menagerie (leaves only); `taxonomy.lineage()`
  exposes the Warden.
- **[F9 + F9½]** `catalogue.find(probe)` is the slow Recall; `isBalanced()`/`height()`/
  `balanceFactor()` gate the lean penalty and print the readout.
- **[F10]** `registry.hasWeakness(w)` + `countWeakness(w)` + `weaknesses()` drive Ward.
- **[F11]** `whispers.find(name)` is the free identify; a **local copy** is banished so one
  beast is forced onto the Recall path — the shared table stays untouched.

You may decompose `runTreeWarden` however you want — `buildMenagerie`, `printTreeMenu`,
`readTreeChoice`, and `identifyFront` are the four natural extractions. The only contract
`main.cpp` depends on is the public API in `Battle.h`:

```cpp
BattleOutcome runTreeWarden(Hero& hero,
                            const Tree<std::string>& taxonomy,
                            const BST<Monster>&      catalogue,
                            const Registry&          registry,
                            const HashTable&         whispers);
```

Internal organisation is yours.

### If you're working on Path A (your own Floor 11 project)

You'll need to merge in `warden-3-trials/` and the wiring. **The diffs are small — and this
time there is no shared-header field to add and no CMake change.**

1. **Copy `warden-3-trials/`** into your project folder.

2. **`battle/Battle.h`** — add `BattleException` (if your header doesn't already have it) and
   the `runTreeWarden` declaration. `BattleException` is thrown for battle errors that are
   *not* a bad index (an unknown weakness typed at the Ward prompt); a genuine bad menu index
   still uses `BagException`. Add the includes the new signature needs
   (`../hero/Tree.h`, `../hero/BST.h`, `../hero/HashTable.h`, `../bestiary/Registry.h`,
   `../bestiary/Monster.h`):

   ```cpp
   class BattleException : public std::exception {
   public:
       explicit BattleException(const std::string& message) : msg_(message) {}
       const char* what() const noexcept override { return msg_.c_str(); }
   private:
       std::string msg_;
   };

   BattleOutcome runTreeWarden(Hero& hero,
                               const Tree<std::string>& taxonomy,
                               const BST<Monster>&      catalogue,
                               const Registry&          registry,
                               const HashTable&         whispers);
   ```

3. **`battle/Battle.cpp`** — add the `runTreeWarden` definition (the big stub in this
   starter is the scaffold; follow its TODOs).

4. **`main.cpp`** — extend the existing `battle` command dispatch to also handle `trees`.
   The four indexes (`taxonomy`, `bestiaryTree`, `registry`, `whispers`) are already locals
   in `main()`; pass them straight through by `const&`:

   ```cpp
   else if (cmd == "battle") {
       std::istringstream bs(rest);
       std::string which;
       bs >> which;
       if (which == "warden") {
           BattleOutcome outcome = runWardenBattle(hero);
           switch (outcome) { /* ...existing Warden 1 banners... */ }
       }
       else if (which == "trees") {
           BattleOutcome outcome =
               runTreeWarden(hero, taxonomy, bestiaryTree, registry, whispers);
           switch (outcome) {
               case BattleOutcome::Victory:
                   std::cout << "\nThe menagerie is unmasked. The Warden falls. The Archive stair opens.\n"; break;
               case BattleOutcome::Defeat:
                   std::cout << "\nThe Warden files you among its faces. The stair stays shut.\n"; break;
               case BattleOutcome::Fled:
                   std::cout << "\nYou step back from the Warden. Its menagerie still stands.\n"; break;
           }
       }
       else {
           std::cout << "Usage: battle warden   |   battle trees\n";
       }
   }
   ```

   And slot one line into `printHelp()`:

   ```cpp
                 << "   battle trees                  — face the Warden of the Trees (Midterm 3)\n"
   ```

**No `UndoAction`-style field extension** (that was Warden 2) and **no `CMakeLists.txt`
change** — `battle/Battle.cpp` is already in your executable from Warden 1, and the `Enemy`
struct + the `identified`/`exposed` flags live entirely inside `Battle.cpp`.

## What you also write — Trials I & II + notes

Inside [`warden-3-trials/`](warden-3-trials/) there are four template files. Open each,
replace the `(your answer)` and `(paste …)` placeholders with your work, and commit them
with the rest of your submission.

| File | What it is |
|------|------------|
| `AI-USE.md`           | Per-trial declaration: which tool, what you asked for, what you did with the result. |
| `trial-1-brief.md`    | Trial I — four short answers (50–100 words each). Floors 9 / 9½ are merged into one. |
| `trial-2-critique.md` | Trial II — engage an LLM with the generic `HashTable<K,V>` prompt, paste prompt + reply, write a ≥150-word critique. |
| `encounter-notes.md`  | Trial III notes — AI declaration, **two functions you wrote without AI**, brief reflection, the four-tie map. |

## Submission

Commit and push **the whole thing** — your modified `main.cpp`, `battle/Battle.{h,cpp}`, and
your `warden-3-trials/` folder — to your project repo by **9:00 AM Friday of Week 15**. The
Friday in-class quiz is during that day's class period. Bring a pen.
