// COMP 2450 — Floor 9½ starter
// hero/BalanceTests.h — declares the `selftest balance` entry point. Provided.
#pragma once

namespace dungeon {

// Runs the Floor 9½ balance self-test (five phases): balance factor,
// isBalanced on a bushy tree vs. a sorted stick, a rotation that repairs
// structure while preserving in-order, and balanceRoot picking the correct
// single rotation. Prints one line per phase; returns true iff all pass.
bool runBalanceSelfTest();

}  // namespace dungeon
