// COMP 2450 — Floor 4 starter
// hero/ChainBench.h — the `benchmark log` harness. Pre-built and provided.
//
// Times Chain<std::string>::push_front against std::vector<std::string>::
// insert(begin(), v) for the same N. They produce identical end-states
// (the latest value at the front); the cost difference is the lesson.

#pragma once

#include <cstddef>

namespace dungeon {

// Run the prepend benchmark for size N.
// Reports milliseconds for Chain::push_front vs vector::insert(begin()).
void runChainBenchmark(std::size_t n);

// Run a sweep: N = 1000, 10000, 100000.
void runChainBenchmarkSweep();

}  // namespace dungeon
