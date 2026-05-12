// COMP 2450 — Floor 4½ starter
// hero/Hero.h — Floor 4 added a Chain<std::string> eventLog. Floor 4½
// adds the ability to walk that log BACKWARD (oldest first), via the
// new `prev` pointer on each node, and exposes Hero to copying — which
// only works once you've implemented the deep copy ctor on Chain<T>.
//
// `printLog` (newest first) is provided in working form below; you wrote
// it last week. `printLogOldest` (oldest first) is the new one and is
// your Monday job.

#pragma once

#include <string>

#include "Bag.h"
#include "Chain.h"
#include "Item.h"

namespace dungeon {

struct Hero {
    std::string         heroName;
    Bag<Item>           inventory;   // Floor 3: Bag<Item> (sortable, indexable).
    Chain<std::string>  eventLog;    // Floor 4: prepend-only history, newest at head.
};

// Load a starting inventory from a pipe-delimited text file.
// File format, one per line:  name|weight|value
// Blank lines and lines starting with '#' are ignored.
// On read failure returns an empty Bag.
Bag<Item> loadInventory(const std::string& path);

// Pretty-print the hero's inventory, numbered, to stdout.
void printInventory(const Hero& hero);

// Print the most recent `n` entries of the hero's event log, newest
// first. If `n` is 0 or larger than the chain, prints the whole chain.
// Walks from eventLog.head() following `next` pointers — Floor 4's
// version, unchanged.
void printLog(const Hero& hero, std::size_t n);

// Print the oldest `n` entries of the event log, oldest first. If `n`
// is 0 or larger than the chain, prints the whole chain.
//
// TODO Floor 4½ (Monday) — implement in Hero.cpp. Walk from
// eventLog.tail() following `prev` pointers. This is the command that
// proves your new pointers actually point.
void printLogOldest(const Hero& hero, std::size_t n);

}  // namespace dungeon
