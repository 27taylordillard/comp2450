// COMP 2450 — Floor 1 starter
// bestiary/Monster.h — provided by the framework. Do not edit.
#pragma once
#include <string>

namespace dungeon {

struct Monster {
    std::string name;
    int         hp;
    int         attack;
    std::string weakness;
};

// Floor 9: order monsters by name so a BST<Monster> keys on the name. The
// BST only ever compares with `<` (never `==`) — it treats "neither less
// nor greater" as equal — so this single operator is all it needs.
inline bool operator<(const Monster& a, const Monster& b) {
    return a.name < b.name;
}

}
