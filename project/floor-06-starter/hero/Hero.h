// COMP 2450 — Floor 6 starter
// hero/Hero.h — Hero gains a third container this week: a Stack of
// UndoActions. Every mutating command in main.cpp snapshots
// hero.inventory and pushes it onto hero.undoStack before doing its
// work; the new `undo` command pops the top action and copies its
// snapshot back over hero.inventory.
//
// The printLog template from Floor 5 is unchanged — it walks any
// iterator pair and is reused for `log` (begin/end) and
// `log --oldest` (rbegin/rend).

#pragma once

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

#include "Bag.h"
#include "Chain.h"
#include "Item.h"
#include "Stack.h"
#include "UndoAction.h"

namespace dungeon {

struct Hero {
    std::string         heroName;
    Bag<Item>           inventory;
    Chain<std::string>  eventLog;
    Stack<UndoAction>   undoStack;     // Floor 6: most-recent mutating command on top.
};

Bag<Item> loadInventory(const std::string& path);
void      printInventory(const Hero& hero);

// Print up to `n` elements walked from [first, last), one per line,
// followed by a "(<label>; chain length K)" footer. Templated on the
// iterator type — works with Chain<T>::iterator, Chain<T>::reverse_iterator,
// Bag<T>::iterator, or anything else satisfying input-iterator.
//
// We use an explicit `it != last` loop here rather than range-based for
// because we want a count and an early stop at `n`. The lesson is the
// same either way: ONE function, ANY container.
template <typename It>
void printLog(It first, It last, std::size_t n, const char* label, std::size_t total) {
    if (first == last) {
        std::cout << "  (the chain is empty — nothing to remember yet)\n";
        return;
    }
    std::size_t printed = 0;
    for (It it = first; it != last && (n == 0 || printed < n); ++it, ++printed) {
        std::cout << "  " << std::setw(2) << std::right
                  << (printed + 1) << ".  " << *it << "\n";
    }
    std::cout << "  (" << label << "; chain length " << total << ")\n";
}

}  // namespace dungeon
