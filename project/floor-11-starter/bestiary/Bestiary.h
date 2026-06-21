// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// bestiary/Bestiary.h — post-Floor-3 reference. The keep's bestiary is
// now stored in a Bag<Monster>, the container template Floor 3
// introduced — same Bag<T> that holds the hero's inventory.

#pragma once

#include <string>

#include "../hero/Bag.h"
#include "Monster.h"

namespace dungeon {

// Read monsters from a pipe-delimited text file into a Bag.
// File format, one per line:  name|hp|attack|weakness
// Blank lines and lines starting with '#' are ignored.
// On read failure returns an empty Bag.
Bag<Monster> loadBestiary(const std::string& path);

// Sort the bestiary by name ascending. Call this BEFORE binarySearch.
void sortBestiary(Bag<Monster>& bestiary);

// Pretty-print one monster to stdout.
void printMonster(const Monster& m);

}  // namespace dungeon
