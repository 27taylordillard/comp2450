// COMP 2450 — Floor 4½ starter
// hero/ChainTests.cpp — selftest harness. Provided. You do not edit this.
//
// Two phases:
//
// Phase 1 (Floor 4 leak check, unchanged):
//   Build a Chain<int>, push_front kCount values, let it go out of scope,
//   confirm allocations == deallocations.
//
// Phase 2 (Floor 4½ deep-copy check, NEW):
//   Build a fresh Chain<int>, copy-construct a second chain from it, let
//   the copy die first, then verify the ORIGINAL is still walkable in
//   both directions and still has kCount nodes. Let the original die.
//   Confirms total allocations == total deallocations == 2 * kCount.
//
// On Monday, before push_back / tail() / push_front-with-prev work, the
// forward and backward walks in phase 2 will print different lengths
// (or zero on the backward walk because tail() returns nullptr). That
// is the test telling you to wire prev / tail_.
//
// On Wednesday, before the deep copy ctor is implemented, phase 2 will
// either crash on double-free (if you went with the default copy ctor)
// or report a backward walk of zero on the copy (if you left the copy
// ctor body empty). That is the test telling you to write the deep copy.

#include "ChainTests.h"

#include <iomanip>
#include <iostream>

#include "Chain.h"

namespace dungeon {

namespace {

constexpr std::size_t kCount = 1000;

// Count nodes by walking head → next. O(n). Used to spot-check that
// the chain's size_ field matches the actual structure.
template <typename T>
std::size_t walkForward(const Chain<T>& c) {
    std::size_t n = 0;
    for (const auto* p = c.head(); p != nullptr; p = p->next) ++n;
    return n;
}

// Count nodes by walking tail → prev. O(n). Should equal walkForward
// on any valid chain. If they disagree, the prev pointers are broken.
template <typename T>
std::size_t walkBackward(const Chain<T>& c) {
    std::size_t n = 0;
    for (const auto* p = c.tail(); p != nullptr; p = p->prev) ++n;
    return n;
}

}  // anonymous namespace

bool runChainSelfTest() {
    detail::NodeStats::reset();

    // -------- Phase 1: leak check on a single chain (Floor 4) --------
    {
        Chain<int> c;
        for (std::size_t i = 0; i < kCount; ++i) {
            c.push_front(static_cast<int>(i));
        }
        // c goes out of scope here; ~Chain() must run.
    }

    const std::size_t a1 = detail::NodeStats::allocations;
    const std::size_t d1 = detail::NodeStats::deallocations;
    const long long   leak1 = static_cast<long long>(a1) - static_cast<long long>(d1);

    std::cout << "  Phase 1 (single chain)\n"
              << "    allocations: "   << std::setw(5) << std::right << a1
              << "   deallocations: " << std::setw(5) << std::right << d1
              << "   leaked: "        << std::setw(5) << std::right << leak1;
    if (a1 == 0 && d1 == 0) {
        std::cout << "   (push_front not implemented yet)\n";
        return false;
    }
    std::cout << (leak1 == 0 ? "   OK\n" : "   LEAK — see ~Chain() / clear()\n");
    if (leak1 != 0) return false;

    // -------- Phase 2: deep-copy + bidirectional walk (Floor 4½) --------
    detail::NodeStats::reset();

    std::size_t fwdOrigAfterCopy = 0;
    std::size_t bwdOrigAfterCopy = 0;
    std::size_t fwdCopy          = 0;
    std::size_t bwdCopy          = 0;

    {
        Chain<int> original;
        for (std::size_t i = 0; i < kCount; ++i) {
            original.push_back(static_cast<int>(i));
        }

        {
            Chain<int> copy(original);   // deep copy ctor — Wednesday's work
            fwdCopy = walkForward(copy);
            bwdCopy = walkBackward(copy);
            // copy dies here. If the deep copy is wrong, this destructor
            // may either be a no-op (empty copy body) or a double-free
            // (default copy ctor). Either way, the next two lines will
            // detect the damage.
        }

        fwdOrigAfterCopy = walkForward(original);
        bwdOrigAfterCopy = walkBackward(original);
    }

    const std::size_t a2 = detail::NodeStats::allocations;
    const std::size_t d2 = detail::NodeStats::deallocations;
    const long long   leak2 = static_cast<long long>(a2) - static_cast<long long>(d2);

    std::cout << "  Phase 2 (deep copy)\n"
              << "    original after copy died — forward walk: "
              << std::setw(5) << std::right << fwdOrigAfterCopy
              << "   backward walk: " << std::setw(5) << std::right << bwdOrigAfterCopy << "\n"
              << "    copy before death        — forward walk: "
              << std::setw(5) << std::right << fwdCopy
              << "   backward walk: " << std::setw(5) << std::right << bwdCopy << "\n"
              << "    allocations: " << std::setw(5) << std::right << a2
              << "   deallocations: " << std::setw(5) << std::right << d2
              << "   leaked: " << std::setw(5) << std::right << leak2;

    const bool fwdOk   = (fwdOrigAfterCopy == kCount && fwdCopy == kCount);
    const bool bwdOk   = (bwdOrigAfterCopy == kCount && bwdCopy == kCount);
    const bool allocOk = (a2 == 2 * kCount && d2 == 2 * kCount);

    if (fwdOk && bwdOk && allocOk) {
        std::cout << "   OK\n";
        return true;
    }
    std::cout << "   FAIL —";
    if (!fwdOk)   std::cout << " forward walk wrong (push_back?);";
    if (!bwdOk)   std::cout << " backward walk wrong (prev / tail_?);";
    if (!allocOk) std::cout << " allocation mismatch (deep copy?);";
    std::cout << "\n";
    return false;
}

}  // namespace dungeon
