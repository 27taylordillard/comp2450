// COMP 2450 — Floor 4 starter
// hero/ChainTests.cpp — selftest harness. Provided. You do not edit this.
//
// What it does: builds a Chain<int>, push_fronts kCount values, lets the
// chain go out of scope (firing the destructor), and prints the running
// allocation/deallocation counters. If your ~Chain() walks the chain
// correctly, allocations == deallocations and leaked == 0.
//
// On Monday, before push_front is implemented, the counters will both
// be 0 and the test will trivially "pass" (zero in, zero out). That is
// a known false positive — you have not built anything yet.
//
// On Wednesday, with push_front working but ~Chain still empty, the
// test will report kCount allocations and 0 deallocations — every node
// leaked. Implement the destructor; rerun; watch the count balance.

#include "ChainTests.h"

#include <iomanip>
#include <iostream>

#include "Chain.h"

namespace dungeon {

namespace {
constexpr std::size_t kCount = 1000;
}  // anonymous namespace

bool runChainSelfTest() {
    detail::NodeStats::reset();

    {
        Chain<int> c;
        for (std::size_t i = 0; i < kCount; ++i) {
            c.push_front(static_cast<int>(i));
        }
        // c goes out of scope here; ~Chain() must run.
    }

    const std::size_t a = detail::NodeStats::allocations;
    const std::size_t d = detail::NodeStats::deallocations;
    const long long   leaked = static_cast<long long>(a) - static_cast<long long>(d);

    std::cout << "  Chain<int> allocations: " << std::setw(5) << std::right << a
              << "   deallocations: "         << std::setw(5) << std::right << d
              << "   leaked: "                << std::setw(5) << std::right << leaked;

    if (a == 0 && d == 0) {
        std::cout << "   (push_front not implemented yet — nothing was built)\n";
        return false;
    }
    if (leaked == 0) {
        std::cout << "   OK\n";
        return true;
    }
    std::cout << "   LEAK — implement ~Chain() / clear()\n";
    return false;
}

}  // namespace dungeon
