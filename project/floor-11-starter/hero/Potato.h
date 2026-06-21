// COMP 2450 — Floor 7 starter
// hero/Potato.h — Hot Potato (Wednesday's exercise).
//
// One free function: hotPotato(names, k) returns the name of the single
// survivor in the classical "pass the potato around the circle and
// every k-th person is eliminated" game. The algorithm is FIFO with a
// rotation step:
//
//   1. Put every name into a Queue<std::string>, in the order given.
//   2. Until only one name remains:
//        a. Rotate k-1 times: front() -> enqueue at back, then dequeue.
//           (This moves the next k-1 people back to the end of the line.)
//        b. dequeue() the front person — they are eliminated.
//   3. Return the one name left.
//
// Examples:
//
//     hotPotato({"A","B","C","D","E"}, 3) == "D"
//     hotPotato({"A"},                 7) == "A"        // one in, one out
//     hotPotato({"A","B"},             1) == "B"        // k=1 drops the front each round
//     hotPotato({},                    3) == ""         // empty input — empty output
//
// You write the body in hero/Potato.cpp using your Queue<std::string>.
// This is the canonical queue-use exercise; reading it should feel like
// the definition of "what is a queue for, actually."

#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace dungeon {

std::string hotPotato(const std::vector<std::string>& names, std::size_t k);

}  // namespace dungeon
