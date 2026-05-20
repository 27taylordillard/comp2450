// COMP 2450 — Floor 7 starter
// hero/Hero.h — Hero gains a fourth container this week: a Queue of
// enemy-action strings. The new `provoke <name>` command enqueues an
// action onto hero.enemyActionQueue; the new `simulate` command drains
// the queue in FIFO order. Last week's Stack<UndoAction> from Floor 6
// is unchanged.
//
// The Hero now owns one of each shape we have built so far:
//   * Bag<Item>            inventory       — random-access container (Floor 3)
//   * Chain<std::string>   eventLog        — chronological list (Floor 4½/5)
//   * Stack<UndoAction>    undoStack       — LIFO history (Floor 6)
//   * Queue<std::string>   enemyActionQueue — FIFO line (Floor 7, NEW)
//
// Same Hero. Four containers. Each one was chosen to match its access
// pattern. That match is the lesson.
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
#include "Queue.h"
#include "Stack.h"
#include "UndoAction.h"

namespace dungeon {

struct Hero {
    std::string         heroName;
    Bag<Item>           inventory;
    Chain<std::string>  eventLog;
    Stack<UndoAction>   undoStack;          // Floor 6: most-recent mutating command on top.
    Queue<std::string>  enemyActionQueue;   // Floor 7: enemies in line, FIFO.
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
