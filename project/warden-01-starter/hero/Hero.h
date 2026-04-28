// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// hero/Hero.h — post-Floor-3 reference. Inventory is now Bag<Item>, the
// container template Floor 3 introduced.
//
// This is the hero the rest of the game grows around. Right now it is
// a name and an inventory. Later floors will bolt on more: spell book
// (Floor 6, stack), action queue (Floor 7, queue), skill tree (Floor 8).

#pragma once

#include <string>

#include "Bag.h"
#include "Item.h"

namespace dungeon {

struct Hero {
    std::string heroName;
    Bag<Item>   inventory;     // post-Floor-3: Bag<Item>, not std::vector<Item>
};

// Load a starting inventory from a pipe-delimited text file.
// File format, one per line:  name|weight|value
// Blank lines and lines starting with '#' are ignored.
// On read failure returns an empty Bag.
Bag<Item> loadInventory(const std::string& path);

// Pretty-print the hero's inventory, numbered, to stdout.
void printInventory(const Hero& hero);

}  // namespace dungeon
