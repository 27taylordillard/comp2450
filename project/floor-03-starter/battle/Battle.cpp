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
//                      sentence (the same kind of defence Trial I Q1
//                      practiced — but this is a different collection
//                      than Q1's items menu; the right ADT may differ).
//
//   F1 (search)      — the Use-item branch MUST call findByName<Item>
//                      against hero.inventory to look up the item the
//                      player typed.
//
//   F2 (sort)        — when the items menu is displayed, sort the
//                      inventory at display time with a comparator
//                      (std::sort or your Floor 2 sortInventory). Pick
//                      a criterion (value — the healing-power stand-in
//                      — weight, or name) and document it in a comment.
//
//   F3 (templates +  — invalid menu input must `throw BattleException`
//      exceptions)     (ready skeleton in Battle.h) — or BagException
//                      where a genuinely bad index is the fault —
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
#include <vector>
#include <algorithm>

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
//constexpr means these values are compile-time constants
//k prefexi --> means const
constexpr int kPlayerStartHP   = 30;
constexpr int kWardenStartHP   = 50;
constexpr int kPlayerAttackDmg = 6;   // damage per Attack action
constexpr int kWardenAttackDmg = 4;   // warden's retaliation damage

//enum create a set of named choices
//enum class, keeps our names scoped
enum class MenuAction {
    Attack,
    UseItem,
    InspectWarden,
    Flee
};
struct MenuOption {
    int number; //number typed by the player
    std::string label; // text displayed by the menu
    MenuAction action; // action performed by our program
};
void printMenu(
    Bag<MenuOption>& menu,
    int playerHP,
    int wardenHP){
        std::cout << "\n -- Your turn -- your hp" << playerHP 
            << "    Warden hp " << wardenHP << "\n";

        for(std::size_t i = 0; i < menu.size(); ++i) {
            std::cout << "    "
                << menu[i].number
                << ". "
                << menu[i].label
                << "\n";
        }
        std::cout << " > ";
    }
    MenuAction readMenuChoice(const Bag<MenuOption>& menu) {
        std::string line;
        if(!std::getline(std::cin, line)) {
            return MenuAction::Flee; 
        }
        int n = -1;

        try {
            n = std::stoi(line);
        }
        catch(...) {
            throw BattleException( "'" + line + "' is not a menu number(enter 1 to " +
                std::to_string(menu.size()) + ")"
            );
        }
        for(std::size_t i = 0; i < menu.size(); ++i) {
            if(menu[i].number == n) {
                return menu[i].action;
            }
        }
        throw BagException(
            static_cast<std::size_t>(n),
            menu.size()
        );
    }
    //handle the player's "use item" action
    //Hero& will give the function access to the 
    //original hero object
    void useItem(Hero& hero, int& playerHP) {
        if(hero.inventory.empty()){
            std::cout << "Your satchel is empty. \n";
            return;
        }
        sortInventory(hero, "value desc");
        std::cout << "Choose an item by name: \n";
        printInventory(hero);
        std::cout << " > ";

        std::string name;

        if(!std::getline(std::cin, name) || name.empty()) {
            std::cout << "you hesitated. \n";
            return;
        }

        //findByName<Item> <-- function-template specilization
        const Item* it = findByName<Item>(hero.inventory, name);

        if(!it) {
            throw BattleException(
                "no item found '" + name + "' in your satchel"
            );
        }
        if(it->name.find("otion") != std::string::npos) {
            playerHP =std::min(
                playerHP + 12,
                kPlayerStartHP
            );
            std::cout << " You drink "
                << it->name
                << ". HP -> "
                << playerHP
                << ".\n";
        }
        else {
            std::cout << " You ready "
                << it ->name
                << " - but it is not a consumable. \n";
        }
    }
}


BattleOutcome runWardenBattle(Hero& hero) {
    int playerHP = kPlayerStartHP;
    int wardenHP = kWardenStartHP;

    Bag<MenuOption > menu;
    menu.push_back({1, "Attack", MenuAction::Attack });
    menu.push_back({2, "Use item", MenuAction::UseItem});
    menu.push_back({3, "Inspect warden", MenuAction::InspectWarden});
    menu.push_back({4, "Flee", MenuAction::Flee});

    while(playerHP > 0 && wardenHP > 0) {
        try {
            printMenu(menu, playerHP, wardenHP);

            switch(readMenuChoice(menu)) {
            case MenuAction::Attack: {
                wardenHP -= kPlayerAttackDmg;
                std::cout << "you strike for "
                    << kPlayerAttackDmg
                    << ".Warden HP -> "
                    << std::max(wardenHP, 0)
                    << ".\n";
                
                //is the warden dead?
                if(wardenHP > 0) {
                    playerHP -= kWardenAttackDmg;

                    std::cout << "The warden retaliates for "
                        << kWardenAttackDmg
                        << ". Your HP ->"
                        << std::max(playerHP, 0)
                        << ".\n";
                }
                //break to exit the switch case, not the while loop
                break;
            }
            case MenuAction::UseItem: {
                useItem(hero, playerHP);
                //using an item does consume our term
                if(wardenHP > 0 && playerHP > 0) {
                    playerHP -= kWardenAttackDmg;

                    std::cout << "The warden strikes while you fumble. Your HP ->"
                        << std::max(playerHP, 0)
                        << ".\n";
                }
                break;
            }
            case MenuAction::InspectWarden : {
                std::cout << "Warden of the Foundations. HP ->"
                    << wardenHP
                    << " / "
                    <<kWardenStartHP
                    << ". No visible weakness (free action). \n";

                break;
            }
            case MenuAction::Flee: {
                //return to immediately exit function
                return BattleOutcome::Fled;
            }
            }
        }
        catch(const std::exception& e) {
            //battle exception and bag exception will inherut
            std::cout << e.what()
                << " - try again.\n";
        }
    }
    return wardenHP <= 0
        ? BattleOutcome::Victory
        : BattleOutcome::Defeat;
}
}
//}  // anonymous namespace 
/* The code that I wrote before Friday's class:
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
    //           if invalid → throw BattleException(...) [F3 — throw].
    //           dispatch on the action:
    //               Attack:    wardenHP -= kPlayerAttackDmg;
    //                          if wardenHP > 0, playerHP -= kWardenAttackDmg.
    //               Use item:  std::sort(hero.inventory.begin(),
    //                                    hero.inventory.end(),
    //                                    yourComparator)             [F2].
    //                          show sorted menu, read item name.
    //                          const Item* it = findByName<Item>(
    //                              hero.inventory, name);             [F1]
    //                          if (!it) throw BattleException(...);   [F3]
    //                          apply effect (heal? buff next attack? …).
    //                          end turn.
    //               Inspect:   print warden state. FREE — do NOT end turn.
    //               Flee:      return BattleOutcome::Fled.
    //       }
    //       catch (const std::exception& e) {                        [F3 — catch]
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
    int playerHP = kPlayerStartHP;
    int wardenHP = kWardenStartHP;

    while (playerHP > 0 && wardenHP > 0) {
        std::cout << "\nPlayer HP: " << playerHP
        << " | Warden HP: " << wardenHP << "\n";

        std::vector<std::string> actions = {"Attack", "Use item", "Inspect Warden", "Flee"};
        try {
            // Display the menu
            for(int i = 0; i < actions.size(); ++i) {
                std::cout << i + 1 << ". " << actions[i] << "\n";
            }
            //get player's choice
            int choice;
            std:: cin >> choice;

            if(std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                throw BattleException("Invalid menu choice.");
            }

            //check menu choice
            if(choice < 1 || choice > actions.size()) {
                throw BattleException("Invalid menu choice. Please select a valid option.");
            }
            //handle attack
            if(choice == 1) {
                wardenHP -= kPlayerAttackDmg;
                std::cout << "You attack the Warden!\n";
                //warden retaliates if still alive
                if(wardenHP > 0) {
                    playerHP -= kWardenAttackDmg;
                    std::cout << "The Warden attacks back!\n";
                }
            }
            //handle use item
            else if (choice == 2) {
                //sort items by name when the menu is displayed
                std::sort(hero.inventory.begin(), hero.inventory.end(),
                        [](const Item& a, const Item& b) {
                              return a.name < b.name; // sorting by name
                        });
                std::cout << "Choose an item:\n";
                for (std::size_t i = 0; i < hero.inventory.size(); ++i) {
                    std:: cout << hero.inventory[i].name << "\n";
                }
                //get item name from player
                std::string itemName;
                std::cin.ignore(1000, '\n');
                std::getline(std::cin, itemName);

                const Item* item = findByName<Item>(hero.inventory, itemName);

                //checks that item was found
                if(item == nullptr) {
                    throw BattleException("Unknown Item.");
                }
                if(item->name == "Healing potion") {
                    playerHP += 10; //heal 10 HP
                    std::cout << "You used a Healing Potion!\n";
                }
                if(item->name == "Loaf of bread") {
                    playerHP += 5;
                    std::cout << "You ate the loaf of bread! You feel a bit better.\n";
                }
                if(item->name == "Rusty sword") {
                    wardenHP -=3;
                    std::cout << "You used the Rusty Sword! The Warden takes some damage.\n";
                }
                if(item-> name == "Iron key") {
                    std::cout << "the Iron key cannot help during this battle.\n";
                }
                if(item->name == "Cloak of shadows") {
                    std::cout << "You used the Cloak of shadows to hide from the Warden!\n";
                }
            }
            else if(choice == 3) {
                std::cout << "The Warden has " << wardenHP << " HP remaining.\n";
                std::cout << "The Warden looks strong and ready to fight!\n";
            }
            else if(choice == 4) {
                std::cout << "You flee from the Warden.\n";
                std::cout << "The gate stays closed.\n";
                return BattleOutcome::Fled;
            }
        }
        catch (const std::exception& e) {
            std::cout << "  " << e.what() << "  Try again.\n";
            continue;   // re-prompt; turn does NOT advance
        }
        //show the battle result
        if(wardenHP <= 0) {
            std::cout << "You defeated the Warden!\n";
            return BattleOutcome::Victory;
        }
        if(playerHP <= 0) {
            std::cout << "You have been defeated by the Warden.\n";
            return BattleOutcome::Defeat;
        }
    }

        
    (void)hero;
    std::cout << "  (Battle scaffold — runWardenBattle is not yet written.)\n"
              << "  (Open battle/Battle.cpp and follow the TODOs.)\n";
    return BattleOutcome::Fled;
}*/

//}  // namespace dungeon
