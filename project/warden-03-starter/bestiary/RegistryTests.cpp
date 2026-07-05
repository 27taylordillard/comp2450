// COMP 2450 — Floor 10 starter
// bestiary/RegistryTests.cpp — the `selftest registry` harness. Provided. Do not edit.
//
// Six phases over a small FIXED sample Registry, built from an in-test
// Bag<Monster> so the expected values are pinned and never depend on
// data/monsters.txt (exactly as BSTTests builds its fixed {5,3,8,2,4,7,9}).
//
// The lesson is encoded in the order: index populates all three containers
// (phase 1), membership against the MAP (phase 2), the indexed lookup that
// replaces Floor 9's whole descent (phase 3), membership against the SET
// (phase 4), counts read without the Phantom Key (phase 5), and a mutation that
// keeps three containers consistent — dropping a weakness whose last bearer
// leaves (phase 6).
//
// Fixed sample (do NOT change — the numbers below depend on it):
//     { "Goblin",   8, 2, "fire"  }
//     { "Wraith",  14, 4, "holy"  }
//     { "Skeleton",10, 3, "blunt" }
//     { "Lich",    35, 6, "holy"  }
//     { "Wisp",     3, 1, "holy"  }
// -> distinct names = 5; distinct weaknesses = {blunt, fire, holy} (3);
//    counts = {blunt:1, fire:1, holy:3}.
//
// MWF: Monday's index + knows green phases 1–2; Wednesday's recall +
// hasWeakness + countWeakness green phases 3–5; Friday's forget greens phase 6.

#include "RegistryTests.h"

#include <iostream>

#include "../hero/Bag.h"
#include "Monster.h"
#include "Registry.h"

namespace dungeon {

namespace {

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

// Build the fixed five-monster sample bag, in this exact order.
Bag<Monster> buildSampleBag() {
    Bag<Monster> b;
    b.push_back({"Goblin",    8, 2, "fire"});
    b.push_back({"Wraith",   14, 4, "holy"});
    b.push_back({"Skeleton", 10, 3, "blunt"});
    b.push_back({"Lich",     35, 6, "holy"});
    b.push_back({"Wisp",      3, 1, "holy"});
    return b;
}

}  // anonymous namespace

bool runRegistrySelfTest() {
    bool all = true;

    // --- Phase 1: index populated all three containers ---
    {
        Registry reg(buildSampleBag());
        bool ok = (reg.size() == 5 && reg.weaknesses().size() == 3);
        if (ok) {
            auto it = reg.weaknessCounts().find("holy");
            ok = (it != reg.weaknessCounts().end() && it->second == 3);
        }
        result("index: size==5, distinct weaknesses==3, count[holy]==3", ok,
               "index must add to byName_, weaknesses_ (a set dedups), AND bump weaknessCounts_[m.weakness]");
        all = all && ok;
    }

    // --- Phase 2: knows — membership against the map ---
    {
        Registry reg(buildSampleBag());
        const bool ok = (reg.knows("Lich") && !reg.knows("Drake"));
        result("knows: Lich present, Drake absent", ok,
               "knows must use byName_.count(name) (or find != end), not a loop; absent name returns false");
        all = all && ok;
    }

    // --- Phase 3: recall — the indexed lookup ---
    {
        Registry reg(buildSampleBag());
        const Monster* w = reg.recall("Wraith");
        const Monster* d = reg.recall("Drake");
        const bool ok = (w != nullptr && w->hp == 14 && d == nullptr);
        result("recall: Wraith->hp==14, Drake==nullptr", ok,
               "recall must map::find and return &it->second, or nullptr at end()");
        all = all && ok;
    }

    // --- Phase 4: hasWeakness — membership against the set ---
    {
        Registry reg(buildSampleBag());
        const bool ok = (reg.hasWeakness("holy") && !reg.hasWeakness("water"));
        result("hasWeakness: holy present, water absent", ok,
               "hasWeakness must use weaknesses_.count(w) on the set, not scan the bestiary");
        all = all && ok;
    }

    // --- Phase 5: countWeakness — counts without the Phantom Key ---
    {
        Registry reg(buildSampleBag());
        const bool ok = (reg.countWeakness("holy") == 3 && reg.countWeakness("water") == 0);
        result("countWeakness: holy==3, water==0", ok,
               "countWeakness must look up weaknessCounts_ with find and return 0 for a missing key (never operator[])");
        all = all && ok;
    }

    // --- Phase 6: forget — mutate, staying consistent across containers ---
    {
        Registry reg(buildSampleBag());
        // Remove one of three holy monsters: count drops to 2, holy stays.
        bool ok = reg.forget("Wisp");
        ok = ok && reg.size() == 4
                && reg.countWeakness("holy") == 2
                && reg.hasWeakness("holy");
        // Remove the only blunt monster: blunt's count hits 0 and must be
        // erased from BOTH maps and the set.
        ok = ok && reg.forget("Skeleton")
                && !reg.hasWeakness("blunt")
                && reg.weaknesses().size() == 2;
        // Absent name removes nothing.
        ok = ok && !reg.forget("Drake");
        result("forget: drop a shared weakness (count--), drop a last weakness (erased), absent==false", ok,
               "forget must erase from byName_, decrement the count, and when a weakness count hits 0 erase it from BOTH weaknessCounts_ and weaknesses_; absent name returns false");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
