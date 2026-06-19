// COMP 2450 — Floor 8 starter
// hero/Taxonomy.cpp — implementation of the taxonomy loader + printer.
// PROVIDED framework; you do not edit this.

#include "Taxonomy.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace dungeon {

namespace {

using Node = Tree<std::string>::Node;

// Linear scan of a node's existing children for one labelled `label`.
// Returns it, or nullptr if this parent has no such child yet. This is
// the find-or-create plumbing the loader needs; it is deliberately simple
// (a tree is not a search structure yet — that is Floor 9's BST).
Node* childNamed(Node* parent, const std::string& label) {
    if (parent == nullptr) return nullptr;
    for (std::size_t i = 0; i < parent->children.size(); ++i) {
        if (parent->children[i]->data == label) return parent->children[i];
    }
    return nullptr;
}

// Split "A/B/C" into {"A","B","C"}, trimming a trailing '\r' so files saved
// with Windows line endings load the same as Unix ones.
std::vector<std::string> splitPath(const std::string& line) {
    std::vector<std::string> labels;
    std::stringstream ss(line);
    std::string label;
    while (std::getline(ss, label, '/')) {
        if (!label.empty() && label.back() == '\r') label.pop_back();
        labels.push_back(label);
    }
    return labels;
}

// Recursive indented print — the worked example. Note the shape: act on
// this node (print it, indented by depth), then recurse into each child
// at depth+1. preOrderInto (your Wednesday TODO) is this same walk.
void printSubtree(const Node* n, std::size_t depth) {
    if (n == nullptr) return;
    for (std::size_t i = 0; i < depth; ++i) std::cout << "    ";
    std::cout << (depth == 0 ? "" : "- ") << n->data << "\n";
    for (std::size_t i = 0; i < n->children.size(); ++i) {
        printSubtree(n->children[i], depth + 1);
    }
}

}  // anonymous namespace

Tree<std::string> loadTaxonomy(const std::string& path) {
    Tree<std::string> tree;
    std::ifstream in(path);
    if (!in) return tree;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::vector<std::string> labels = splitPath(line);
        if (labels.empty() || labels[0].empty()) continue;

        Node* cur = tree.makeRoot(labels[0]);
        for (std::size_t i = 1; i < labels.size() && cur != nullptr; ++i) {
            Node* next = childNamed(cur, labels[i]);
            if (next == nullptr) next = tree.addChild(cur, labels[i]);
            cur = next;   // nullptr if addChild is still stubbed — loop guard stops us safely
        }
    }
    return tree;
}

void printTaxonomy(const Tree<std::string>& tree) {
    if (tree.empty()) {
        std::cout << "  (the catalogue is empty)\n";
        return;
    }
    printSubtree(tree.root(), 0);
    std::cout << "  (" << tree.size() << " creatures and categories; height "
              << tree.height() << ")\n";
}

}  // namespace dungeon
