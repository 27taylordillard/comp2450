// COMP 2450 — Warden of the Foundations (Midterm 1)
// battle/Battle.h — public API for the boss-battle command.
//
// Read the public midterm page on the site before you start typing.
// It has the full rubric and the four Floor 0–3 ties you must wire
// into your implementation in Battle.cpp.
//
// You may add helpers, types, or constants here as you see fit. The
// only contract main.cpp depends on is `runWardenBattle(Hero&)` and
// the BattleOutcome enum below.

#pragma once

#include "../hero/Hero.h"

namespace dungeon {

// What happened at the end of the encounter. main.cpp uses this to pick
// the correct closing banner.
enum class BattleOutcome {
    Victory,    // warden HP <= 0
    Defeat,     // player HP <= 0
    Fled        // player chose Flee
};

// Run the Warden of the Foundations encounter.
//   - Reads choices from std::cin.
//   - Prints play-by-play to std::cout.
//   - Mutates hero.inventory if items are consumed.
// Returns the outcome.
BattleOutcome runWardenBattle(Hero& hero);

}  // namespace dungeon
