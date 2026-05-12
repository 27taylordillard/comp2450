// COMP 2450 — Floor 4 starter
// hero/ChainTests.h — the `selftest chain` command. Pre-built and provided.
//
// Verifies, without needing a leak detector, that every Node ctor was
// matched by a Node dtor by the time the test scope ends. The diagnostic
// counters live on detail::NodeStats inside Chain.h.
//
// Used by main.cpp's `selftest chain` command, which is your primary
// debugging tool on Wednesday when you're writing the destructor.

#pragma once

namespace dungeon {

// Run the chain self-test. Prints a one-line summary of allocations,
// deallocations, and leaked nodes. Returns true on success (zero leak).
bool runChainSelfTest();

}  // namespace dungeon
