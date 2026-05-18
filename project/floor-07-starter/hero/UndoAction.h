// COMP 2450 — Floor 6 starter
// hero/UndoAction.h — what gets pushed onto Hero::undoStack.
//
// A single struct that bundles together everything `undo` needs to
// reverse one command:
//   * a human description (so undo can SAY what it undid)
//   * a snapshot of the hero's inventory taken BEFORE the command ran
//
// On Friday you write the `undo` dispatcher in main.cpp. Its body is
// dead simple — pop the top action, copy the snapshot back over
// hero.inventory, print the description.
//
// Why a full inventory snapshot? Because for most mutating commands
// (sort, take, eventually drop/give/find/loot) the cheapest correct
// undo is "remember what the bag looked like, put it back." A stack
// of snapshots makes EVERY mutating command undoable through the same
// dispatcher. The cost — a Bag<Item> copy per push — is fine at human
// scale; we are not commiting to undo a million operations per second.

#pragma once

#include <string>

#include "Bag.h"
#include "Item.h"

namespace dungeon {

struct UndoAction {
    std::string description;        // shown to the player on `undo`
    Bag<Item>   inventorySnapshot;  // restored over hero.inventory on undo
};

}  // namespace dungeon
