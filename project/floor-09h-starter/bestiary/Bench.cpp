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

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <string>
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

// A volatile sink the compiler cannot prove unused.
static volatile const void* g_benchSink = nullptr;

template <typename F>
double avgMicros(F fn, std::size_t iterations) {
    auto t0 = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < iterations; ++i) {
        g_benchSink = fn();
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
        double l = avgMicros([&]{ return linearSearch          (bestiary, target); }, iterations);
        double b = avgMicros([&]{ return binarySearch          (bestiary, target); }, iterations);
        double r = avgMicros([&]{ return binarySearchRecursive(bestiary, target); }, iterations);
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
        double l = avgMicros([&]{ return linearSearch(monsters, target); }, iterations);
        double t = avgMicros([&]{ return tree.find(probe); }, iterations);
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

    double b = avgMicros([&]{ return stick.find(probe); }, iterations);
    double s = avgMicros([&]{
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

}  // namespace dungeon
