# Floor 10 — The Hall of Names — starter

Read [the public Floor 10 page](https://hogred.github.io/comp2450/floors/floor-10/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 9½ submission.

## What's new for Floor 10

You do not build a container this week — you **use** three. For five floors you built pointer structures by hand: `Chain`, `Stack`, `Queue`, `Tree`, `BST`, and the AVL balancing you deliberately *didn't* finish. The lesson of Floor 9½ was *don't hand-roll the balanced tree — reach for `std::set`/`std::map`, which **are** balanced trees.* **This floor is where you reach.** You write **client code** against `std::set`, `std::map`, and `std::pair`; you do **not** implement an associative container. The payoff, said plainly: the library already hands you the balanced tree you just studied, and a key→value index on top of it, for free.

All the new work is in **`bestiary/Registry.h`** — a thin `Registry` class that re-indexes McCown's bestiary (the same `Bag<Monster>` loaded from `data/monsters.txt`) into three standard containers, added **in parallel** to the working `Bag` and `BST` from earlier floors. Nothing earlier is touched. The three containers (all **provided**):

- `std::map<std::string, Monster> byName_` — the bestiary indexed by name. The **key→value index**: one declaration plus one `find()` does what your whole hand-written `BST<Monster>` + `recall`/`forget` did on Floor 9.
- `std::set<std::string> weaknesses_` — the **distinct** weaknesses present. The raw data has duplicates (`fire` ×6, `holy` ×3, …); a set collapses them, stays sorted, and answers membership in one call.
- `std::map<std::string, int> weaknessCounts_` — how many monsters share each weakness. The home of the `operator[]` discussion.

You write **six small method bodies**, in the clearly marked `// ===== YOU WRITE =====` block near the bottom:

- `index`, `knows` — **Monday.** `index` populates all three containers (`byName_[m.name] = m;`, `weaknesses_.insert(m.weakness);`, `weaknessCounts_[m.weakness] += 1;`) and is what the provided constructor calls once per monster. `knows` is membership in one call — `byName_.count(name) > 0`, **not** a loop.
- `recall`, `hasWeakness`, `countWeakness` — **Wednesday.** `recall` is one `map::find` (return `&it->second` or `nullptr`) — the one-liner that replaces Floor 9's whole recursive descent. `hasWeakness` is `set::count` on `weaknesses_`. `countWeakness` reads `weaknessCounts_` with **`find`, never `operator[]`** — and it is `const`, so `[]` won't even compile there (that's the point: the design makes the Phantom Key bug impossible).
- `forget` — **Friday.** Remove the named monster and keep the derived structures consistent: `erase` from `byName_`, decrement the count, and **when a count hits 0, erase the weakness from BOTH `weaknessCounts_` and `weaknesses_`.** Return whether anything was removed.

Also provided (you don't edit these):

- `bestiary/RegistryTests.h/.cpp` — `selftest registry`, **six phases** over a **fixed inline sample** (it does *not* read `data/monsters.txt`, so the expected values are pinned).
- The REPL commands `knows`, `lookup`, `strike`, `names`, `weaknesses`, `weakness`, and `benchmark names`.
- `benchmark names [N]` — the payoff: `std::map::find` vs your Floor 9 `BST::find` vs Floor 1's linear scan. It builds its comparison `std::map` **inline**, so it works from day one, independent of your stubs.

Everything from Floors 0–9½ is in place and **working** — including your complete `BST<T>` and its balance helpers. The prior floors' work ships as finished reference code; this starter is self-contained.

> ### Two verbs named `recall`/`forget` — read this
> Floor 9 gave you `recall <name>` and `forget <name>`, bound to the **BST**. They are still here and **unchanged**. The `Registry`'s own lookup/removal methods are *also* named `recall`/`forget` internally (a deliberate story echo) — but their **REPL verbs are different**: you call the Registry's lookup with **`lookup <name>`** and its removal with **`strike <name>`**. So when you `strike` a monster from the Hall of Names and then `recall` it from the tree and it is **still there**, nothing is broken — the Bag, the BST, and the Registry are three independent indexes over the same data, and you only changed one.

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new Registry work does-nothing-useful-yet:

- `selftest registry` — **all six phases FAIL** with one-line diagnostics naming the stubbed helper. It does **not** crash and does **not** fail to build; that's the staircase. (Every *other* selftest — `bst`, `balance`, `chain`, `tree`, `stack`, `queue` — passes in this same stub state, which proves the prior floors ship as finished reference code.)
- `knows` / `lookup` / `weakness` — answer "no" / "not found" / "nothing" until you write the methods.
- `names` / `weaknesses` — print empty (or the distinct set once `index` is written) until the methods land.
- `benchmark names` — **already works** (it doesn't depend on your stubs): it shows the linear scan climbing while the BST and `std::map` stay flat. This is the comparison the whole floor is built around.

By Friday, `selftest registry` is all green and the headline commands produce real output.

## What you write this week

| Day | Methods in `bestiary/Registry.h` | Lights up |
|-----|-----------------------------------|-----------|
| **Mon** | `index`, `knows` | `selftest registry` phases 1–2; `knows <name>` answers; `weaknesses` shows the distinct set **already sorted** (file order in, alphabetical out — that's the balanced tree). |
| **Wed** | `recall`, `hasWeakness`, `countWeakness` | phases 3–5; `lookup <name>` returns a full record in one `find`; `names` lists the bestiary in name order via `for (const auto& [name, m] : ...)` — the `std::pair` + structured-binding moment. |
| **Fri** | `forget` | phase 6 — all green. Then `benchmark names` lands the lesson: `std::map` matches your hand-built balanced tree and crushes the scan, in one declaration. |

The recurring idea is "which container, which call, and why," not pointer mechanics: a `std::set` when you only need to **know** something is present, a `std::map` when you need a **value** back.

## Watch out for the Phantom Key

`std::map::operator[]` on a **missing** key does not return "not found" — it default-constructs a value and **inserts it**. So reading a count with `weaknessCounts_["thunder"]` doesn't just return `0`; it *creates* a `thunder → 0` entry, a weakness no beast has. Ask with `count()`/`find()`; reserve `[]` for keys you mean to **set** (like `index`, where creating the key is the point). Making `countWeakness` `const` turns the mistake into a compile error — use that.

## Bringing your own Floor 9½ code forward

If you customized `hero/BST.h` (Floors 9/9½), **bring your helper bodies across** — this starter ships the reference versions, so drop your own over them if you'd rather build on your code. The Registry is brand-new this floor, so there's nothing of yours to carry into `bestiary/Registry.h`. Everything earlier is unchanged; swap in your versions only if you customized them.

## Lab notes

`floor-10/lab-notes.md` (in your project repo, not in this starter) — the lab items are spelled out on [the Floor 10 page](https://hogred.github.io/comp2450/floors/floor-10/). Commit at least three times — Mon (`index`/`knows`), Wed (`recall`/`hasWeakness`/`countWeakness`), Fri (`forget` + lab notes).
