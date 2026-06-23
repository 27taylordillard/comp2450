// COMP 2450 — Floor 6 starter
// bestiary/Search.h — generic findByName<T> + the Floor 1 Monster-only
// searches kept around for benchmark/Bench.cpp.
//
// As-of state at the start of Floor 6: the Floor 5 std::find_if rewrite
// is in place. findByName<T> is one line, and it works against any
// container with begin/end iterators that yield T& — Bag<T>, Chain<T>,
// or anything else that satisfies the iterator contract. You will not
// edit this file on Floor 6.

#pragma once

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

#include "../hero/Bag.h"
#include "Monster.h"

namespace dungeon {

// ---- Floor 1 reference: Monster-specific searches (unchanged) --------
const Monster* linearSearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

const Monster* binarySearch(const std::vector<Monster>& bestiary,
                            const std::string&         name);

const Monster* binarySearchRecursive(const std::vector<Monster>& bestiary,
                                     const std::string&         name);

const Monster* findMonster(const std::vector<Monster>& bestiary,
                           const std::string&         name);


// ---- findByName: works on any iterable container of named items -----
//
// std::find_if walks the container using the container's own iterator
// and a predicate that names which match we want. By satisfying the
// iterator contract, the SAME one line works against Bag<T> AND
// Chain<T> in main.cpp's search command.
//
// Return convention: pointer at the first match, or nullptr. We
// dereference the iterator to get a reference, then take its address.

template <typename T>
const T* findByName(const Bag<T>& items, const std::string& name) {
    auto it = std::find_if(items.begin(), items.end(),
                           [&name](const T& x) { return x.name == name; });
    return (it != items.end()) ? &*it : nullptr;
}

}  // namespace dungeon
