// COMP 2450 — Floor 5 starter
// hero/IteratorTests.cpp — the `selftest iterator` harness. Provided. You do not edit this.
//
// Runs four phases against Chain<int>::iterator and const_iterator. Each
// phase prints OK or FAIL with a one-line diagnostic that names the
// likely culprit (which operator is still stubbed). The phases:
//
//   1. range-for + count               — needs operator++ / != / *
//   2. std::find for a known value     — needs operator++ / == / *
//   3. std::distance(begin, end)       — needs operator++ / !=
//   4. range-for on const ref          — needs const_iterator and cbegin/cend
//   5. std::reverse + spot check       — needs operator-- (Friday)
//
// On Monday morning, before any iterator work is in, every phase FAILs
// with "begin == end (stub returns true)" because the stub operator==
// makes begin trivially equal to end and the loops skip. That's the
// expected starting state — your Monday job is to fix it.

#include "IteratorTests.h"

#include <algorithm>
#include <iostream>
#include <iterator>

#include "Chain.h"

namespace dungeon {

namespace {

constexpr int kCount = 100;

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

}  // anonymous namespace

bool runIteratorSelfTest() {
    Chain<int> c;
    for (int i = 0; i < kCount; ++i) c.push_back(i);

    bool all = true;

    // --- Phase 1: range-for + count ---
    {
        int n = 0;
        int last = -1;
        for (int v : c) { ++n; last = v; }
        const bool ok = (n == kCount && last == kCount - 1);
        result("range-for over Chain<int>", ok,
               n == 0 ? "begin == end (stub returns true) — implement operator++ and operator=="
                      : "walked wrong number of nodes — check operator++ advances p_->next");
        all = all && ok;
    }

    // --- Phase 2: std::find for a known value ---
    {
        auto it = std::find(c.begin(), c.end(), 42);
        const bool ok = (it != c.end() && *it == 42);
        result("std::find(Chain<int>, 42)", ok,
               "std::find returned end() — likely operator++ stub or operator== stub");
        all = all && ok;
    }

    // --- Phase 3: std::distance ---
    {
        auto d = std::distance(c.begin(), c.end());
        const bool ok = (d == kCount);
        result("std::distance(begin, end)", ok,
               "expected 100 — got 0 means begin == end immediately (operator== stub)");
        all = all && ok;
    }

    // --- Phase 4: range-for on const ref (exercises const_iterator) ---
    {
        const Chain<int>& cref = c;
        int n = 0;
        for (const int& v : cref) { ++n; (void)v; }
        const bool ok = (n == kCount);
        result("range-for over const Chain<int>&", ok,
               "const_iterator not yet wired — Wednesday's work");
        all = all && ok;
    }

    // --- Phase 5: std::reverse + spot check ---
    // std::reverse needs bidirectional — calls operator-- on the iterator.
    // We guard *r.begin() with `begin() != end()` to avoid dereferencing
    // the stub iterator (p_ == nullptr) before operator++/operator== are
    // implemented; with the stubs in place, the guard short-circuits and
    // we just report FAIL without crashing.
    {
        Chain<int> r;
        for (int i = 0; i < 10; ++i) r.push_back(i);   // 0,1,2,...,9
        std::reverse(r.begin(), r.end());
        bool ok = false;
        if (!r.empty() && r.begin() != r.end()) {
            ok = (*r.begin() == 9);
        }
        result("std::reverse(Chain<int>) — first now == 9", ok,
               "operator-- not yet wired (Friday) — std::reverse can't walk back");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
