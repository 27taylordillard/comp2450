// COMP 2450 — Floor 4 starter
// hero/ChainBench.cpp — benchmark harness. Provided. You do not edit this.
//
// The whole point of this file is to make the cost of push_front *visible*
// at a scale your eyes can read. At N = 100,000 the chain is roughly a
// thousand times faster than the vector — that is not a typo, it is what
// O(1) per insert vs O(n) per insert looks like. Read your terminal.
//
// Why std::vector<std::string> and not Bag<std::string>: Bag<T> is a
// thin wrapper over std::vector<T>, so the cost is identical. We use the
// vector directly so it is obvious to the reader what container we are
// poking — the underlying contiguous-array insert is the lesson, not the
// wrapper.

#include "ChainBench.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Chain.h"

namespace dungeon {

namespace {

using Clock = std::chrono::steady_clock;

double millisSince(Clock::time_point start) {
    auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

double timeChainPushFront(std::size_t n) {
    Chain<std::string> c;
    auto start = Clock::now();
    for (std::size_t i = 0; i < n; ++i) {
        c.push_front("event " + std::to_string(i));
    }
    return millisSince(start);
}

double timeVectorInsertBegin(std::size_t n) {
    std::vector<std::string> v;
    auto start = Clock::now();
    for (std::size_t i = 0; i < n; ++i) {
        v.insert(v.begin(), "event " + std::to_string(i));
    }
    return millisSince(start);
}

}  // anonymous namespace

void runChainBenchmark(std::size_t n) {
    if (n == 0) { runChainBenchmarkSweep(); return; }

    const double tChain  = timeChainPushFront(n);
    const double tVector = timeVectorInsertBegin(n);

    std::cout << "  N=" << std::setw(7) << std::right << n
              << "   Chain::push_front = " << std::setw(8) << std::right
              << std::fixed << std::setprecision(2) << tChain  << " ms"
              << "   Bag::insert(begin) = " << std::setw(8) << std::right
              << std::fixed << std::setprecision(2) << tVector << " ms\n";
    std::cout.unsetf(std::ios::fixed);
    std::cout.unsetf(std::ios::right);
}

void runChainBenchmarkSweep() {
    std::cout << "  (linked O(1) per insert vs vector O(n) per insert)\n";
    runChainBenchmark(1000);
    runChainBenchmark(10000);
    runChainBenchmark(100000);
}

}  // namespace dungeon
