// COMP 2450 — Floor 7 starter
// hero/QueueTests.h — the `selftest queue` command. Provided.
//
// Exercises Queue<int> through five phases. Each phase prints OK / FAIL
// with a one-line diagnostic so you can tell which method is still
// stubbed. Phases short-circuit: once one fails, the rest still run
// but they don't crash on undefined behavior — every check that would
// dereference an empty queue is guarded.

#pragma once

namespace dungeon {

// Run the queue self-test. Returns true if every phase passes.
bool runQueueSelfTest();

}  // namespace dungeon
