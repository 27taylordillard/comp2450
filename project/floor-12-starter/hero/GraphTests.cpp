// COMP 2450 — Floor 12 starter (the finale)
// hero/GraphTests.cpp — the `selftest crossing` harness. Provided. Do not edit.
//
// Six phases over small FIXED inline sample graphs, built in-test so the
// expected values are pinned and never depend on data/labyrinth.txt (exactly
// as BSTTests builds {5,3,8,2,4,7,9} and HashTableTests builds its
// five-monster sample). Output format matches the other harnesses byte for
// byte: two leading spaces, "phase: OK" / "phase: FAIL — <diag>" with a real
// em-dash, and a trailing "all phases OK" / "(see FAILs above)".
//
// Testing a traversal deterministically — the design rule (read before editing
// anything, though you should not edit this file): BFS/DFS visit ORDER depends
// on the order neighbors were inserted into the adjacency list. adj_ is an
// unordered_map, but the traversal order depends only on each room's NEIGHBOR
// VECTOR order (which is insertion order via push_back), NOT on the map's
// bucket order — so as long as the test fixes the connect() call order, the
// per-room neighbor order is deterministic and the visit order is pinned.
//   * Phases 1–4 assert ORDER-INDEPENDENT properties (vertices exist,
//     reachability, hop-distance, "visited once"), robust regardless of hashing.
//   * Phases 5–6 assert the EXACT visit sequence of a fixed tiny graph whose
//     edges are added in a controlled order, so the expected vector is pinned.
//
// Fixed sample graph A (a graph WITH A CYCLE, so the visited set is exercised):
//   rooms {A, B, C, D, E}; edges added in THIS order:
//       A–B, B–C, C–A  (the cycle A-B-C-A), then C–D, D–E.
//   E is the deepest room; the A-B-C triangle is the cycle that would trap a
//   visited-less traversal in an infinite loop.
// Fixed sample graph B (for reachability): the same, plus an ISOLATED room Z
//   (addRoom("Z") with no edges), so reachable(A, Z) == false and BFS/DFS from
//   A never reach Z.
//
// MWF: Monday's addRoom + connect + neighbors green phases 1–3; Wednesday's
// reachable + bfsOrder green phases 4–5; Friday's dfsOrder greens phase 6.

#include "GraphTests.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Graph.h"

namespace dungeon {

namespace {

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

// Build fixed sample graph A: the cycle A-B-C plus the C-D-E corridor. Edges
// are added in a CONTROLLED order so the neighbor vectors — and therefore the
// BFS/DFS visit order — are deterministic and pinnable.
Graph buildSampleA() {
    Graph g;
    g.connect("A", "B");   // A: [B]      B: [A]
    g.connect("B", "C");   // B: [A,C]    C: [B]
    g.connect("C", "A");   // C: [B,A]    A: [B,C]   <- the cycle A-B-C-A
    g.connect("C", "D");   // C: [B,A,D]  D: [C]
    g.connect("D", "E");   // D: [C,E]    E: [D]
    return g;
}

// Sample A plus the isolated room Z (no passages) for the reachability phase.
Graph buildSampleAB() {
    Graph g = buildSampleA();
    g.addRoom("Z");        // a sealed room: reachable(A, Z) must be false
    return g;
}

// True iff `v` contains `x`.
bool contains(const std::vector<std::string>& v, const std::string& x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

// True iff `v` has no duplicate entries (every room recorded at most once).
bool allUnique(std::vector<std::string> v) {
    std::sort(v.begin(), v.end());
    return std::adjacent_find(v.begin(), v.end()) == v.end();
}

}  // anonymous namespace

bool runGraphSelfTest() {
    bool all = true;

    // --- Phase 1: addRoom builds vertices, idempotently ---
    // After addRoom("A") twice and addRoom("B"), there are exactly two rooms,
    // both present, and an un-added room is absent. Adding "A" twice must not
    // create a second "A" (idempotent).
    {
        Graph g;
        g.addRoom("A");
        g.addRoom("A");   // idempotent: still one "A"
        g.addRoom("B");
        const bool ok = (g.roomCount() == 2
                         && g.hasRoom("A") && g.hasRoom("B")
                         && !g.hasRoom("Q"));
        result("addRoom: two rooms added, idempotent, absent room absent", ok,
               "addRoom must add the room as a key with an empty adjacency list, and adding an "
               "existing room must NOT duplicate it (idempotent)");
        all = all && ok;
    }

    // --- Phase 2: connect adds an UNDIRECTED edge ---
    // After connect("A","B") on an empty graph, both rooms exist (connect
    // ensures them), and the edge is symmetric: B is in A's neighbors AND A is
    // in B's neighbors. Connecting only one direction fails here.
    {
        Graph g;
        g.connect("A", "B");
        const bool bothExist = g.hasRoom("A") && g.hasRoom("B");
        const bool symmetric = contains(g.neighbors("A"), "B")
                            && contains(g.neighbors("B"), "A");
        const bool ok = bothExist && symmetric;
        result("connect: undirected edge — both rooms exist, B in A and A in B", ok,
               "connect must add BOTH directions (a in b's list AND b in a's list) — an undirected "
               "edge is symmetric — and must ensure both rooms exist");
        all = all && ok;
    }

    // --- Phase 3: neighbors returns the adjacency list, no phantom insert ---
    // In sample A, C is adjacent to exactly {B, A, D} (as a SET — order is the
    // student's neighbor-insertion order and not asserted here). Querying an
    // ABSENT room returns empty AND must not grow the graph: roomCount() is
    // unchanged afterward (neighbors must use find, not operator[]).
    {
        Graph g = buildSampleA();
        const std::size_t before = g.roomCount();
        const std::vector<std::string> nC = g.neighbors("C");
        const bool cRight = (nC.size() == 3
                             && contains(nC, "A")
                             && contains(nC, "B")
                             && contains(nC, "D"));
        const std::vector<std::string> nAbsent = g.neighbors("Q");
        const bool absentEmpty = nAbsent.empty();
        const bool noPhantom = (g.roomCount() == before && !g.hasRoom("Q"));
        const bool ok = cRight && absentEmpty && noPhantom;
        result("neighbors: C -> {A,B,D}; absent room empty AND no phantom insert", ok,
               "neighbors must return the room's adjacency list (or empty for an absent room) "
               "WITHOUT inserting the missing key — use find, not operator[]");
        all = all && ok;
    }

    // --- Phase 4: reachable via BFS, across the cycle and to a sealed room ---
    // On sample A+B: A reaches E (A->...->E across the cycle), A reaches itself
    // trivially, and A does NOT reach the isolated Z. The cycle A-B-C must NOT
    // hang — if this phase returns at all, the visited set is doing its job.
    {
        Graph g = buildSampleAB();
        const bool toE     = g.reachable("A", "E");    // across the corridor
        const bool toSelf  = g.reachable("A", "A");    // trivially true
        const bool toZ     = g.reachable("A", "Z");    // sealed: must be false
        const bool ok = (toE && toSelf && !toZ);
        result("reachable: A->E true, A->A true, A->Z false (sealed room)", ok,
               "reachable must BFS from `from`, marking rooms visited so a cycle does not loop "
               "forever; return true iff `to` is found, false if the search drains (disconnected)");
        all = all && ok;
    }

    // --- Phase 5: bfsOrder is level-by-level, each room once ---
    // On sample A from A, with edges added in the fixed order, the breadth-first
    // visit order is exactly {A, B, C, D, E}: A first, then A's neighbors
    // (B, C), then theirs (D), then E. Every room appears exactly once (no
    // repeats despite the cycle), and the count is the reachable-room count, 5.
    {
        Graph g = buildSampleA();
        const std::vector<std::string> order = g.bfsOrder("A");
        const std::vector<std::string> expected = {"A", "B", "C", "D", "E"};
        const bool ok = (order == expected) && allUnique(order);
        result("bfsOrder: from A is [A, B, C, D, E], each room once", ok,
               "bfsOrder must visit breadth-first using the Queue and a visited set: start, then "
               "distance-1, then distance-2; each room exactly once (no repeats in the cycle); "
               "record rooms as they are visited");
        all = all && ok;
    }

    // --- Phase 6: dfsOrder plunges deep, each room once ---
    // On sample A from A, with edges added in the fixed order, the explicit-Stack
    // depth-first visit order is exactly {A, C, D, E, B}: from A it pushes B then
    // C, pops C (LIFO), plunges C->D->E to the corridor's end, then backtracks to
    // B. Every reachable room appears exactly once (no repeats despite the
    // cycle), the count is 5, and A is first. (The exact sequence is the
    // explicit-stack DFS, pinned against the reference solution; a recursive DFS
    // could differ — this floor's DFS MUST use the Stack.)
    {
        Graph g = buildSampleA();
        const std::vector<std::string> order = g.dfsOrder("A");
        const std::vector<std::string> expected = {"A", "C", "D", "E", "B"};
        const bool ok = (order == expected) && allUnique(order);
        result("dfsOrder: from A is [A, C, D, E, B], each room once", ok,
               "dfsOrder must visit depth-first using the Stack and a visited set: go deep along a "
               "path before backtracking; each room exactly once; record rooms as they are visited");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
