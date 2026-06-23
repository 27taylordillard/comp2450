# Floor 12 — The Labyrinth — starter (THE FINALE)

Read [the public Floor 12 page](https://hogred.github.io/comp2450/floors/floor-12/) before you start. It has the full week's narrative, objectives, and lab requirements. This README only covers the starter scaffold and the swap-in path from your Floor 11 submission.

## What's new for Floor 12

This is the **last floor of the dungeon** — the finale. For eleven floors every structure you built stored **items**: a `Bag` of monsters, a `Chain` of items, a `Stack` of undo-actions, a `Queue` of turns, a `Tree` of categories, a `BST` and a `HashTable` of monsters. The Labyrinth stores **none** of those. It stores the **connections** between rooms — which passage opens onto which. The data stopped being a list of things and became a **web of relationships**, and crossing that web is the last lesson of the descent: it was never about storing. It is about **connecting**.

The structure is an **undirected, unweighted adjacency-list graph** (ZyBook Ch 13). The rooms are **vertices**; the passages between them are **edges**. *Undirected* means a door you walk through you can walk back through, so every edge runs both ways. *Unweighted* means every passage costs one step, so the **shortest path is the fewest doors** — the number of *hops* — which is exactly what **breadth-first search** finds for free.

And the finale does not build an eighth container. It **reaches back** for two you already built:

- **BFS** (reachability + shortest path) rides **your Floor 7 `Queue`**.
- **DFS** rides **your Floor 6 `Stack`**.
- The **visited set** that keeps a cycle from looping forever is the **`std::unordered_set` you met on Floor 11**.

All the new work is in **`hero/Graph.h`** — a concrete `Graph` class keyed on the room **name**, added **in parallel** to the working `Bag`, `BST` (Floor 9), `Registry` (Floor 10), and `HashTable` (Floor 11). Nothing earlier is touched. The one member is **provided**:

- `std::unordered_map<std::string, std::vector<std::string>> adj_` — the adjacency list: `adj_["Lich"]` is the vector of rooms one passage from the Lich's chamber. The §13.3 representation in one declaration. (`std::unordered_map` is a Floor-11 callback and makes copy/destructor compiler-supplied — no Rule of Three here.)

You write **six small method bodies**, in the clearly marked `// ===== YOU WRITE =====` block:

- `addRoom`, `connect`, `neighbors` — **Monday.** `addRoom` ensures a room is a vertex (a key with a possibly-empty neighbor list), idempotently. `connect` adds an **undirected** edge — **both directions** (the single most common bug is doing only one). `neighbors` returns a room's adjacency list using **`find`, not `operator[]`** (so querying an absent room does not *create* it — the Phantom-Key bug from Floor 10, now on the graph).
- `reachable`, `bfsOrder` — **Wednesday.** Both are a **breadth-first search** over **your `Queue`** plus a **visited set**: `reachable` returns whether a path exists; `bfsOrder` returns the rooms in level-by-level visit order, each once. The visited set is what stops a **cycle** from looping forever — mark a room visited the first time you see it and never enqueue it again.
- `dfsOrder` — **Friday.** A **depth-first search** over **your `Stack`** (push start; pop; if unvisited, mark and record, then push its neighbors). Same map, **opposite order** from BFS — LIFO instead of FIFO, exactly the Stack-vs-Queue contrast from Floors 6–7. **Use the Stack, not recursion** (the cliffhanger promised "the Stack you built becomes depth-first").

Also provided (you don't edit these):

- `hops(from, to)` — a BFS that **counts** the shortest way down (or `-1` if unreachable). It rides on **your** `neighbors()`, so `descend` works the moment your Monday work lands; you do **not** write it. (Off-by-one: the start room is **0** doors from itself, an adjacent room is **1**.)
- `hero/GraphTests.h/.cpp` — `selftest crossing`, **six phases** over **fixed inline sample graphs** (it does *not* read `data/labyrinth.txt`, so the expected values are pinned). Phases 1–4 assert order-independent properties; phases 5–6 pin the exact visit order of a tiny controlled graph.
- `data/labyrinth.txt` — the room graph (passages, not monsters). One edge per line (`RoomA | RoomB`); a single room on its own line is an isolated room. Connected **except** one sealed room (`Oubliette`) for the `reach … == false` demo; it has a **cycle** and a **branch** so BFS and DFS come out in different orders.
- The REPL commands `map`, `doors`, `reach`, `cross`, `descend`, and `benchmark crossing`.
- `benchmark crossing [N]` — the §13.4 contrast as a race: an adjacency **list** crossing vs an adjacency **matrix** crossing of the same sparse graph. The competitors are built **inline**, so the command works from day one; the *YourGraph* column prints `--` until you write `bfsOrder`.

Everything from Floors 0–11 is in place and **working** — including your complete `BST`, the Floor 10 `Registry`, and the Floor 11 `HashTable`. The prior floors' work ships as finished reference code; this starter is self-contained.

> ### Five independent indexes — read this
> The Bag, the BST, the Registry, the HashTable, and now the Graph are **five independent indexes**. Four of them mirror the **monsters**; the Graph mirrors the **map** — it is the first index *not* built from the bestiary, because it stores *passages*, not *monsters*. So `banish`-ing or `strike`-ing a monster changes nothing in the Labyrinth, and crossing the Labyrinth touches no monster index. (Floors 10 and 11 already taught the three- and four-way versions of "independent views"; this is the fifth, of a different thing entirely.)

## Build & run

```
cmake -B build
cmake --build build
./build/the_descent          # macOS / Linux
build\the_descent.exe        # Windows cmd
```

You should land at the `> ` prompt with everything from previous floors working. The new Graph work does-nothing-useful-yet:

- `selftest crossing` — **all six phases FAIL** with one-line diagnostics naming the stubbed method. It does **not** crash and does **not** fail to build; that's the staircase. (Every *other* selftest — `whispers`, `bst`, `balance`, `registry`, `chain`, `tree`, `stack`, `queue` — passes in this same stub state, which proves the prior floors ship as finished reference code.)
- `map` — prints `(the Labyrinth is dark — is Graph::connect written yet?)` until `connect`/`addRoom` are written.
- `doors` / `reach` / `cross` / `descend` — empty / no path / no rooms / no way down until the bodies land.
- `benchmark crossing` — **already works** (the matrix and list columns don't depend on your stubs): the matrix climbs steeply (O(V²)) while the list stays flat-ish. The *YourGraph* column shows `--` until you write `bfsOrder`, then joins the race tracking the reference list.

By Friday, `selftest crossing` is all green and the headline commands produce real output.

## What you write this week

| Day | Methods in `hero/Graph.h` | Lights up |
|-----|---------------------------|-----------|
| **Mon** | `addRoom`, `connect`, `neighbors` | `selftest crossing` phases **1–3**; `map` prints every room and the doors out of it (the adjacency list); `doors <room>` lists one room's passages. The map is **built** — nothing is traversed yet. |
| **Wed** | `reachable`, `bfsOrder` | phases **4–5**; `reach <from> <to>` answers yes/no; `cross <start> --bfs` walks the map **level by level**; `descend <from> <to>` reports the **shortest way down** in doors. This is the **Queue** you built on Floor 7, plus the **visited set** (the Floor 11 hash set) that keeps a cycle from looping forever. |
| **Fri** | `dfsOrder` | phase **6** — all green. `cross <start> --dfs` **plunges deep** then backtracks — a *different order* from `--bfs` on the *same* map, over your **Stack**. Then `benchmark crossing` lands the lesson: the adjacency list is flat-ish on a sparse map while the matrix pays O(V²). Close on the whole-arc wrap and the **Final Descent**. |

The recurring idea is **connect, then walk it two ways without revisiting** — the FIFO `Queue` gives "level by level," the LIFO `Stack` gives "plunge then backtrack," and the same visited set serves both. No pointer surgery (the `Stack`/`Queue`/`unordered_set` do the bookkeeping); no new container internals (the adjacency list is a provided `unordered_map`).

## Watch out for the Cycle (the finale's monster)

A graph is **not a tree** — it has no leaves to stop you, no single root. Follow the passages of a loop `A → B → C → A` naively and you walk it **forever**: the program does not crash, it simply **never returns** — the scariest kind of bug, discovered when a demo hangs. The fix is the **visited set**: mark each room the *first* time you discover it, and **check membership before you enqueue/push it** so a room never enters the frontier twice. The same set works for BFS (over the `Queue`) and DFS (over the `Stack`) — the container differs, the *memory* is identical. Grix tried to cross without it and is still walking the first loop, delighted he's "making progress."

## Bringing your own Floor 11 code forward

If you customized `hero/BST.h`, `bestiary/Registry.h`, `hero/HashTable.h`, or `hero/Chain.h`, **bring your bodies across** — this starter ships the reference versions, so drop your own over them if you'd rather build on your code. The `Graph` is brand-new this floor, so there's nothing of yours to carry into `hero/Graph.h`. The traversals reuse `hero/Stack.h` and `hero/Queue.h` as finished reference code; if you swap in your own, keep `push`/`pop`/`top`/`empty` on the Stack and `enqueue`/`dequeue`/`front`/`empty` on the Queue or `Graph` won't compile.

## Lab notes

`floor-12/lab-notes.md` (in your project repo, not in this starter) — the lab items are spelled out on [the Floor 12 page](https://hogred.github.io/comp2450/floors/floor-12/). Commit at least three times — Mon (`addRoom`/`connect`/`neighbors`), Wed (`reachable`/`bfsOrder`), Fri (`dfsOrder` + lab notes). This is the last floor; the cumulative **Final Descent** (final exam / capstone) follows.
