// COMP 2450 — Warden of the Foundations (Midterm 1)
// battle/Battle.cpp — YOU build the body.
//
// =====================================================================
// REQUIRED MECHANICS
// =====================================================================
// Re-read the public midterm page if anything below is unclear.
//
// MENU (at minimum, four options):
//   1. Attack          — damage the warden; warden retaliates that turn
//   2. Use item        — pick from inventory; effect; turn ends
//   3. Inspect Warden  — print warden's state; FREE action — turn does
//                        NOT end
//   4. Flee            — leave; gate stays closed
//
// END:
//   Victory  when wardenHP <= 0
//   Defeat   when playerHP <= 0
//   Fled     when the player chose Flee
//
// =====================================================================
// FLOOR-TIED REQUIREMENTS — ALL FOUR MUST APPEAR IN YOUR CODE
// =====================================================================
//
//   F0 (ADT)         — the available actions on a turn must live in a
//                      container of your choice. ABOVE the declaration,
//                      in a comment, name the ADT and defend it in one
//                      sentence (this is your Trial I Q1 answer, in
//                      code form).
//
//   F1 (search)      — the Use-item branch MUST call findByName<Item>
//                      against hero.inventory to look up the item the
//                      player typed.
//
//   F2 (sort)        — when the items menu is displayed, std::sort the
//                      inventory FIRST with a comparator. Pick a
//                      criterion (healing power, weight, value, …) and
//                      document it in a comment.
//
//   F3 (templates +  — invalid menu input must `throw BagException`
//      exceptions)     (or your own type derived from std::exception),
//                      caught INSIDE the battle loop so the player gets
//                      another prompt — not a crash, not an exit.
//
// =====================================================================
// WHAT THE GRADER WILL DO
// =====================================================================
//   1. cmake --build the project. If it does not compile, the Warden
//      has won by default.
//   2. Type `battle warden`, play through to BOTH a victory and a
//      defeat (or attempt to — items + RNG permitting).
//   3. Type a deliberately invalid menu choice (e.g., "9" for a
//      4-option menu). The game must NOT crash; it must re-prompt.
//   4. Open this file and find each of the four Floor ties. They must
//      be REAL — i.e., the menu actually runs through your container,
//      Use-item actually goes through findByName, the items menu is
//      actually sorted, the throw actually fires on bad input.

#include "Battle.h"

#include <iostream>
#include <string>

#include "../hero/Bag.h"
#include "../hero/BagException.h"
#include "../hero/Item.h"
#include "../bestiary/Search.h"
#include "../hero/Sort.h"

namespace dungeon {

namespace {

// =====================================================================
// Tunable battle parameters. Edit to taste; document any tuning in
// encounter-notes.md so the grader knows what to expect.
// =====================================================================
constexpr int kPlayerStartHP   = 30;
constexpr int kWardenStartHP   = 50;
constexpr int kPlayerAttackDmg = 6;   // damage per Attack action
constexpr int kWardenAttackDmg = 4;   // warden's retaliation damage

}  // anonymous namespace

BattleOutcome runWardenBattle(Hero& hero) {
    // TODO — write the boss battle. Suggested outline (yours to refactor):
    //
    //   int playerHP = kPlayerStartHP;
    //   int wardenHP = kWardenStartHP;
    //
    //   while (playerHP > 0 && wardenHP > 0) {
    //       print state (HPs, last action — your choice).
    //
    //       try {
    //           show menu (using your F0 container of actions).
    //           read input.
    //           if invalid → throw BagException(...) [F3 — throw].
    //           dispatch on the action:
    //               Attack:    wardenHP -= kPlayerAttackDmg;
    //                          if wardenHP > 0, playerHP -= kWardenAttackDmg.
    //               Use item:  std::sort(hero.inventory.begin(),
    //                                    hero.inventory.end(),
    //                                    yourComparator)             [F2].
    //                          show sorted menu, read item name.
    //                          const Item* it = findByName<Item>(
    //                              hero.inventory, name);             [F1]
    //                          if (!it) throw BagException(...);      [F3]
    //                          apply effect (heal? buff next attack? …).
    //                          end turn.
    //               Inspect:   print warden state. FREE — do NOT end turn.
    //               Flee:      return BattleOutcome::Fled.
    //       }
    //       catch (const BagException& e) {                           [F3 — catch]
    //           std::cout << "  " << e.what() << "  Try again.\n";
    //           continue;   // re-prompt; turn does NOT advance
    //       }
    //   }
    //
    //   return wardenHP <= 0 ? BattleOutcome::Victory
    //                        : BattleOutcome::Defeat;
    //
    // Decompose into helpers however you want. The contract main.cpp
    // depends on is just runWardenBattle(Hero&).
    //
    // Replace the placeholder body below.

    (void)hero;
    std::cout << "  (Battle scaffold — runWardenBattle is not yet written.)\n"
              << "  (Open battle/Battle.cpp and follow the TODOs.)\n";
    return BattleOutcome::Fled;
}

}  // namespace dungeon
