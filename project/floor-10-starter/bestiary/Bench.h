// COMP 2450 — Floor 1 starter
// bestiary/Bench.h — provided by the framework. Do not edit.
//
// The benchmark harness for Lab 1. Generates a synthetic bestiary at the
// requested size, sorts it, and times all three of your search functions
// against the worst-case query (last element) and an absent query.
#pragma once
#include <cstddef>

namespace dungeon {

// Run the benchmark for one bestiary size. Prints two rows to stdout:
// one for the "last element" query, one for the "absent" query.
// Each search is executed `iterations` times and the per-call average
// (in microseconds) is reported.
void runBenchmark(std::size_t n, std::size_t iterations = 1000);

// Run the default sweep: N = 10, 100, 1'000, 10'000, 100'000.
// Useful for a single-command Lab 1 deliverable.
void runBenchmarkSweep();

// ---- Floor 9: linear scan vs BST<Monster> lookup ----------------------
// Builds a synthetic bestiary, then times a linear scan against a
// BST<Monster> lookup on the worst-case (last) and absent queries. The
// tree is built from SHUFFLED inserts so it doesn't degenerate into a
// stick. Prints two rows, same format as runBenchmark.
void runBSTBenchmark(std::size_t n, std::size_t iterations = 1000);

// Floor 9 sweep: N = 100, 1'000, 10'000, 100'000.
void runBSTBenchmarkSweep();

// ---- Floor 9½: plain BST (sorted insert) vs std::set ------------------
// Builds the SAME sorted input into a plain BST<Monster> (which degenerates
// into a stick) and a std::set<std::string> (which stays balanced), then
// times the worst-case lookup on each. Prints one row per N: the plain BST's
// height + find time vs std::set's find time.
void runBalanceBenchmark(std::size_t n, std::size_t iterations = 1000);

// Floor 9½ sweep: N = 10, 100, 1'000 (kept modest — a sorted-insert stick
// is as deep as a recursion).
void runBalanceBenchmarkSweep();

// ---- Floor 10: std::map lookup vs BST lookup vs linear scan -----------
// Builds the SAME synthetic bestiary as a std::map<std::string,Monster> (built
// INLINE, so it does not depend on the student's Registry), a shuffled
// BST<Monster>, and a sorted std::vector, then races the worst-case (last) and
// absent lookups across all three. Prints two rows, one per query.
void runNamesBenchmark(std::size_t n, std::size_t iterations = 1000);

// Floor 10 sweep: N = 100, 1'000, 10'000, 100'000.
void runNamesBenchmarkSweep();

}
