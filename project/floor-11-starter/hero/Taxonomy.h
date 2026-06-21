// COMP 2450 — Floor 8 starter
// hero/Taxonomy.h — load and pretty-print Scrivener McCown's
// classification tree. PROVIDED framework; you do not edit this.
//
// Lives in hero/ (beside Tree.h and the other container harnesses) so it
// can include "Tree.h" as a sibling — the project only does cross-folder
// includes from main.cpp. The monster names it loads are just string data.
//
// loadTaxonomy() reads data/taxonomy.txt (one root-to-leaf path per line)
// and builds a Tree<std::string> by calling your Tree<T>::makeRoot and
// Tree<T>::addChild. Until you implement addChild (Monday), the loader
// builds the root only and stops — `taxonomy` will print a single line.
//
// printTaxonomy() is a worked example of recursion over a tree: it walks
// the structure depth-first, indenting one step per level. Read it before
// you write preOrderInto — it is the same recursion shape, minus the
// std::vector and plus the indentation.

#pragma once

#include <string>

#include "Tree.h"

namespace dungeon {

// Build the classification tree from a path-per-line file. Returns an
// empty tree if the file cannot be opened.
Tree<std::string> loadTaxonomy(const std::string& path);

// Print the tree as an indented outline (depth-first, one indent per level)
// followed by a "(N creatures and categories; height H)" footer.
void printTaxonomy(const Tree<std::string>& tree);

}  // namespace dungeon
