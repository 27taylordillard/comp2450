// COMP 2450 — Floor 6 starter
// hero/StackTests.h — the `selftest stack` command. Provided.
//
// Exercises Stack<int> through five phases. Each phase prints OK / FAIL
// with a one-line diagnostic so you can tell which operator is still
// stubbed. Phases short-circuit: once one fails, the rest still run
// but they don't crash on undefined behavior — every check that would
// dereference an empty stack is guarded.

#pragma once

namespace dungeon {

// Run the stack self-test. Returns true if every phase passes.
bool runStackSelfTest();

}  // namespace dungeon
