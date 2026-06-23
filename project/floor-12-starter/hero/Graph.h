// COMP 2450 — Floor 12 starter (THE FINALE)
// hero/Graph.h — the Labyrinth. THE WORK IS HERE (the six bodies in the
// "===== YOU WRITE =====" block near the bottom).
//
// For eleven floors every structure you built stored ITEMS — a Bag of
// monsters, a Chain of items, a Stack of undo-actions, a Queue of enemy
// turns, a Tree of categories, a BST and a HashTable of monsters. The
// Labyrinth stores none of those. It stores the CONNECTIONS between rooms:
// which passage opens onto which. The data stopped being a list of things
// and became a web of relationships — and crossing that web is the last
// lesson of the descent: it was never about storing. It is about connecting.
//
// =====================================================================
// What this class is — an adjacency-list graph of dungeon rooms (§13.3)
// =====================================================================
// An UNDIRECTED, UNWEIGHTED graph (ZyBook Ch 13). The rooms are VERTICES
// (§13.1); the passages between them are EDGES. "Undirected" means a door you
// walk through you can walk back through, so every edge runs both ways
// (§13.5 contrast). "Unweighted" means every passage costs one step, so the
// shortest path is simply the FEWEST DOORS — the number of hops — which is
// exactly what breadth-first search finds for free (§13.6 → §13.7).
//
// The representation is an ADJACENCY LIST (§13.3): each room keeps a list of
// the rooms it connects to directly. We store that as a
// std::unordered_map<std::string, std::vector<std::string>> — adj_["Lich"] is
// the vector of rooms one passage from the Lich's chamber. That is the §13.3
// representation in a single declaration, and using std::unordered_map is a
// Floor-11 callback (the library hash map you met last floor) that makes
// copy/destructor compiler-supplied (no Rule of Three for you here). The
// CONTRAST representation — an adjacency MATRIX (§13.4), a V×V grid of 0/1 —
// is read-for-contrast only: it reserves a slot for every POSSIBLE door
// whether or not it exists, O(V²) space, wasteful for a sparse dungeon map.
// The list stores only the doors that are really there.
//
// It is keyed on the room NAME (a std::string) on purpose, not an integer id:
// the headline beats are "BFS over the Queue you built" and "DFS over the
// Stack you built," and seeing the room names — Lich, Goblin, Entrance —
// flow through the queue and the stack is what makes the traversal legible.
//
// It is added IN PARALLEL to the working Bag/BST/Registry/HashTable —
// nothing earlier is touched. It is the FIRST index that does not re-derive
// from the monster Bag: it is built from data/labyrinth.txt and stores
// passages, not monsters. Five independent indexes now coexist, four of the
// monsters and one of the MAP.
//
// =====================================================================
// What is provided vs. what you write
// =====================================================================
// PROVIDED (complete — do NOT edit): the one member; a default constructor;
// the constructor that reads data/labyrinth.txt and calls your
// addRoom/connect to build the map (the parsing is provided — until
// connect()/addRoom() work, it builds an empty/partial map and `selftest
// crossing` FAILs with a diagnostic). The trivial accessors roomCount() /
// empty() / hasRoom(). The read-only adjacency() view the `map` command
// prints. And hops() — a BFS that COUNTS distance (the shortest way down),
// built on your primitives, so `descend` works the moment your BFS does. You
// do NOT write hops; you write the BFS it rides on.
//
// YOU WRITE (six small bodies, near the bottom):
//   Monday    — addRoom, connect, neighbors
//               (build the map: rooms are vertices, passages are edges)
//   Wednesday — reachable, bfsOrder
//               (cross it breadth-first, reusing your Floor 7 Queue + a
//                visited set — never revisit, so a cycle cannot loop forever)
//   Friday    — dfsOrder
//               (cross it depth-first, reusing your Floor 6 Stack; same map,
//                opposite order — LIFO instead of FIFO)
//
// =====================================================================
// Two traps to notice now
// =====================================================================
// (1) The visited set is what stops a CYCLE from looping forever. A graph is
//     not a tree — it has no leaves to stop you. Follow the passages of a
//     loop A-B-C-A naively and you walk it endlessly; the program does not
//     crash, it simply never returns. Mark each room the FIRST time you see
//     it and never enqueue/push it again. (The visited set is the hash set
//     you met on Floor 11 — std::unordered_set<std::string>.)
// (2) neighbors must NOT insert on a missing key. std::unordered_map's
//     operator[] CREATES a default entry when the key is absent — the
//     Phantom-Key bug from Floor 10, now on the graph. Use find, not
//     operator[], and keep neighbors const.

#pragma once

#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Queue.h"   // your Floor 7 FIFO — BFS rides this
#include "Stack.h"   // your Floor 6 LIFO — DFS rides this

namespace dungeon {

class Graph {
public:
    // ----- construction (PROVIDED, complete) -------------------------
    // An empty Labyrinth — no rooms, no passages.
    Graph() = default;

    // Build the map from data/labyrinth.txt. The parsing is provided; the
    // BUILDING is yours. Each data line is either:
    //   * an edge — two room names separated by `|` (e.g. "Goblin | Wraith"),
    //     which calls connect(a, b); or
    //   * a single room name on its own line, which calls addRoom(room) — used
    //     for a room that has no passages (a deliberately ISOLATED room, so
    //     `reach`-ing it returns false).
    // '#'-comment lines and blanks are ignored. Until your connect()/addRoom()
    // work (Monday), this loop runs but builds an empty/partial map, so
    // `selftest crossing` FAILs and `map` prints the dark-Labyrinth guard.
    explicit Graph(const std::string& dataPath) {
        std::ifstream in(dataPath);
        if (!in) return;   // missing file -> empty graph; map/selftest report it
        std::string line;
        while (std::getline(in, line)) {
            // Trim trailing CR so a CRLF data file parses cleanly.
            if (!line.empty() && line.back() == '\r') line.pop_back();
            // Skip blanks and comments.
            std::size_t firstNonSpace = line.find_first_not_of(" \t");
            if (firstNonSpace == std::string::npos) continue;
            if (line[firstNonSpace] == '#') continue;

            // Split on the first '|'. No '|' => the whole line is one room name.
            std::size_t bar = line.find('|');
            if (bar == std::string::npos) {
                std::string room = trim(line);
                if (!room.empty()) addRoom(room);
            } else {
                std::string a = trim(line.substr(0, bar));
                std::string b = trim(line.substr(bar + 1));
                if (!a.empty() && !b.empty()) connect(a, b);
            }
        }
    }

    // ----- trivial accessors (PROVIDED, complete) --------------------
    std::size_t roomCount() const { return adj_.size(); }
    bool        empty()     const { return adj_.empty(); }
    bool        hasRoom(const std::string& r) const { return adj_.count(r) > 0; }

    // Read-only view of the adjacency list, so the `map` command can print
    // each room and its doors (the §13.3 representation made visible).
    const std::unordered_map<std::string, std::vector<std::string>>& adjacency() const {
        return adj_;
    }

    // ----- shortest path (PROVIDED, complete) ------------------------
    // The "shortest way down" the Floor 11 cliffhanger promised. A BFS that
    // tracks DISTANCE: the number of doors from `from` to `to`, or -1 if `to`
    // is unreachable. Because the graph is UNWEIGHTED, the level at which BFS
    // first reaches a room IS its shortest hop-distance (§13.1 distance +
    // §13.7 BFS, joined). The off-by-one is pinned here: the start room is 0
    // doors from itself; an adjacent room is 1.
    //
    // This rides on YOUR neighbors() — so it returns -1 (everything
    // unreachable) until your connect()/neighbors() are written, and starts
    // giving real distances the moment your Monday work lands. You do not
    // write this; you write the BFS it is built from (reachable / bfsOrder).
    int hops(const std::string& from, const std::string& to) const {
        if (!hasRoom(from) || !hasRoom(to)) return -1;
        if (from == to) return 0;
        std::unordered_map<std::string, int> dist;
        Queue<std::string> frontier;
        frontier.enqueue(from);
        dist[from] = 0;
        while (!frontier.empty()) {
            const std::string here = frontier.front();
            frontier.dequeue();
            for (const std::string& next : neighbors(here)) {
                if (dist.count(next)) continue;        // already discovered
                dist[next] = dist[here] + 1;
                if (next == to) return dist[next];
                frontier.enqueue(next);
            }
        }
        return -1;   // drained the frontier without reaching `to`
    }

    // ===== YOU WRITE — the six method bodies =========================
    // Each ships as a stub that COMPILES but returns an empty/false/no-op
    // answer, so `selftest crossing` FAILs with a diagnostic rather than
    // failing to build (same pattern as the Floor 9 BST / Floor 10 Registry /
    // Floor 11 HashTable stubs). The stubs never crash: an empty adj_ and
    // empty returns are always valid. Fill them in MWF.

    // ----- Monday: build the map (rooms are vertices, passages are edges) ----

    // Ensure `room` is a vertex in the graph. If it is not already a key in
    // adj_, create it with an EMPTY neighbor list; if it already exists, do
    // nothing (idempotent — adding an existing room must not wipe its doors).
    // A vertex is just a key with a (possibly empty) adjacency list (§13.3).
    //
    // TODO Floor 12 (Monday). Sketch:
    //     if (!adj_.count(room)) adj_[room] = {};
    //  (or simply `adj_[room];`, which default-constructs an empty vector for a
    //   new key and leaves an existing one untouched.)
    // Stub is a no-op, so roomCount() stays 0 and phase 1 FAILs.
    void addRoom(const std::string& room) {
        (void)room;   // TODO Monday — ensure `room` is a key in adj_ (idempotent)
    }

    // Add an UNDIRECTED edge between `a` and `b`: make sure both rooms exist,
    // then record the passage in BOTH directions. A door you walk through you
    // can walk back through (§13.5 contrast), so the edge is symmetric — `b`
    // goes in `a`'s neighbor list AND `a` goes in `b`'s. The single most
    // common bug on this floor is connecting only one direction.
    //
    // TODO Floor 12 (Monday). Sketch:
    //     addRoom(a); addRoom(b);
    //     adj_[a].push_back(b);
    //     adj_[b].push_back(a);
    // Stub is a no-op, so no edges exist and phases 2–6 FAIL.
    void connect(const std::string& a, const std::string& b) {
        (void)a; (void)b;   // TODO Monday — add BOTH directions of an undirected edge
    }

    // Return the rooms directly connected to `room` (a copy of its adjacency
    // list), or an empty vector if `room` has no doors or is not on the map.
    // The §13.3 "list of adjacent vertices." Used by both traversals, by the
    // provided hops(), and by the `doors` command.
    //
    // Must be const, and must NOT insert on a missing key: adj_.operator[]
    // would CREATE an empty entry for an absent room (the Phantom-Key bug from
    // Floor 10). Use find, not operator[].
    //
    // TODO Floor 12 (Monday). Sketch:
    //     auto it = adj_.find(room);
    //     if (it == adj_.end()) return {};
    //     return it->second;
    // Stub returns {}, so phases 3–6 FAIL.
    std::vector<std::string> neighbors(const std::string& room) const {
        (void)room;
        return {};   // TODO Monday — return room's adjacency list (find, not [])
    }

    // ----- Wednesday: cross it breadth-first (reuse the Queue) --------

    // Can you get from `from` to `to` following passages? A breadth-first
    // search reusing your Floor 7 Queue<std::string> and a visited set (the
    // hash set from Floor 11, std::unordered_set<std::string>):
    //   enqueue `from` and mark it visited; while the queue is non-empty,
    //   dequeue a room; if it is `to`, return true; otherwise enqueue each
    //   unvisited neighbor, marking it visited AS YOU ENQUEUE (so a room never
    //   enters the queue twice). Return false if the queue drains.
    // The visited set is what keeps a cycle from looping forever — mark BEFORE
    // you enqueue, check membership BEFORE you add to the frontier.
    //
    // TODO Floor 12 (Wednesday). Sketch:
    //     if (!hasRoom(from) || !hasRoom(to)) return false;
    //     Queue<std::string> frontier;
    //     std::unordered_set<std::string> visited;
    //     frontier.enqueue(from); visited.insert(from);
    //     while (!frontier.empty()) {
    //         std::string here = frontier.front(); frontier.dequeue();
    //         if (here == to) return true;
    //         for (const std::string& next : neighbors(here))
    //             if (!visited.count(next)) { visited.insert(next); frontier.enqueue(next); }
    //     }
    //     return false;
    // Stub returns false, so phase 4 FAILs.
    bool reachable(const std::string& from, const std::string& to) const {
        (void)from; (void)to;
        return false;   // TODO Wednesday — BFS from `from`, visited set, find `to`
    }

    // Return the rooms in BREADTH-FIRST visit order from `start` — start, then
    // every room one door away, then two doors away, and so on — each room
    // exactly ONCE (no repeats, even across a cycle). Same Queue + visited-set
    // machinery as reachable, but instead of stopping at a target you record
    // every room into an output vector as you VISIT it (dequeue it). This is
    // the order that proves BFS goes level by level, and it is the order the
    // provided hops() counts.
    //
    // TODO Floor 12 (Wednesday). Sketch:
    //     std::vector<std::string> order;
    //     if (!hasRoom(start)) return order;
    //     Queue<std::string> frontier;
    //     std::unordered_set<std::string> visited;
    //     frontier.enqueue(start); visited.insert(start);
    //     while (!frontier.empty()) {
    //         std::string here = frontier.front(); frontier.dequeue();
    //         order.push_back(here);
    //         for (const std::string& next : neighbors(here))
    //             if (!visited.count(next)) { visited.insert(next); frontier.enqueue(next); }
    //     }
    //     return order;
    // Stub returns {}, so phase 5 FAILs.
    std::vector<std::string> bfsOrder(const std::string& start) const {
        (void)start;
        return {};   // TODO Wednesday — BFS visit order, each room once
    }

    // ----- Friday: cross it depth-first (reuse the Stack) ------------

    // Return the rooms in DEPTH-FIRST visit order from `start` — plunge down
    // one path to its end before backtracking — each room exactly ONCE, using
    // your Floor 6 Stack<std::string> (NOT recursion: the cliffhanger promised
    // "the Stack you built becomes depth-first"). The shape:
    //   push `start`; while the stack is non-empty, pop a room; if it is
    //   already visited, skip it; otherwise mark it visited, record it, and
    //   push each of its neighbors (the unvisited ones).
    // Same visited set as BFS — the only difference between the two traversals
    // is the container: a Stack (LIFO) goes deep, a Queue (FIFO) goes level by
    // level. That is the Stack-vs-Queue contrast from Floors 6–7, made into
    // two different walks of the same map.
    //
    // TODO Floor 12 (Friday). Sketch:
    //     std::vector<std::string> order;
    //     if (!hasRoom(start)) return order;
    //     Stack<std::string> frontier;
    //     std::unordered_set<std::string> visited;
    //     frontier.push(start);
    //     while (!frontier.empty()) {
    //         std::string here = frontier.top(); frontier.pop();
    //         if (visited.count(here)) continue;
    //         visited.insert(here);
    //         order.push_back(here);
    //         for (const std::string& next : neighbors(here))
    //             if (!visited.count(next)) frontier.push(next);
    //     }
    //     return order;
    // Stub returns {}, so phase 6 FAILs.
    std::vector<std::string> dfsOrder(const std::string& start) const {
        (void)start;
        return {};   // TODO Friday — DFS visit order over the Stack, each room once
    }

private:
    // The adjacency list: each room name maps to the list of rooms one passage
    // away. The §13.3 representation in one declaration. std::unordered_map and
    // std::vector are both RAII, so copy/destructor are compiler-supplied.
    std::unordered_map<std::string, std::vector<std::string>> adj_;

    // Strip leading/trailing spaces and tabs from a room name parsed out of
    // the data file, so "Goblin | Wraith" yields "Goblin" and "Wraith", not
    // "Goblin " and " Wraith". (Loader helper — not part of the graph API.)
    static std::string trim(const std::string& s) {
        std::size_t a = s.find_first_not_of(" \t");
        if (a == std::string::npos) return {};
        std::size_t b = s.find_last_not_of(" \t");
        return s.substr(a, b - a + 1);
    }
};

}  // namespace dungeon
