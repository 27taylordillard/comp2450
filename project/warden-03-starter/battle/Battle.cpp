// COMP 2450 — Wardens of the Foundations + the Trees (Midterm 3 starter)
// battle/Battle.cpp — TWO encounters live here now.
//
//   * runWardenBattle  — Midterm 1, single boss (Floors 0–3). FILLED below,
//                        the shared reference. Do not touch it; it is the
//                        Warden-1 encounter both later wardens sit beside.
//   * runTreeWarden    — Midterm 3, the Masked Menagerie (Floors 8–11).
//                        THIS IS THE STUB YOU WRITE. The requirements, the
//                        grader's script, the four required ties, and a
//                        suggested outline are in the big comment block above
//                        the stub. It ships as a placeholder so the project
//                        builds and runs even for an incomplete submission.
//
// Warden 1 (runWardenBattle) — Floor 0–3 ties, unchanged:
//   F0 — actions live in a Bag<MenuAction>.
//   F1 — Use-item branch calls findByName<Item> on hero.inventory.
//   F2 — items menu std::sort's the inventory by value-descending first.
//   F3 — bad input throws BagException, caught inside the loop.

#include "Battle.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../bestiary/Search.h"
#include "../hero/Bag.h"
#include "../hero/BagException.h"
#include "../hero/Item.h"
#include "../hero/Queue.h"
#include "../hero/Sort.h"

namespace dungeon {

namespace {

constexpr int kPlayerStartHP   = 30;
constexpr int kWardenStartHP   = 50;
constexpr int kPlayerAttackDmg = 6;
constexpr int kWardenAttackDmg = 4;

// F0 — abstract data type for the available turn actions. Bag<MenuAction>
// is the Floor-3 container template; one stamping for an enum-payload
// list is all the action menu needs. Defended in one sentence: the menu
// is a small, ordered, indexed list with no insertions mid-list, which
// is exactly what Bag<T> (vector-backed) is good for.
enum class MenuAction { Attack, UseItem, Inspect, Flee };

struct MenuOption {
    int         number;
    std::string label;
    MenuAction  action;
};

void printMenu(const Bag<MenuOption>& menu, int playerHP, int wardenHP) {
    std::cout << "\n  -- Your turn --   you HP " << playerHP
              << "    Warden HP " << wardenHP << "\n";
    for (std::size_t i = 0; i < menu.size(); ++i) {
        std::cout << "    " << menu[i].number << ". " << menu[i].label << "\n";
    }
    std::cout << "  > ";
}

MenuAction readMenuChoice(const Bag<MenuOption>& menu) {
    std::string line;
    if (!std::getline(std::cin, line)) {
        // EOF: treat as flee so we don't loop on closed stdin.
        return MenuAction::Flee;
    }
    int n = -1;
    try { n = std::stoi(line); }
    catch (...) {
        // F3 — non-numeric input throws into the battle loop's catch.
        throw BagException(
            static_cast<std::size_t>(menu.size() + 1),
            menu.size());
    }
    for (std::size_t i = 0; i < menu.size(); ++i) {
        if (menu[i].number == n) return menu[i].action;
    }
    // F3 — out-of-range throws too.
    throw BagException(static_cast<std::size_t>(n), menu.size());
}

// Use-item handler. Sorts the inventory by value descending (F2),
// shows it, asks for a name, looks it up via findByName<Item> (F1),
// applies the effect. A "Healing potion" heals 12 HP and is consumed
// in spirit (we don't actually remove the item this floor — that is
// Floor 4½'s remove-mid-list lesson).
void useItem(Hero& hero, int& playerHP) {
    if (hero.inventory.empty()) {
        std::cout << "  Your satchel is empty.\n";
        return;
    }

    // F2 — sort the menu by value descending so the most valuable item
    // shows first. Sort routine reused from Floor 2; comparator is a
    // lambda.
    sortInventory(hero, "value desc");
    std::cout << "  Choose an item by name:\n";
    printInventory(hero);
    std::cout << "  > ";

    std::string name;
    if (!std::getline(std::cin, name) || name.empty()) {
        std::cout << "  You hesitate.\n";
        return;
    }

    // F1 — findByName<Item> lookup.
    const Item* it = findByName<Item>(hero.inventory, name);
    if (!it) {
        // F3 — no such item: throw, caught by the battle-loop catch.
        throw BagException(0, hero.inventory.size());
    }

    if (it->name.find("otion") != std::string::npos) {
        playerHP = std::min(playerHP + 12, kPlayerStartHP);
        std::cout << "  You drink " << it->name << ". HP -> " << playerHP << ".\n";
    } else {
        std::cout << "  You ready " << it->name << " — but it is not a consumable.\n";
    }
}

}  // anonymous namespace

BattleOutcome runWardenBattle(Hero& hero) {
    int playerHP = kPlayerStartHP;
    int wardenHP = kWardenStartHP;

    // F0 — turn actions in a Bag<MenuOption>.
    Bag<MenuOption> menu;
    menu.push_back({1, "Attack",         MenuAction::Attack});
    menu.push_back({2, "Use item",       MenuAction::UseItem});
    menu.push_back({3, "Inspect Warden", MenuAction::Inspect});
    menu.push_back({4, "Flee",           MenuAction::Flee});

    hero.eventLog.push_front("battle warden — engaged");

    while (playerHP > 0 && wardenHP > 0) {
        // F3 — anything thrown by readMenuChoice or useItem lands here,
        // we re-prompt without ending the turn.
        try {
            printMenu(menu, playerHP, wardenHP);
            switch (readMenuChoice(menu)) {
                case MenuAction::Attack: {
                    wardenHP -= kPlayerAttackDmg;
                    std::cout << "  You strike for " << kPlayerAttackDmg
                              << ".  Warden HP -> " << std::max(wardenHP, 0) << ".\n";
                    hero.eventLog.push_front(
                        "you struck for " + std::to_string(kPlayerAttackDmg));
                    if (wardenHP > 0) {
                        playerHP -= kWardenAttackDmg;
                        std::cout << "  The Warden retaliates for "
                                  << kWardenAttackDmg
                                  << ".  Your HP -> " << std::max(playerHP, 0) << ".\n";
                        hero.eventLog.push_front(
                            "the Warden struck for " + std::to_string(kWardenAttackDmg));
                    }
                    break;
                }
                case MenuAction::UseItem: {
                    useItem(hero, playerHP);
                    // Using an item ends the turn — the warden retaliates.
                    if (wardenHP > 0 && playerHP > 0) {
                        playerHP -= kWardenAttackDmg;
                        std::cout << "  The Warden strikes while you fumble.  Your HP -> "
                                  << std::max(playerHP, 0) << ".\n";
                        hero.eventLog.push_front(
                            "the Warden struck for " + std::to_string(kWardenAttackDmg));
                    }
                    break;
                }
                case MenuAction::Inspect: {
                    std::cout << "  Warden of the Foundations.  HP " << wardenHP
                              << " / " << kWardenStartHP
                              << ".  No visible weakness.  (free action)\n";
                    // FREE action — turn does not end.
                    break;
                }
                case MenuAction::Flee: {
                    hero.eventLog.push_front("battle warden — fled");
                    return BattleOutcome::Fled;
                }
            }
        }
        catch (const BagException& e) {
            std::cout << "  " << e.what() << " — try again.\n";
            // Turn does NOT advance.
        }
    }

    if (wardenHP <= 0) {
        hero.eventLog.push_front("battle warden — outcome: Victory");
        return BattleOutcome::Victory;
    }
    hero.eventLog.push_front("battle warden — outcome: Defeat");
    return BattleOutcome::Defeat;
}

// =====================================================================
// WARDEN 3 — the Masked Menagerie (Floors 8–11).  YOU BUILD THE BODY.
// =====================================================================
//
// Re-read the public midterm page (the Warden of the Trees) if anything
// below is unclear.  The Warden is the shadow of ordering: it wears the
// Hold's beasts as stolen faces and CANNOT be struck while its menagerie
// stands.  You break each mask by IDENTIFYING it, EXPOSING its weakness,
// and STRIKING; then you unmask the Warden itself by CLASSIFYING a lineage.
//
// REQUIRED MECHANICS
// =====================================================================
// Player starts at 45 HP.  Numbers are yours to tune; document tunings in
// encounter-notes.md.  The menagerie length is a fixed constant (5) — the
// `battle trees` command takes NO argument.
//
// Build a Queue<Enemy> of the first 5 LEAF beasts in taxonomy.levelOrder()
// order (see [F8] below).  Each Enemy starts as just a NAME — its hp / atk /
// weakness are filled in when you identify it.  The front of the line is the
// current mask.  When the queue empties, the Warden itself (HP 24, ATK 5) is
// the target.
//
// MENU while the menagerie stands (six options):
//   1. Whisper the front mask  — whispers.find(name); FREE, no retaliation.
//                                If the name was banished (see [F11]), the
//                                niche is silent → "recall it instead."
//   2. Recall the front mask   — catalogue.find(probe); fills the same fields
//                                the SLOW way, then prints the balance readout.
//                                Costs 1 retaliation; a SECOND when the
//                                catalogue is not balanced (the leaning shelf).
//   3. Ward its weakness       — registry.hasWeakness(w) validates; a match on
//                                the identified beast's weakness EXPOSES it.
//                                Prints registry.countWeakness(w).  Costs 1
//                                retaliation.  Unknown weakness → clean
//                                re-prompt, no beat lost.
//   4. Strike                  — exposed: 14, NO retaliation; else 4 chip and
//                                the beast retaliates its ATK.  HP <= 0 →
//                                line.dequeue() and the next mask steps up.
//   5. Study the menagerie     — print the remaining order + replay eventLog
//                                via printLog.  FREE — turn does NOT end.
//   6. Flee                    — return BattleOutcome::Fled.
//
// MENU once the menagerie is spent (the Warden phase):
//   1. Classify a lineage      — taxonomy.lineage(name); a complete, correct
//                                root-to-leaf lineage EXPOSES the Warden.
//                                Costs 1 retaliation.
//   2. Strike / 3. Study / 4. Flee — as above (chip 4 is the slow fallback;
//                                exposed 14 fells the Warden).
//
// END:
//   Victory  when the menagerie is cleared AND the Warden's HP <= 0
//            (report surviving HP in the closing line — page convention).
//   Defeat   when playerHP <= 0.
//   Fled     when the player chose Flee.
//
// FLOOR-TIED REQUIREMENTS — ALL FOUR MUST APPEAR IN YOUR CODE
// =====================================================================
//
//   [F8]  Tree / taxonomy / lineage
//         * taxonomy.levelOrder()  builds the menagerie's arrival order (BFS).
//           Keep only the LEAF names the catalogue knows — levelOrder()
//           returns CATEGORIES too ("Undead", "Beasts") and the root; filter
//           them out or you will try to fight "Undead".  Note in a comment
//           that preOrder() (DFS) would send a DIFFERENT mask forward first
//           (Skeleton, not Bone Spider).
//         * taxonomy.lineage(name)  exposes the Warden in the boss phase.
//
//   [F9 + F9½]  BST + balance
//         * catalogue.find(probe)  is the slow Recall identify.  Build the
//           probe with ONLY the name set — Monster probe; probe.name = name;
//           (exactly as main.cpp's `recall` command does).  find returns a
//           const Node*; read node->data.
//         * catalogue.isBalanced() / catalogue.height() / catalogue.balanceFactor()
//           gate the lean penalty and print the balance readout.  The real
//           catalogue is NOT balanced — Recall costs the extra hit.
//
//   [F10] sets & maps
//         * registry.hasWeakness(w)   — std::set membership; validates a ward.
//         * registry.countWeakness(w) — std::map find (const → Phantom-Key-proof);
//           "how many kin fear this."
//         * registry.weaknesses()     — print the arsenal.
//
//   [F11] hash table (the whisper)
//         * whispers.find(name)  — the FREE instant identify.
//         * The BANISH: copy the table locally FIRST, then erase one name, so
//           the shared `whispers` is never mutated and one beast is forced
//           onto the slow Recall path:
//               HashTable localWhispers = whispers;   // copy BEFORE erase
//               localWhispers.erase("Bone Spider");   // the banished mask
//           Bone Spider is the first level-order leaf, so it is in every
//           menagerie — whispering it returns nullptr → you MUST Recall it.
//
// WHAT THE GRADER WILL DO  (the 3-phase self-check — run it before you push)
// =====================================================================
//   Phase 1 — the skeleton holds.  `battle trees` starts; the menagerie steps
//     forward in level-order (Bone Spider first); the state line prints every
//     beat; win one run on purpose and lose one on purpose.
//   Phase 2 — the gate holds.  Whisper and Study are FREE (no retaliation,
//     same beat); typing `9` and `abc` at the menu re-prompts the SAME beat
//     without crashing; a mistyped weakness in Ward errors cleanly.
//   Phase 3 — the four indexes hold.  Put a finger on each tie in your source:
//     [F8] levelOrder / lineage;  [F9] catalogue.find + [F9½] isBalanced /
//     height;  [F10] hasWeakness / countWeakness;  [F11] whispers.find;  and
//     confirm the banished beast forces a Recall.
//
// SUGGESTED OUTLINE  (decompose however you like — these are the natural cuts)
// =====================================================================
//   // A local value type, in this anonymous namespace, exactly like W2's Enemy:
//   struct Enemy {
//       std::string name;
//       int         hp = 0, atk = 0;
//       std::string weakness;
//       bool        identified = false, exposed = false;
//   };
//
//   Queue<Enemy> buildMenagerie(const Tree<std::string>& taxonomy,
//                               const BST<Monster>& catalogue,
//                               std::size_t count) {
//       // [F8] walk taxonomy.levelOrder(); keep leaves catalogue.find() knows;
//       //      enqueue the first `count` as name-only Enemies.
//   }
//   void identifyFront(Enemy& e, const Monster& m);          // fill hp/atk/weakness
//   void printTreeMenu(const Bag<TreeOption>& menu, ...);    // state line + options
//   TreeAction readTreeChoice(const Bag<TreeOption>& menu);  // stoi → BagException
//
//   BattleOutcome runTreeWarden(...) {
//       int playerHP = 45, wardenHP = 24;  bool wardenExposed = false;
//       Queue<Enemy> line = buildMenagerie(taxonomy, catalogue, 5);
//       HashTable localWhispers = whispers;   // [F11] copy BEFORE erase
//       localWhispers.erase("Bone Spider");   // [F11] the banish
//       ... build the two menus ...
//       while (playerHP > 0 && wardenHP > 0) {
//           bool wardenPhase = line.empty();
//           try {
//               print state + the phase's menu;  switch (readTreeChoice(...)) {
//                   case Whisper: localWhispers.find(front.name); FREE.
//                   case Recall:  catalogue.find(probe); balance readout;
//                                 retaliation (+1 when !isBalanced()).
//                   case Ward:    registry.hasWeakness / countWeakness; expose
//                                 on a match; 1 retaliation; bad weakness →
//                                 throw BattleException (clean re-prompt).
//                   case Strike:  exposed 14 no-retaliate / chip 4 + retaliate;
//                                 dequeue on death.  (Warden phase: same, vs boss.)
//                   case Classify:taxonomy.lineage(name); expose Warden; 1 retaliation.
//                   case Study:   remaining order + printLog(eventLog...). FREE.
//                   case Flee:    return BattleOutcome::Fled;
//               }
//           }
//           catch (const BagException&   e) { /* bad menu index → re-prompt */ }
//           catch (const BattleException& e) { /* bad weakness etc. → re-prompt */ }
//       }
//       return (wardenHP <= 0) ? Victory : Defeat;   // report surviving HP on Victory
//   }
//
// Read line.front()'s fields BEFORE line.dequeue() — dequeue destroys the
// front node (the W2 dangling-front hazard).  Make the localWhispers COPY
// before any erase, and never mutate the shared `whispers`.
//
// The only contract main.cpp depends on is the public API in Battle.h:
//   BattleOutcome runTreeWarden(Hero&, const Tree<std::string>&,
//                               const BST<Monster>&, const Registry&,
//                               const HashTable&);
// Internal organisation is yours.  Replace the placeholder body below.

BattleOutcome runTreeWarden(Hero& hero,
                            const Tree<std::string>& taxonomy,
                            const BST<Monster>&      catalogue,
                            const Registry&          registry,
                            const HashTable&         whispers) {
    (void)hero; (void)taxonomy; (void)catalogue; (void)registry; (void)whispers;
    std::cout << "  (Tree-warden scaffold — runTreeWarden is not yet written.)\n"
              << "  (Open battle/Battle.cpp and follow the TODOs.)\n";
    return BattleOutcome::Fled;
}

}  // namespace dungeon
