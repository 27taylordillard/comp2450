// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// hero/Hero.cpp — post-Floor-3 reference. loadInventory now returns a
// Bag<Item>, mirroring Hero::inventory's new type.

#include "Hero.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace dungeon {

// Read the starter inventory. Same shape as Bestiary's loader; skip
// blank/commented lines, abandon any malformed row rather than abort.
Bag<Item> loadInventory(const std::string& path) {
    Bag<Item> result;
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Your satchel is missing: " << path << "\n";
        return result;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        Item it;
        std::istringstream ss(line);
        std::string field;

        if (!std::getline(ss, it.name, '|')) continue;
        if (!std::getline(ss, field,   '|')) continue;
        it.weight = std::stod(field);       // stod = "string to double"
        if (!std::getline(ss, field,   '|')) continue;
        it.value = std::stoi(field);        // stoi = "string to int"

        result.push_back(it);
    }
    return result;
}

// Pretty-print the inventory. The std::setw / std::left dance aligns
// the columns so a visual scan catches ties (Iron key and Loaf of bread
// both at wt 0.1, for instance — the stability demo).
void printInventory(const Hero& hero) {
    if (hero.inventory.empty()) {
        std::cout << "Your satchel is empty.\n";
        return;
    }
    for (std::size_t i = 0; i < hero.inventory.size(); ++i) {
        const Item& it = hero.inventory[i];
        std::cout << "  " << std::setw(2) << std::right << (i + 1) << ".  "
                  << std::setw(18) << std::left << it.name
                  << "(wt " << std::fixed << std::setprecision(1) << it.weight
                  << ", val " << it.value << ")\n";
        std::cout.unsetf(std::ios::fixed);
        std::cout.unsetf(std::ios::left);
    }
}

}  // namespace dungeon
