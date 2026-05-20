// COMP 2450 — Wardens of the Foundations + Middle Gates
// battle/Battle.h — public API for the boss-battle commands.
//
// Two encounters live behind this header now:
//   * runWardenBattle    — Midterm 1, single boss (Floors 0–3).
//   * runWaveSurvival    — Midterm 2, N waves of enemies     (Floors 4–7).
//
// Read the public midterm pages on the site before you start typing.
// They have the full rubrics and the required floor ties for each.
//
// You may add helpers, types, or constants in Battle.cpp as you see
// fit. The only contracts main.cpp depends on are the two functions
// declared here and the BattleOutcome enum below.

#pragma once

#include "../hero/Hero.h"

namespace dungeon {

// What happened at the end of an encounter. main.cpp uses this to pick
// the correct closing banner.
enum class BattleOutcome {
    Victory,    // boss HP <= 0  (warden 1)  /  every wave cleared (warden 2)
    Defeat,     // player HP <= 0
    Fled        // player chose Flee
};

// Run the Warden of the Foundations encounter (Midterm 1).
//   - Reads choices from std::cin.
//   - Prints play-by-play to std::cout.
//   - Mutates hero.inventory if items are consumed.
// Returns the outcome.
BattleOutcome runWardenBattle(Hero& hero);

// Run the Warden of the Middle Gates encounter (Midterm 2) — N waves of
// enemies, played out one wave at a time, with FIFO targeting of the
// current wave's front-most enemy and a stack-backed `undo` that rewinds
// the most recent turn's HP + inventory (but NOT the wave queue).
//
// N must be >= 1; values <= 0 are treated as 1.
BattleOutcome runWaveSurvival(Hero& hero, int waves);

}  // namespace dungeon
