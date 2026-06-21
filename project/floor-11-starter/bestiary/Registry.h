// COMP 2450 — Floor 11 starter
// bestiary/Registry.h — the Hall of Names, FINISHED. This is the completed
// Floor 10 work, shipped as reference code: you do NOT edit it on Floor 11.
//
// Registry re-indexes McCown's bestiary (the same Bag<Monster> loaded from
// data/monsters.txt) into three standard associative containers, added in
// parallel to the Bag and the BST:
//
//   byName_         std::map<std::string, Monster>  — the bestiary indexed by
//                   name. One declaration + one find() does what the whole
//                   hand-written BST<Monster> did on Floor 9.
//   weaknesses_     std::set<std::string>           — the DISTINCT weaknesses
//                   present (the raw data has duplicates). A set collapses
//                   duplicates, stays sorted, answers membership in one call.
//   weaknessCounts_ std::map<std::string, int>      — how many monsters share
//                   each weakness.
//
// The Floor 10 lesson, preserved here for reference: use the library. Every
// method body is a std::set/std::map/std::pair call, not a loop — that was the
// point. operator[] inserts on read (the "Phantom Key"), so countWeakness reads
// with find() and is const, which makes the bug a compile error.

#pragma once

#include <cstddef>
#include <map>
#include <set>
#include <string>

#include "../hero/Bag.h"
#include "Monster.h"

namespace dungeon {

class Registry {
public:
    // Walk the bestiary in FILE order and index() each monster. The set/maps
    // sort on insert, so the output comes out alphabetical despite the
    // out-of-order inserts — the "free sort" of Floor 9, with no tree code.
    explicit Registry(const Bag<Monster>& bestiary) {
        for (const Monster& m : bestiary) index(m);
    }

    std::size_t size()  const { return byName_.size(); }
    bool        empty() const { return byName_.empty(); }

    const std::map<std::string, Monster>& byName()         const { return byName_; }
    const std::set<std::string>&          weaknesses()     const { return weaknesses_; }
    const std::map<std::string, int>&     weaknessCounts() const { return weaknessCounts_; }

    // Insert m into all three containers. byName_ and weaknessCounts_ use
    // operator[] because we MEAN to create/update here (that is the correct use
    // of []); weaknesses_ is a set, so insert() silently no-ops on a duplicate.
    void index(const Monster& m) {
        byName_[m.name] = m;
        weaknesses_.insert(m.weakness);
        weaknessCounts_[m.weakness] += 1;
    }

    // Membership in one call — count() returns 0 or 1 on a map (unique keys).
    bool knows(const std::string& name) const {
        return byName_.count(name) > 0;
    }

    // The one-liner that replaces Floor 9's whole recursive descent.
    const Monster* recall(const std::string& name) const {
        auto it = byName_.find(name);
        return it == byName_.end() ? nullptr : &it->second;
    }

    // Membership against the SET this time (contrast with knows()'s MAP).
    bool hasWeakness(const std::string& w) const {
        return weaknesses_.count(w) > 0;
    }

    // Read a count with find(), never operator[]. This method is const, so []
    // would not even compile here — the const-ness makes the Phantom Key bug
    // mechanically impossible.
    int countWeakness(const std::string& w) const {
        auto it = weaknessCounts_.find(w);
        return it == weaknessCounts_.end() ? 0 : it->second;
    }

    // Remove the named monster and keep the derived structures consistent: read
    // the weakness BEFORE erasing the record, then decrement the count; if it
    // hits 0, the last bearer is gone, so drop the weakness from BOTH the count
    // map and the distinct-weakness set. Returns whether anything was removed.
    bool forget(const std::string& name) {
        auto it = byName_.find(name);
        if (it == byName_.end()) return false;
        const std::string w = it->second.weakness;
        byName_.erase(it);
        if (--weaknessCounts_[w] == 0) {
            weaknessCounts_.erase(w);
            weaknesses_.erase(w);
        }
        return true;
    }

private:
    std::map<std::string, Monster> byName_;          // name -> the record
    std::set<std::string>          weaknesses_;       // distinct weaknesses
    std::map<std::string, int>     weaknessCounts_;   // weakness -> how many
};

}  // namespace dungeon
