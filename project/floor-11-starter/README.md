# Floor 11 — The Hall of Whispers — starter

Read [the public Floor 11 page](https://hogred.github.io/comp2450/floors/floor-11/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 10 submission.

## What's new for Floor 11

This is the **last data structure you build by hand** in the course — and it is made out of one you already built. Floor 10 walked a balanced tree in **O(log n)** by keeping everything in **order**. The Hall of Whispers answers in **O(1) average** by giving up order entirely: it **computes** a bucket from the name (a *hash*) and goes straight there. A whisper, not a walk.

The structure is a **separate-chaining hash table**. The bucket array is a `std::vector`, and **each bucket is one of your own `Chain<Monster>` linked lists** (`hero/Chain.h`, from Floor 4) holding every monster that hashes to that index. That is the payoff, said plainly: a hash table **is an array of the chain you already built.** When two different names land in the same bucket — a **collision**, which is not a bug but *inevitable* (more names than buckets) — the bucket's `Chain` simply holds both, and you scan a short list of two instead of all 15.

All the new work is in **`hero/HashTable.h`** — a concrete `HashTable` class keyed on the monster's **name**, added **in parallel** to the working `Bag`, `BST` (Floor 9), and `Registry` (Floor 10). Nothing earlier is touched. The members (both **provided**):

- `std::vector<Chain<Monster>> buckets_` — the bucket array: an array of *your* `Chain`.
- `std::size_t count_` — items stored, so `size()` and `loadFactor()` are O(1).

You write **six small method bodies**, in the clearly marked `// ===== YOU WRITE =====` block:

- `hashKey`, `bucketIndex`, basic `insert` — **Monday.** `hashKey` is a **multiplicative string hash** (`h = h * 31 + c` over the characters) — it returns a raw hash *code*. `bucketIndex` reduces that code to a slot with `% buckets_.size()` (the **modulo hash**). `insert`'s basic body computes the bucket, `push_back`s the monster, and `++count_`.
- `find`, `erase`, and the rest of `insert` — **Wednesday.** `find` computes `bucketIndex(name)` and scans **only that one bucket's chain** — and compares the **name**, not just the bucket (two names can collide). `erase` removes the matching item from its bucket's chain via the provided `Chain::eraseFirst(pred)`. `insert` gains its **dup-check** (re-inserting a name updates in place and returns `false`, no double-count).
- `rehashIfNeeded` — **Friday.** When `loadFactor()` climbs past the threshold (0.75), **double the bucket count** and rehash. The resize *driver* (`rehash`) is provided; you write the **one line inside it** that recomputes each item's new home (an item's bucket changes when the bucket count changes).

Also provided (you don't edit these):

- `hero/HashTableTests.h/.cpp` — `selftest whispers`, **six phases** over **fixed inline samples** (it does *not* read `data/monsters.txt`, so the expected values are pinned). Where collisions must be guaranteed regardless of your hash, the test forces them with a tiny bucket count.
- `hero/Chain.h` — gains a reference-only `eraseFirst(pred)` helper. You still **do not edit** `Chain.h`.
- The REPL commands `hash`, `buckets`, `whisper`, `banish`, `load`, and `benchmark whispers`.
- `benchmark whispers [N]` — the payoff: your `HashTable::find` vs `std::unordered_map` vs `std::map` vs your Floor 9 `BST::find` vs Floor 1's linear scan. The competitors are built **inline**, so the command works from day one; the *your-HashTable* column prints `--` until you write `insert`.

Everything from Floors 0–10 is in place and **working** — including your complete `BST<T>` and the Floor 10 `Registry`. The prior floors' work ships as finished reference code; this starter is self-contained.

> ### Four independent indexes — read this
> The Bag, the BST, the Registry, and now the HashTable are **four independent indexes** over the same data. `banish <name>` removes a monster from the **HashTable only** — it does *not* touch the Bag, the BST, or the Registry. So when you `banish` a monster and then `list`/`catalogue`/`names` and it is **still there**, nothing is broken; you only changed one of four views. (Floor 10 already taught the three-way version of this.)

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new HashTable work does-nothing-useful-yet:

- `selftest whispers` — **all six phases FAIL** with one-line diagnostics naming the stubbed method. It does **not** crash and does **not** fail to build; that's the staircase. (Every *other* selftest — `bst`, `balance`, `registry`, `chain`, `tree`, `stack`, `queue` — passes in this same stub state, which proves the prior floors ship as finished reference code.)
- `hash <name>` — prints `hash(...) = 0  ->  bucket 0 of 8` until `hashKey`/`bucketIndex` are written.
- `buckets` / `whisper` / `banish` / `load` — empty / no answer / nothing removed until `insert`/`find`/`erase` land.
- `benchmark whispers` — **already works** (the competitor columns don't depend on your stubs): linear climbs while `std::map`, `std::unordered_map`, and the BST stay flat. The *HashTable* column shows `--` until you write `insert`, then joins the race flat.

By Friday, `selftest whispers` is all green and the headline commands produce real output.

## What you write this week

| Day | Methods in `hero/HashTable.h` | Lights up |
|-----|-------------------------------|-----------|
| **Mon** | `hashKey`, `bucketIndex`, basic `insert` | `selftest whispers` phase 1; `hash <name>` shows the computed bucket; `buckets` shows the 15 monsters **scattered** across slots — with at least one bucket holding two names (your first **collision**). |
| **Wed** | finish `insert` (no dups), `find`, `erase` | phases 2–5; `whisper <name>` returns a record by scanning **one** bucket; `banish <name>` removes one item and `buckets` shows the chain shrink. |
| **Fri** | `rehashIfNeeded` (+ the re-file line in `rehash`) | phase 6 — all green. `load` shows the load factor; force growth and the bucket count **doubles**. Then `benchmark whispers` lands the lesson: your hash table is **flat** and `std::unordered_map` tracks it. |

The recurring idea is **the hashing pipeline** — key → hash *code* → bucket *index* → the *chain* in that bucket — and the **load factor / rehash** trade, not pointer mechanics (the `Chain` does that).

## Watch out for the bad hash (Grix's mistake)

A hash function must **scatter** — spread keys evenly across the buckets. Grix's "fast" hash returned the name's *length*, so every six-letter beast (`Goblin`, `Wraith`, `Ratking`…) piled into bucket 6 and his "hash table" became a few **enormous chains** — a linked list wearing a hash table's hat, **O(n) again.** The multiplicative string hash mixes every character so similar names land far apart. If your `buckets` dump is lopsided (one giant chain, the rest empty), your hash is clumping, not scattering.

## Bringing your own Floor 10 code forward

If you customized `hero/BST.h`, `bestiary/Registry.h`, or `hero/Chain.h`, **bring your bodies across** — this starter ships the reference versions, so drop your own over them if you'd rather build on your code. The `HashTable` is brand-new this floor, so there's nothing of yours to carry into `hero/HashTable.h`. (Note: `Chain.h` here adds `eraseFirst` — if you swap in your own `Chain.h`, keep that helper or `HashTable::erase` won't compile.)

## Lab notes

`floor-11/lab-notes.md` (in your project repo, not in this starter) — the lab items are spelled out on [the Floor 11 page](https://hogred.github.io/comp2450/floors/floor-11/). Commit at least three times — Mon (`hashKey`/`bucketIndex`/`insert`), Wed (`find`/`erase`/no-dup `insert`), Fri (`rehashIfNeeded` + lab notes).
