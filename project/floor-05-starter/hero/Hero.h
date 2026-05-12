// COMP 2450 — Floor 5 starter
// hero/Hero.h — Floor 4½'s two printLog functions (one walking head→tail,
// one walking tail→head) collapse into ONE templated printLog that takes
// a pair of iterators and does not care which container they came from.
//
// `main.cpp` now calls printLog with begin/end for newest-first and
// rbegin/rend for oldest-first. The same template instantiates on a
// Chain<std::string>::iterator AND a Bag<Item>::iterator without
// modification — that's the whole point of Floor 5.

#pragma once

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

#include "Bag.h"
#include "Chain.h"
#include "Item.h"

namespace dungeon {

struct Hero {
    std::string         heroName;
    Bag<Item>           inventory;
    Chain<std::string>  eventLog;
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
