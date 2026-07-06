// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// bestiary/Bench.cpp — Floor 1's search benchmark, kept as a teaching
// reference. Builds its own synthetic std::vector<Monster> internally
// so it can call the Floor 1 reference searches that still take
// std::vector<Monster>. Sorts the synthetic data directly with std::sort
// — sortBestiary post-Floor-3 takes a Bag<Monster>& and is no longer
// the right tool here.

#include "Bench.h"
#include "Bestiary.h"
#include "Search.h"

#include "../hero/BST.h"
#include "../hero/Bag.h"
#include "../hero/HashTable.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace dungeon {

namespace {

// Build a synthetic bestiary of N monsters with names "Monster_0000001",
// "Monster_0000002", ... so lexicographic order matches numeric order.
// All stats are placeholders; this bestiary is for timing only.
std::vector<Monster> makeSynthetic(std::size_t n) {
    std::vector<Monster> v;
    v.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "Monster_" << std::setfill('0') << std::setw(7) << i;
        v.push_back({oss.str(), 10, 2, "fire"});
    }
    return v;
}

// `count` PRESENT names to query in the "hit" row, sampled UNIFORMLY across the
// whole synthetic bestiary (not just the front) and shuffled. Two reasons this
// matters: (1) the queried key changes EVERY iteration, so the lookup can't be
// hoisted out of the timing loop (see avgMicros); (2) the targets are spread
// across the entire key range, so linear search's average cost grows with N
// (querying only the first few names would let linear "win" by always finding
// the target near the front). One distinct key per iteration, so there is no
// short repeating cycle the optimizer could precompute.
std::vector<std::string> makeHitKeys(const std::vector<Monster>& monsters,
                                     std::size_t count, std::mt19937& rng) {
    std::vector<std::string> keys;
    keys.reserve(count);
    const std::size_t n = monsters.size();
    for (std::size_t i = 0; i < count; ++i) {
        // Spread the samples across [0, n): evenly-strided, then jittered so the
        // set isn't a rigid arithmetic progression.
        std::size_t idx = (n == 0) ? 0 : (i * 2654435761u + (rng() % n)) % n;
        keys.push_back(monsters[idx].name);
    }
    std::shuffle(keys.begin(), keys.end(), rng);
    return keys;
}

// `count` genuinely-ABSENT names to query in the "absent" row. Each sorts AFTER
// every synthetic "Monster_*" name, so every lookup is a true miss (a full walk
// for linear, a full root-to-leaf descent for the trees, a full bucket-probe for
// the hash maps). They all DIFFER, one per iteration, so the miss can't be
// hoisted or precomputed either.
std::vector<std::string> makeMissKeys(std::size_t count) {
    std::vector<std::string> keys;
    keys.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::ostringstream oss;
        oss << "ZZZZZ_NO_SUCH_" << std::setfill('0') << std::setw(7) << i;
        keys.push_back(oss.str());
    }
    return keys;
}

// A volatile sink the compiler cannot prove unused.
static volatile const void* g_benchSink = nullptr;

// Force `value` to be materialized and treated as observable, so the optimizer
// cannot delete the computation that produced it. This is the standard
// benchmark "DoNotOptimize" barrier (the same trick Google Benchmark uses).
// On GCC/Clang the inline-asm pretends to read `value` and to clobber memory,
// which is opaque to the optimizer. We need it because a plain volatile store
// is NOT always enough: on this toolchain GCC can fully inline a std::map /
// hash lookup and then prove the whole timing loop is "pure," collapsing it to
// a hoisted 0.000 us even when the queried key varies. MSVC has no GCC-style
// inline asm on x64, so there we force a volatile read of `value`'s first byte
// (volatile accesses are never deleted) plus a compiler-level fence. Either
// way, every iteration's lookup stays genuinely observable, so the timing
// reflects real work.
template <typename T>
inline void doNotOptimize(const T& value) {
#if defined(_MSC_VER) && !defined(__clang__)
    (void)*reinterpret_cast<const volatile char*>(&value);
    std::atomic_signal_fence(std::memory_order_seq_cst);
#else
    asm volatile("" : : "r,m"(value) : "memory");
#endif
}

// Time a lookup callable across `iterations` iterations and return the average
// microseconds. The callable receives the iteration counter `i` so the caller
// can VARY the queried key every iteration (e.g. keys[i]).
//
// Two anti-optimization measures work together here:
//   (1) `i` lets the caller feed a DIFFERENT key each iteration, so the lookup
//       is not loop-invariant (a fixed key would be hoisted, computed once, and
//       the per-iteration cost would collapse to 0.000 us).
//   (2) doNotOptimize() on each result is the belt-and-suspenders barrier that
//       stops GCC from deleting an inlined lookup whose result it could prove
//       unused — see the note on doNotOptimize above.
template <typename F>
double avgMicros(F fn, std::size_t iterations) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < iterations; ++i) {
        auto result = fn(i);
        doNotOptimize(result);
        g_benchSink = result;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double total = std::chrono::duration<double, std::micro>(t1 - t0).count();
    return total / static_cast<double>(iterations);
}

void printRow(std::size_t n, const std::string& which,
              double l, double b, double r) {
    std::cout << "  N=" << std::setw(7) << n
              << "  query=" << std::setw(6) << std::left << which << std::right
              << "  linear=" << std::setw(10) << std::fixed << std::setprecision(3) << l << " us"
              << "  binary=" << std::setw(8) << b << " us"
              << "  recursive=" << std::setw(8) << r << " us"
              << "\n";
}

}  // anonymous namespace

void runBenchmark(std::size_t n, std::size_t iterations) {
    auto bestiary = makeSynthetic(n);
    // Pre-Floor-3 this called sortBestiary(bestiary). Post-Floor-3
    // sortBestiary takes a Bag<Monster>&; the bench keeps using
    // std::vector<Monster> internally to match the Floor 1 reference
    // searches' signatures, so we call std::sort directly.
    std::sort(bestiary.begin(), bestiary.end(),
              [](const Monster& a, const Monster& b) { return a.name < b.name; });

    const std::string last_name   = bestiary.back().name;
    const std::string absent_name = "ZZZZZ_NO_SUCH_MONSTER";

    auto runOnce = [&](const std::string& target) {
        double l = avgMicros([&](std::size_t){ return linearSearch          (bestiary, target); }, iterations);
        double b = avgMicros([&](std::size_t){ return binarySearch          (bestiary, target); }, iterations);
        double r = avgMicros([&](std::size_t){ return binarySearchRecursive(bestiary, target); }, iterations);
        return std::tuple<double,double,double>{l, b, r};
    };

    auto [l1, b1, r1] = runOnce(last_name);
    printRow(n, "last",   l1, b1, r1);

    auto [l2, b2, r2] = runOnce(absent_name);
    printRow(n, "absent", l2, b2, r2);
}

void runBenchmarkSweep() {
    std::cout << "\n-- Race the Bestiary (1000 iterations per cell) --\n";
    for (std::size_t n : {std::size_t{10}, std::size_t{100}, std::size_t{1000},
                          std::size_t{10000}, std::size_t{100000}}) {
        runBenchmark(n);
    }
    std::cout << "\nNotice the curve: linear grows with N, binary grows with log N.\n"
              << "For tiny N the constants matter and the gap is small.\n"
              << "For large N the gap is enormous.\n\n";
}

void runBSTBenchmark(std::size_t n, std::size_t iterations) {
    auto monsters = makeSynthetic(n);
    std::sort(monsters.begin(), monsters.end(),
              [](const Monster& a, const Monster& b) { return a.name < b.name; });

    // Build the BST from a SHUFFLED copy. Inserting already-sorted data
    // would build a stick of depth N — pathologically slow AND deep enough
    // to overflow the recursive insert/find at large N. A shuffle gives a
    // realistic ~log N tree, which is the honest comparison against linear.
    auto shuffled = monsters;
    std::mt19937 rng(2450);
    std::shuffle(shuffled.begin(), shuffled.end(), rng);
    BST<Monster> tree;
    for (const Monster& m : shuffled) tree.insert(m);

    auto raceRow = [&](const char* which, const std::string& target) {
        Monster probe;
        probe.name = target;
        double l = avgMicros([&](std::size_t){ return linearSearch(monsters, target); }, iterations);
        double t = avgMicros([&](std::size_t){ return tree.find(probe); }, iterations);
        std::cout << "  N=" << std::setw(7) << n
                  << "  query=" << std::setw(6) << std::left << which << std::right
                  << "  linear=" << std::setw(10) << std::fixed << std::setprecision(3) << l << " us"
                  << "  BST=" << std::setw(10) << t << " us"
                  << "  (tree height " << tree.height() << ")\n";
    };
    raceRow("last",   monsters.back().name);
    raceRow("absent", "ZZZZZ_NO_SUCH_MONSTER");
}

void runBSTBenchmarkSweep() {
    std::cout << "\n-- Bestiary: linear scan vs BST lookup (1000 iterations per cell) --\n";
    for (std::size_t n : {std::size_t{100}, std::size_t{1000},
                          std::size_t{10000}, std::size_t{100000}}) {
        runBSTBenchmark(n);
    }
    std::cout << "\nLinear scan grows with N; BST lookup grows with log N — the same\n"
              << "win binary search gave you on Floor 1, but now the structure STAYS\n"
              << "searchable as you insert and remove, with no re-sort step.\n"
              << "One catch: this tree was built from SHUFFLED data. Insert the\n"
              << "monsters already sorted and the BST degenerates into a stick\n"
              << "(height ~N), and lookup drops right back to linear — the problem\n"
              << "the next floor's self-balancing tree exists to solve.\n\n";
}

// ---- Floor 9.5: plain BST (sorted insert -> stick) vs std::set --------
void runBalanceBenchmark(std::size_t n, std::size_t iterations) {
    auto monsters = makeSynthetic(n);   // names already in sorted order

    // Plain BST<Monster>, inserted in SORTED order -> a right-leaning stick
    // of height n-1. We keep n modest here precisely because a stick this
    // deep is also a recursion this deep.
    BST<Monster> stick;
    for (const Monster& m : monsters) stick.insert(m);

    // std::set<std::string> over the SAME names -> a balanced (red-black)
    // tree that rebalances on every insert, so it never becomes a stick.
    std::set<std::string> balanced;
    for (const Monster& m : monsters) balanced.insert(m.name);

    const std::string target = monsters.back().name;   // worst case for the stick
    Monster probe;
    probe.name = target;

    double b = avgMicros([&](std::size_t){ return stick.find(probe); }, iterations);
    double s = avgMicros([&](std::size_t){
        auto it = balanced.find(target);
        return it != balanced.end() ? static_cast<const void*>(&*it) : nullptr;
    }, iterations);

    std::cout << "  N=" << std::setw(7) << n
              << "  plain BST: height=" << std::setw(6) << stick.height()
              << "  find=" << std::setw(9) << std::fixed << std::setprecision(3) << b << " us"
              << "    std::set: find=" << std::setw(8) << s << " us\n";
}

void runBalanceBenchmarkSweep() {
    std::cout << "\n-- Same sorted input: your plain BST vs std::set (1000 iterations per cell) --\n";
    for (std::size_t n : {std::size_t{10}, std::size_t{100}, std::size_t{1000}}) {
        runBalanceBenchmark(n);
    }
    std::cout << "\nSame data, same order. Inserted in sorted order, your plain BST\n"
              << "degenerated into a stick of height N-1 — lookup is back to O(n).\n"
              << "std::set saw the identical input and stayed shallow: it is a\n"
              << "balanced tree (a red-black tree) that rotates on every insert to\n"
              << "hold its height near log2 N. You do not hand-write that balancing —\n"
              << "you reach for std::set / std::map. That is the next floor, the\n"
              << "Hall of Names.\n\n";
}

// ---- Floor 10: std::map lookup vs BST lookup vs linear scan -----------
// The Hall of Names payoff, as a race. We build the SAME synthetic bestiary
// three ways and time the worst-case (last) and absent lookups:
//   * std::map<std::string, Monster> — keyed by name. Built INLINE here (not
//     via the student's Registry) so this command works from day one,
//     independent of the Registry stubs — same approach as benchmark balance.
//   * BST<Monster> — built from SHUFFLED inserts so it stays ~log N deep (an
//     honest comparison, not a degenerate stick).
//   * linearSearch over the vector — Floor 1's scan, for contrast.
// The lesson: std::map matches the balanced tree and crushes the scan, and you
// wrote one line (std::map<std::string,Monster>) to get it.
void runNamesBenchmark(std::size_t n, std::size_t iterations) {
    auto monsters = makeSynthetic(n);
    std::sort(monsters.begin(), monsters.end(),
              [](const Monster& a, const Monster& b) { return a.name < b.name; });

    // std::map keyed by name — built inline, stub-independent.
    std::map<std::string, Monster> byName;
    for (const Monster& m : monsters) byName.emplace(m.name, m);

    // BST<Monster> from SHUFFLED inserts so it doesn't degenerate into a stick.
    auto shuffled = monsters;
    std::mt19937 rng(2450);
    std::shuffle(shuffled.begin(), shuffled.end(), rng);
    BST<Monster> tree;
    for (const Monster& m : shuffled) tree.insert(m);

    // Probe keys that VARY per iteration so the lookups can't be hoisted out of
    // the timing loop (see avgMicros). One distinct key per iteration. The "hit"
    // row queries PRESENT names spread across the whole range; the "absent" row
    // queries genuinely-ABSENT names.
    const std::vector<std::string> hitKeys  = makeHitKeys(monsters, iterations, rng);
    const std::vector<std::string> missKeys = makeMissKeys(iterations);

    auto raceRow = [&](const char* which, const std::vector<std::string>& keys) {
        double l = avgMicros([&](std::size_t i){
            return linearSearch(monsters, keys[i]);
        }, iterations);
        double t = avgMicros([&](std::size_t i){
            Monster probe; probe.name = keys[i];
            return tree.find(probe);
        }, iterations);
        double m = avgMicros([&](std::size_t i){
            auto it = byName.find(keys[i]);
            return it != byName.end() ? static_cast<const void*>(&it->second) : nullptr;
        }, iterations);
        std::cout << "  N=" << std::setw(7) << n
                  << "  query=" << std::setw(6) << std::left << which << std::right
                  << "  linear=" << std::setw(10) << std::fixed << std::setprecision(3) << l << " us"
                  << "  BST=" << std::setw(8) << t << " us"
                  << "  std::map=" << std::setw(8) << m << " us"
                  << "  (tree height " << tree.height() << ")\n";
    };
    raceRow("hit",    hitKeys);
    raceRow("absent", missKeys);
}

void runNamesBenchmarkSweep() {
    std::cout << "\n-- The Hall of Names: linear scan vs BST vs std::map (1000 iterations per cell) --\n";
    for (std::size_t n : {std::size_t{100}, std::size_t{1000},
                          std::size_t{10000}, std::size_t{100000}}) {
        runNamesBenchmark(n);
    }
    std::cout << "\nstd::map::find tracks the BST — both grow with log N, both crush\n"
              << "the linear scan. The difference is the code: the BST was a class you\n"
              << "wrote across a whole floor; std::map was one declaration. And unlike\n"
              << "your plain BST, std::map is balanced (a red-black tree), so it NEVER\n"
              << "degenerates into a stick — the library gives you the balanced tree of\n"
              << "Floor 9½ and a key->value index on top of it, for free.\n\n";
}

// ---- Floor 11: your HashTable vs std::unordered_map vs std::map vs BST vs linear ----
// The Hall of Whispers payoff, as a race. We build the SAME synthetic bestiary
// five ways and time the worst-case (last) and absent lookups:
//   * YOUR HashTable<Monster> — separate chaining, your hash function. Built from
//     a synthetic Bag<Monster>. THIS is the headline column: a hash table is
//     flat O(1) average and beats even the balanced tree at large N. If your
//     HashTable is still stubbed (size()==0 after loading), this column prints
//     `--` (there is nothing to time yet) while every other column still runs —
//     so the command works from day one, same as benchmark balance / names.
//   * std::unordered_map<std::string, Monster> — the LIBRARY hash table (a hash
//     table under the hood, §12.9). Built inline. It tracks your HashTable: it
//     is the library version of exactly what you built.
//   * std::map<std::string, Monster> — Floor 10's balanced-tree map (O(log n)).
//   * BST<Monster> — Floor 9's tree, built from SHUFFLED inserts so it stays
//     ~log N deep (an honest comparison, not a degenerate stick).
//   * linearSearch over the vector — Floor 1's scan, for contrast.
void runWhispersBenchmark(std::size_t n, std::size_t iterations) {
    auto monsters = makeSynthetic(n);
    std::sort(monsters.begin(), monsters.end(),
              [](const Monster& a, const Monster& b) { return a.name < b.name; });

    // YOUR HashTable — built from a synthetic Bag<Monster>. If insert() is still
    // stubbed, built.size() stays 0 and we print `--` for this column.
    Bag<Monster> bag;
    for (const Monster& m : monsters) bag.push_back(m);
    HashTable whispers(bag);
    const bool haveHashTable = (whispers.size() == n);

    // std::unordered_map — the library hash table, built inline (stub-independent).
    std::unordered_map<std::string, Monster> umap;
    umap.reserve(n * 2);
    for (const Monster& m : monsters) umap.emplace(m.name, m);

    // std::map — Floor 10's balanced-tree map, built inline.
    std::map<std::string, Monster> omap;
    for (const Monster& m : monsters) omap.emplace(m.name, m);

    // BST<Monster> from SHUFFLED inserts so it doesn't degenerate into a stick.
    auto shuffled = monsters;
    std::mt19937 rng(2450);
    std::shuffle(shuffled.begin(), shuffled.end(), rng);
    BST<Monster> tree;
    for (const Monster& m : shuffled) tree.insert(m);

    // Probe keys that VARY per iteration so the lookups can't be hoisted out of
    // the timing loop (see avgMicros). Without this, the fully-inlinable lookups
    // (std::map, and YOUR HashTable) collapse to a hoisted 0.000 us and the whole
    // race becomes meaningless. One distinct key per iteration. The "hit" row
    // queries PRESENT names spread across the whole range; the "absent" row
    // queries genuinely-ABSENT names.
    const std::vector<std::string> hitKeys  = makeHitKeys(monsters, iterations, rng);
    const std::vector<std::string> missKeys = makeMissKeys(iterations);

    auto raceRow = [&](const char* which, const std::vector<std::string>& keys) {
        double lin = avgMicros([&](std::size_t i){
            return linearSearch(monsters, keys[i]);
        }, iterations);
        double bst = avgMicros([&](std::size_t i){
            Monster probe; probe.name = keys[i];
            return tree.find(probe);
        }, iterations);
        double omp = avgMicros([&](std::size_t i){
            auto it = omap.find(keys[i]);
            return it != omap.end() ? static_cast<const void*>(&it->second) : nullptr;
        }, iterations);
        double ump = avgMicros([&](std::size_t i){
            auto it = umap.find(keys[i]);
            return it != umap.end() ? static_cast<const void*>(&it->second) : nullptr;
        }, iterations);

        std::cout << "  N=" << std::setw(7) << n
                  << "  query=" << std::setw(6) << std::left << which << std::right
                  << "  linear=" << std::setw(10) << std::fixed << std::setprecision(3) << lin << " us"
                  << "  BST=" << std::setw(8) << bst << " us"
                  << "  std::map=" << std::setw(8) << omp << " us"
                  << "  unordered_map=" << std::setw(8) << ump << " us"
                  << "  HashTable=";
        if (haveHashTable) {
            double ht = avgMicros([&](std::size_t i){
                return whispers.find(keys[i]);
            }, iterations);
            std::cout << std::setw(8) << ht << " us";
        } else {
            std::cout << std::setw(8) << "--" << "   ";
        }
        std::cout << "\n";
    };
    raceRow("hit",    hitKeys);
    raceRow("absent", missKeys);
}

void runWhispersBenchmarkSweep() {
    std::cout << "\n-- The Hall of Whispers: linear vs BST vs std::map vs unordered_map vs YOUR HashTable"
              << " (1000 iterations per cell) --\n";
    for (std::size_t n : {std::size_t{100}, std::size_t{1000},
                          std::size_t{10000}, std::size_t{100000}}) {
        runWhispersBenchmark(n);
    }
    std::cout << "\nYour HashTable is FLAT — average O(1) — and at large N it beats even the\n"
              << "balanced tree (std::map / your BST grow with log N; the hash table does not\n"
              << "grow at all). std::unordered_map tracks your HashTable because it IS a hash\n"
              << "table — the library version of exactly what you built. The trade for that\n"
              << "speed: ORDER. std::map's keys come out sorted; a hash container's do not —\n"
              << "iterate `buckets` and the names are scrambled. And the speed only holds while\n"
              << "the load factor stays low: let the chains grow long and the whisper becomes a\n"
              << "walk. (If the HashTable column shows `--`, write insert() first — see\n"
              << "`selftest whispers`.)\n\n";
}

}  // namespace dungeon
