// COMP 2450 — Floor 5 starter
// hero/IteratorTests.h — the `selftest iterator` command. Provided.
//
// Exercises Chain<int>::iterator and const_iterator with three standard
// library tools — range-based for, std::find, and std::distance — plus
// std::reverse (Friday). Each phase prints PASS / FAIL with a one-line
// diagnostic so you can tell which operator is still stubbed.

#pragma once

namespace dungeon {

// Run the iterator self-test. Returns true if every phase passes.
bool runIteratorSelfTest();

}  // namespace dungeon
