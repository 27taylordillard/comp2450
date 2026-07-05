// COMP 2450 — Wardens of the Foundations + the Trees
// battle/Battle.h — public API for the boss-battle commands.
//
// Two encounters live behind this header now:
//   * runWardenBattle    — Midterm 1, single boss (Floors 0–3).
//   * runTreeWarden      — Midterm 3, the Masked Menagerie (Floors 8–11).
//
// Read the public midterm page on the site before you start typing.
// It has the full rubric, the AI policy, the schedule, and the four
// Floor 8–11 ties you must wire into your runTreeWarden.
//
// You may add helpers, types, or constants in Battle.cpp as you see
// fit. The only contracts main.cpp depends on are the two functions
// declared here and the BattleOutcome enum below.

#pragma once

#include <exception>
#include <string>

#include "../bestiary/Monster.h"
#include "../bestiary/Registry.h"
#include "../hero/BST.h"
#include "../hero/HashTable.h"
#include "../hero/Hero.h"
#include "../hero/Tree.h"

namespace dungeon {

// Throw BattleException for any battle error that is not genuinely a bad
// index — an unknown weakness typed at the Ward prompt, a malformed
// command. (A genuinely bad index still wants BagException, which carries
// the index and the size.) It is caught inside the battle loop so every
// error re-prompts the SAME beat without ending the turn.
class BattleException : public std::exception {
public:
    explicit BattleException(const std::string& message) : msg_(message) {}
    const char* what() const noexcept override { return msg_.c_str(); }

private:
    std::string msg_;
};

// What happened at the end of an encounter. main.cpp uses this to pick
// the correct closing banner.
enum class BattleOutcome {
    Victory,    // boss HP <= 0
    Defeat,     // player HP <= 0
    Fled        // player chose Flee
};

// Run the Warden of the Foundations encounter (Midterm 1).
//   - Reads choices from std::cin.
//   - Prints play-by-play to std::cout.
//   - Mutates hero.inventory if items are consumed.
// Returns the outcome.
BattleOutcome runWardenBattle(Hero& hero);

// Run the Warden of the Trees encounter (Midterm 3) — the Masked Menagerie.
// A single shielded boss marshals a line of the Hold's beasts in the order
// McCown's classification tree hands them out (taxonomy.levelOrder()). Each
// masked beast steps forward one at a time; you IDENTIFY it (whisper = the
// free O(1) hash lookup; recall = the slow BST descent that costs more when
// the catalogue leans), EXPOSE its weakness from your Registry arsenal, then
// STRIKE. When the menagerie is cleared, the Warden itself is exposed only by
// CLASSIFYING — tracing a beast's full lineage.
//
// All four indexes are passed by const& — the battle never mutates them.
// The one mutation (the Warden banishes a name from the Hall of Whispers) is
// done on a LOCAL COPY, so the session-wide `whispers` stays untouched.
BattleOutcome runTreeWarden(Hero& hero,
                            const Tree<std::string>& taxonomy,
                            const BST<Monster>&      catalogue,
                            const Registry&          registry,
                            const HashTable&         whispers);

}  // namespace dungeon
