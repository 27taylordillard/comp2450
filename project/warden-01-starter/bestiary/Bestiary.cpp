// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// bestiary/Bestiary.cpp — post-Floor-3 reference. Loader returns a
// Bag<Monster>; sortBestiary takes a Bag<Monster>&. The Bag's begin()
// and end() forward to the underlying std::vector's iterators, so
// std::sort (and any STL algorithm) works on a Bag the same way it
// works on a vector.

#include "Bestiary.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace dungeon {

Bag<Monster> loadBestiary(const std::string& path) {
    Bag<Monster> result;
    std::ifstream in(path);
    if (!in) {
        std::cerr << "The Bestiary tablet is missing or unreadable: "
                  << path << "\n";
        return result;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        Monster m;
        std::istringstream ss(line);
        std::string field;

        if (!std::getline(ss, m.name, '|')) continue;
        if (!std::getline(ss, field,  '|')) continue;
        m.hp = std::stoi(field);
        if (!std::getline(ss, field,  '|')) continue;
        m.attack = std::stoi(field);
        if (!std::getline(ss, m.weakness, '|')) continue;

        result.push_back(m);
    }
    return result;
}

void sortBestiary(Bag<Monster>& bestiary) {
    std::sort(bestiary.begin(), bestiary.end(),
              [](const Monster& a, const Monster& b) {
                  return a.name < b.name;
              });
}

void printMonster(const Monster& m) {
    std::cout << m.name
              << "   HP " << m.hp
              << "   ATK " << m.attack
              << "   weakness: " << m.weakness
              << "\n";
}

}  // namespace dungeon
