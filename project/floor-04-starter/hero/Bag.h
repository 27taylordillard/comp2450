// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// hero/Bag.h — completed Floor 3 (Wednesday + Friday) reference.
//
// A class template. One class declaration; the compiler stamps out a
// fresh copy for every type T you instantiate. By Floor 3's Friday this
// same class backs two very different things — hero.inventory is a
// Bag<Item>, the bestiary is a Bag<Monster>. One mold, two shapes.
//
// What this class owns: nothing fancy. The actual storage is a private
// std::vector<T>. Bag<T> is a WRAPPER — it is NOT a reinvention of
// vector. The point was the template MACHINERY, not the data layout.
// Storage is reinvented on Floor 4 (linked lists), where node ownership
// forces the new/delete/destructor conversation.
//
// Why the bodies live in the header:
//   Template bodies must be visible to every caller, so the compiler
//   can stamp the right version for each T. A .cpp file would only
//   compile the template for the types it happened to name inside
//   itself — useless for a container. Class templates live in headers.
//   That is a language rule, not a style choice.

#pragma once

#include <cstddef>
#include <vector>

#include "BagException.h"

namespace dungeon {

template <typename T>
class Bag {
public:
    // ---- inspection ----------------------------------------------------

    std::size_t size() const  { return data_.size();  }
    bool        empty() const { return data_.empty(); }

    // ---- access — unchecked --------------------------------------------
    //
    // operator[] does NOT bounds-check. It is the fast path. Callers
    // who KNOW their index is in range use this — for instance, the
    // body of a `for (size_t i = 0; i < bag.size(); ++i)` loop can't
    // possibly be out of range. When the index came from USER INPUT,
    // prefer at() (below).

    const T& operator[](std::size_t i) const { return data_[i]; }
          T& operator[](std::size_t i)       { return data_[i]; }

    // ---- access — checked ----------------------------------------------
    //
    // at() bounds-checks and THROWS a BagException when the caller asks
    // for something impossible. Use this when the index came from user
    // input — when the main loop handles `inspect 99` but the bag only
    // holds five items, at() throws, the try/catch in main catches, the
    // game keeps going.

    const T& at(std::size_t i) const {
        if (i >= data_.size()) throw BagException(i, data_.size());
        return data_[i];
    }
    T& at(std::size_t i) {
        if (i >= data_.size()) throw BagException(i, data_.size());
        return data_[i];
    }

    // ---- mutation ------------------------------------------------------

    void push_back(const T& value) { data_.push_back(value); }
    void clear()                   { data_.clear(); }

    // ---- iteration -----------------------------------------------------
    //
    // begin()/end() are what lets `for (auto& x : bag)` work AND lets
    // std::sort / std::find / every STL algorithm work on a Bag. Delegate
    // to the underlying vector's iterators.
    //
    // Why both const and non-const overloads? A function that takes
    // `const Bag<T>&` (like findByName<T> does) only sees the const
    // pair. A function that wants to MUTATE the bag sees the non-const
    // pair. The compiler picks based on what kind of reference you have.

    using iterator       = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator       begin()       { return data_.begin(); }
    iterator       end()         { return data_.end();   }
    const_iterator begin() const { return data_.begin(); }
    const_iterator end()   const { return data_.end();   }

private:
    std::vector<T> data_;
};

}  // namespace dungeon
