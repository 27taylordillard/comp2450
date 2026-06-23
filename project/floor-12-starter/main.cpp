// COMP 2450 — The Descent
// Floor 12 — The Labyrinth — starter  (THE FINALE)
//
// This is the post-Floor-11 reference codebase plus this week's Floor-12 stubs.
// Everything Floors 0–11 delivered is in place and WORKING (no longer stubbed):
// Bag<T>, the sorts, the boss battle, Chain<T> with iterators, Stack<T> and
// Queue<T>, isBalanced + `lint`, `undo`, `provoke`/`simulate`, `potato`, the
// general Tree<T> with `taxonomy`/`lineage`, the full Floor 9 BST<T>, the Floor
// 9½ balancing, the Floor 10 Registry (std::set/std::map), AND the Floor 11
// HashTable. `catalogue`, `recall`, `forget`, `balance`, `knows`, `lookup`,
// `strike`, `names`, `weaknesses`, `weakness`, `hash`, `buckets`, `whisper`,
// `banish`, `load`, and every `benchmark` so far all work.
//
// This is the LAST floor of the dungeon — the finale. For eleven floors every
// structure stored ITEMS. The Labyrinth stores none: it stores the CONNECTIONS
// between rooms — which passage opens onto which — as an undirected, unweighted
// adjacency-list graph (ZyBook Ch 13). And crossing it does not build an eighth
// container; it REACHES BACK for two you already built: breadth-first search
// rides your Floor 7 Queue, depth-first search rides your Floor 6 Stack, and a
// hash set (the std::unordered_set you met on Floor 11) is the "visited" memory
// that keeps a cycle from looping forever. The descent was never about storing.
// It is about connecting.
//
// What is NEW for Floor 12 (all in hero/Graph.h — six small bodies):
//   * addRoom / connect     — a room is a vertex; a passage is an UNDIRECTED
//                             edge (both directions). Stubbed (Mon).
//   * neighbors             — the rooms one door away (the adjacency list). Stubbed (Mon).
//   * reachable / bfsOrder  — cross it breadth-first over your Queue + a visited
//                             set; reachability and level-by-level order. Stubbed (Wed).
//   * dfsOrder              — cross it depth-first over your Stack; same map,
//                             opposite order. Stubbed (Fri).
//   * hops                  — PROVIDED: a BFS that counts the shortest way down
//                             (rides your neighbors). You do NOT write it.
//   * hero/GraphTests.*     — `selftest crossing` exercises all of it (6 phases).
//   * `map` / `doors` / `reach` / `cross` / `descend`
//                             — the REPL demos (fresh verbs; nothing earlier uses them).
//   * `benchmark crossing [N]` — adjacency LIST vs adjacency MATRIX crossing.
//                             The §13.4 contrast: sparse list flat-ish, matrix O(V²).
//
// What you do this week (see the Floor 12 page):
//   Mon — addRoom, connect, neighbors; `selftest crossing` phases 1–3 pass;
//         `map` prints every room and its doors; `doors <room>` lists one room's
//         passages. The map is built; nothing is traversed yet.
//   Wed — reachable, bfsOrder; phases 4–5 pass; `reach <from> <to>` answers
//         yes/no; `cross <start> --bfs` walks level by level; `descend <from>
//         <to>` reports the shortest way down (the Queue + the visited set).
//   Fri — dfsOrder; phase 6 passes; `cross <start> --dfs` plunges deep (a
//         DIFFERENT order from --bfs on the SAME map, over your Stack);
//         `benchmark crossing` lands the list-vs-matrix payoff. Then the
//         whole-arc wrap and the Final Descent.

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "battle/Battle.h"
#include "bestiary/Bench.h"
#include "bestiary/Bestiary.h"
#include "bestiary/Registry.h"
#include "bestiary/RegistryTests.h"
#include "bestiary/Search.h"
#include "hero/Bench.h"
#include "hero/ChainBench.h"
#include "hero/ChainTests.h"
#include "hero/Graph.h"
#include "hero/GraphTests.h"
#include "hero/HashTable.h"
#include "hero/HashTableTests.h"
#include "hero/Hero.h"
#include "hero/IteratorTests.h"
#include "hero/Lint.h"
#include "hero/Potato.h"
#include "hero/QueueTests.h"
#include "hero/Sort.h"
#include "hero/StackTests.h"
#include "hero/BST.h"
#include "hero/BSTTests.h"
#include "hero/BalanceTests.h"
#include "hero/Taxonomy.h"
#include "hero/TreeTests.h"

using namespace dungeon;

namespace {

void splitFirst(const std::string& line, std::string& cmd, std::string& rest) {
    auto sp = line.find(' ');
    if (sp == std::string::npos) { cmd = line; rest.clear(); }
    else { cmd = line.substr(0, sp); rest = line.substr(sp + 1); }
}

void printHelp() {
    std::cout << "(commands:\n"
              << "   search <name>                 — look up by name in bestiary, inventory, OR event log\n"
              << "   list                          — list the bestiary\n"
              << "   inventory                     — list your inventory\n"
              << "   inspect <n>                   — show the nth item in your inventory\n"
              << "   take <name>                   — add a small item by that name to your inventory\n"
              << "   sort inventory by <key> [asc|desc]\n"
              << "                                 — key is name, weight, or value\n"
              << "   undo                          — reverse the most recent mutating command\n"
              << "   lint <text>                   — check that brackets/parens/braces in <text> balance\n"
              << "   provoke <name>                — enqueue an enemy action onto the line\n"
              << "   simulate                      — drain the enemy line in FIFO order\n"
              << "   potato <name>... <k>          — classical hot-potato survivor (FIFO elimination)\n"
              << "   taxonomy                      — print McCown's classification tree as an outline\n"
              << "   taxonomy --dfs                — list every entry depth-first (pre-order)\n"
              << "   taxonomy --bfs                — list every entry breadth-first (level-order)\n"
              << "   lineage <name>                — show the root-to-creature classification path\n"
              << "   catalogue                     — list the bestiary in name order (BST in-order)\n"
              << "   recall <name>                 — look up a monster fast in the BST\n"
              << "   forget <name>                 — remove a monster from the BST\n"
              << "   balance                       — is McCown's catalogue tree balanced? (height + factor)\n"
              << "   knows <name>                  — does the Registry (std::map) know this name?\n"
              << "   lookup <name>                 — look up a monster in the Registry (one std::map::find)\n"
              << "   strike <name>                 — remove a monster from the Registry (keeps the maps consistent)\n"
              << "   names                         — list the bestiary in name order (std::map + structured bindings)\n"
              << "   weaknesses                    — the distinct weaknesses (std::set), each with its count\n"
              << "   weakness <w>                  — how many monsters fear <w>? (set membership + map count)\n"
              << "   hash <name>                   — show hashKey(name) and the bucket it computes to\n"
              << "   buckets                       — print the hash table: which names share each bucket (collisions)\n"
              << "   whisper <name>                — look up a monster in the HashTable (scan one bucket, O(1) avg)\n"
              << "   banish <name>                 — remove a monster from the HashTable (only the HashTable)\n"
              << "   load                          — show count, bucketCount, and load factor (rehash made visible)\n"
              << "   map                           — print the Labyrinth: every room and the doors out of it\n"
              << "   doors <room>                  — list the rooms one passage away (the room's neighbors)\n"
              << "   reach <from> <to>             — is there any path from <from> to <to>? (BFS)\n"
              << "   cross <start> --bfs           — walk the map breadth-first from <start> (level by level)\n"
              << "   cross <start> --dfs           — walk the map depth-first from <start> (plunge, then backtrack)\n"
              << "   descend <from> <to>           — the shortest way down, in doors (BFS distance)\n"
              << "   log [n]                       — show the last n events, newest first\n"
              << "   log --oldest [n]              — show the first n events, oldest first\n"
              << "   clone hero                    — deep-copy the hero, print both logs, let the copy die\n"
              << "   selftest chain                — Chain<T> leak + deep-copy harness\n"
              << "   selftest iterator             — Chain<T>::iterator harness\n"
              << "   selftest stack                — Stack<T> push/pop/top/LIFO harness\n"
              << "   selftest queue                — Queue<T> enqueue/dequeue/front/FIFO harness\n"
              << "   selftest tree                 — Tree<T> addChild/size/height/traversal/lineage harness\n"
              << "   selftest bst                  — BST<T> insert/find/inorder/remove harness\n"
              << "   selftest balance              — BST<T> balance factor / rotation harness\n"
              << "   selftest registry             — Registry std::set/std::map/std::pair harness\n"
              << "   selftest whispers             — HashTable hash/insert/find/erase/rehash harness\n"
              << "   selftest crossing             — Graph addRoom/connect/neighbors/reachable/bfs/dfs harness\n"
              << "   benchmark [N]                 — race the search algorithms\n"
              << "   benchmark sort [N] [--sorted] [--bad-pivot]\n"
              << "                                 — race the sorting algorithms\n"
              << "   benchmark log [N]             — race Chain::push_front vs vector insert(begin)\n"
              << "   benchmark bst [N]             — race linear scan vs BST lookup over the bestiary\n"
              << "   benchmark balance [N]         — same sorted input: plain BST (stick) vs std::set\n"
              << "   benchmark names [N]           — std::map::find vs BST::find vs linear scan\n"
              << "   benchmark whispers [N]        — your HashTable vs unordered_map vs std::map vs BST vs linear\n"
              << "   benchmark crossing [N]        — adjacency LIST vs adjacency MATRIX graph crossing (BFS)\n"
              << "   battle warden                 — face the Warden of the Foundations\n"
              << "   help                          — this screen\n"
              << "   quit                          — leave the dungeon)\n";
}

// Snapshot hero.inventory and push it onto hero.undoStack with the given
// description. Call BEFORE mutating the inventory. The push is what makes
// the next `undo` know what to restore. (Floor 6 wiring — unchanged.)
void recordUndo(Hero& hero, const std::string& description) {
    UndoAction a;
    a.description       = description;
    a.inventorySnapshot = hero.inventory;
    hero.undoStack.push(a);
}

}  // anonymous namespace

int main() {
    std::cout << "\n=== THE LABYRINTH ===\n\n";

    std::cout << "What is your name, adventurer? ";
    Hero hero;
    std::getline(std::cin, hero.heroName);
    if (hero.heroName.empty()) hero.heroName = "Nameless One";

    std::cout << "\nWelcome, " << hero.heroName << ".\n"
              << "The shelves and niches are all behind you now. This last hall holds no\n"
              << "beasts at all — only the dark between the doors. The Nameless Bard walks\n"
              << "a pace ahead, tuning a low string. \"Eleven halls you filled, and locked\n"
              << "each beast in its place,\" he says. \"This one keeps nothing — only which\n"
              << "passage opens onto which. A map is not a list of rooms; it is a web of\n"
              << "ways between them. The only questions left are CAN you reach the bottom,\n"
              << "and by the SHORTEST road.\" He smiles. \"You will not build a new thing to\n"
              << "cross it. You will reach back: the Queue you built becomes the\n"
              << "breadth-first walk, the Stack becomes the deep one, and the hash set\n"
              << "remembers where you have been — so a corridor that loops on itself does\n"
              << "not walk you in circles forever.\"\n\n";

    Bag<Monster> bestiary = loadBestiary("data/monsters.txt");
    if (bestiary.empty()) {
        std::cerr << "The Bestiary tablet is blank. Cannot proceed.\n";
        return 1;
    }
    // Floor 9 (finished reference): McCown's bestiary, rebuilt as a binary
    // search tree keyed by name. Insert in FILE order (deliberately NOT sorted —
    // inserting already-sorted data degenerates a BST into a stick, the worst
    // case). `catalogue`/`recall`/`forget`/`balance` already work.
    BST<Monster> bestiaryTree;
    for (const Monster& m : bestiary) bestiaryTree.insert(m);

    sortBestiary(bestiary);

    // Floor 10 (finished reference): the bestiary as standard associative
    // containers — a std::map keyed by name and a std::set of distinct
    // weaknesses. The balanced tree of Floor 9½, handed to you by the library.
    // Added IN PARALLEL: the Bag and the BST above are untouched.
    // `knows`/`lookup`/`strike`/`names`/`weaknesses`/`weakness` already work.
    Registry registry(bestiary);

    // Floor 11 (finished reference): the bestiary as a hand-built hash table —
    // an array of the Chain<Monster> buckets you wrote on Floor 4, with your own
    // hash function. It starts with 8 buckets; inserting all 15 monsters crosses
    // the load-factor threshold and the table GROWS itself (rehash) — try `load`
    // and `buckets` to watch it. Added IN PARALLEL: the Bag, the BST, and the
    // Registry above are untouched. `hash`/`buckets`/`whisper`/`banish`/`load`
    // already work (this floor does not touch the HashTable).
    HashTable whispers(bestiary);

    // Floor 12 (the finale): the dungeon MAP as an undirected, unweighted
    // adjacency-list graph — rooms are vertices, passages are edges. Every hall
    // above stored ITEMS; the Labyrinth stores the CONNECTIONS between them.
    // Crossing it reuses two structures you already built: BFS rides your Floor 7
    // Queue, DFS rides your Floor 6 Stack, and a hash set (Floor 11) is the
    // "visited" memory that keeps a cycle from looping forever. It is the first
    // index NOT derived from the bestiary Bag — it is built from its own data
    // file. Until your connect()/addRoom() work (Monday), the map is empty and
    // `map` prints nothing.
    Graph labyrinth("data/labyrinth.txt");

    // Floor 8 (finished reference): McCown's classification tree, built from
    // data/taxonomy.txt. `taxonomy`/`taxonomy --dfs/--bfs`/`lineage` already work.
    Tree<std::string> taxonomy = loadTaxonomy("data/taxonomy.txt");

    hero.inventory = loadInventory("data/starter-inventory.txt");
    if (hero.inventory.empty()) {
        std::cerr << "Your satchel is empty. Cannot proceed.\n";
        return 1;
    }

    hero.eventLog.push_front("began session as \"" + hero.heroName + "\"");

    printHelp();
    std::cout << "\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        try {
            std::string cmd, rest;
            splitFirst(line, cmd, rest);

            if (cmd == "quit" || cmd == "exit") {
                std::cout << "The Bard lets the string fall silent. \"Go well. I have your\n"
                          << "ending already — I have been singing it since the first floor.\"\n";
                break;
            }
            else if (cmd == "help") {
                printHelp();
            }
            else if (cmd == "list") {
                for (const auto& m : bestiary) printMonster(m);
                hero.eventLog.push_front("list — bestiary printed");
            }
            else if (cmd == "search") {
                if (rest.empty()) {
                    std::cout << "Speak a name:  search <monster, item, or substring of a past event>\n";
                    continue;
                }
                if (const Monster* m = findByName<Monster>(bestiary, rest)) {
                    printMonster(*m);
                    std::cout << "  (found in bestiary)\n";
                    hero.eventLog.push_front("search " + rest + " — found in bestiary");
                    continue;
                }
                if (const Item* it = findByName<Item>(hero.inventory, rest)) {
                    std::cout << "  " << it->name
                              << "  (wt " << it->weight
                              << ", val " << it->value << ")\n"
                              << "  (found in inventory)\n";
                    hero.eventLog.push_front("search " + rest + " — found in inventory");
                    continue;
                }
                auto eit = std::find_if(
                    hero.eventLog.begin(), hero.eventLog.end(),
                    [&rest](const std::string& s) { return s.find(rest) != std::string::npos; });
                if (eit != hero.eventLog.end()) {
                    std::cout << "  " << *eit << "\n"
                              << "  (found in event log)\n";
                    hero.eventLog.push_front("search " + rest + " — found in event log");
                    continue;
                }
                std::cout << "No such creature, item, or past event under that name.\n";
                hero.eventLog.push_front("search " + rest + " — not found");
            }
            else if (cmd == "inventory") {
                printInventory(hero);
                hero.eventLog.push_front(
                    "inventory — listed " + std::to_string(hero.inventory.size()) + " items");
            }
            else if (cmd == "inspect") {
                if (rest.empty()) {
                    std::cout << "Usage: inspect <n>\n";
                    continue;
                }
                std::size_t n;
                try { n = std::stoull(rest); }
                catch (...) {
                    std::cout << "Usage: inspect <n>\n";
                    continue;
                }
                if (n == 0) {
                    std::cout << "Items are numbered from 1, adventurer.\n";
                    continue;
                }
                const Item& it = hero.inventory.at(n - 1);
                std::cout << "  " << it.name
                          << "  (wt " << it.weight
                          << ", val " << it.value << ")\n";
                hero.eventLog.push_front("inspect " + rest + " — ok");
            }
            else if (cmd == "take") {
                if (rest.empty()) {
                    std::cout << "Usage: take <name>\n";
                    continue;
                }
                recordUndo(hero, "take " + rest);
                Item it{rest, 0.1, 1};
                hero.inventory.push_back(it);
                std::cout << "  Taken: " << rest << "  (wt 0.1, val 1)\n";
                hero.eventLog.push_front("take " + rest);
            }
            else if (cmd == "sort") {
                std::istringstream in(rest);
                std::string what, by, key, dir;
                in >> what >> by >> key >> dir;
                if (what != "inventory" || by != "by" || key.empty()) {
                    std::cout << "Usage: sort inventory by <name|weight|value> [asc|desc]\n";
                    continue;
                }
                std::string criterion = key;
                if (!dir.empty()) criterion += " " + dir;
                recordUndo(hero, "sort inventory by " + criterion);
                if (!sortInventory(hero, criterion)) {
                    hero.undoStack.pop();
                    std::cout << "Unknown sort key. Try: name, weight, or value.\n";
                    continue;
                }
                printInventory(hero);
                hero.eventLog.push_front("sort inventory by " + criterion);
            }
            else if (cmd == "undo") {
                // Floor 6 reference. The order matters: bind the reference
                // BEFORE pop(). Once you pop, the Action you were looking
                // at is gone.
                if (hero.undoStack.empty()) {
                    std::cout << "Nothing to undo.\n";
                } else {
                    const UndoAction& a = hero.undoStack.top();
                    hero.inventory = a.inventorySnapshot;
                    std::cout << "Undid: " << a.description << "\n";
                    hero.eventLog.push_front("undo (" + a.description + ")");
                    hero.undoStack.pop();
                }
            }
            else if (cmd == "lint") {
                if (rest.empty()) {
                    std::cout << "Usage: lint <text>\n";
                    continue;
                }
                const bool ok = isBalanced(rest);
                std::cout << "  " << (ok ? "balanced" : "not balanced") << "\n";
                hero.eventLog.push_front(std::string("lint — ") + (ok ? "balanced" : "not balanced"));
            }
            else if (cmd == "provoke") {
                if (rest.empty()) {
                    std::cout << "Usage: provoke <name>\n";
                    continue;
                }
                // Floor 7 wiring — enqueue a synthesized enemy action onto
                // hero.enemyActionQueue. Until your Queue<T>::enqueue actually
                // grows the queue (Monday), this is a no-op and `simulate`
                // will find the line empty.
                hero.enemyActionQueue.enqueue(rest + " stirs in the dark");
                std::cout << "  " << rest << " takes a number and joins the line.\n";
                hero.eventLog.push_front("provoke " + rest);
            }
            else if (cmd == "simulate") {
                // Drain the enemy line in FIFO order. Note the ordering:
                // bind `action` BEFORE calling dequeue() — the same hazard
                // as Floor 6's undo. dequeue() destroys the front node, so
                // anything you want to read FROM it must be read first.
                // Both ADTs share one rule: read before you remove.
                if (hero.enemyActionQueue.empty()) {
                    std::cout << "No enemies are in line. Use `provoke <name>` first.\n";
                } else {
                    std::size_t turn = 1;
                    while (!hero.enemyActionQueue.empty()) {
                        const std::string& action = hero.enemyActionQueue.front();
                        std::cout << "  Turn " << turn << ": " << action << ".\n";
                        hero.eventLog.push_front(
                            "simulate turn " + std::to_string(turn) + ": " + action);
                        hero.enemyActionQueue.dequeue();
                        ++turn;
                    }
                    std::cout << "All enemies have acted; the line is empty.\n";
                }
            }
            else if (cmd == "potato") {
                // Parse "<name> <name> ... <k>" — the last token is k, the
                // rest are names. The hotPotato body lives in Potato.cpp
                // and is Wednesday's TODO; until then it returns the
                // placeholder string.
                std::istringstream in(rest);
                std::vector<std::string> tokens;
                std::string tok;
                while (in >> tok) tokens.push_back(tok);
                if (tokens.size() < 2) {
                    std::cout << "Usage: potato <name> <name>... <k>\n";
                    continue;
                }
                std::size_t k = 0;
                try { k = std::stoull(tokens.back()); }
                catch (...) {
                    std::cout << "Last argument must be a positive integer k.\n";
                    continue;
                }
                if (k < 1) {
                    std::cout << "k must be at least 1.\n";
                    continue;
                }
                tokens.pop_back();
                const std::string survivor = hotPotato(tokens, k);
                std::cout << "  Survivor: " << survivor << "\n";
                hero.eventLog.push_front("potato — survivor " + survivor);
            }
            else if (cmd == "taxonomy") {
                // `taxonomy` prints the indented outline (provided printer).
                // `taxonomy --dfs` / `--bfs` print the flat visit order from
                // your preOrder() / levelOrder(). The contrast between the
                // two lists is the whole lesson: same tree, two orders.
                if (rest.empty()) {
                    printTaxonomy(taxonomy);
                } else if (rest == "--dfs") {
                    std::cout << "  depth-first (pre-order):\n";
                    for (const std::string& s : taxonomy.preOrder()) std::cout << "    " << s << "\n";
                } else if (rest == "--bfs") {
                    std::cout << "  breadth-first (level-order):\n";
                    for (const std::string& s : taxonomy.levelOrder()) std::cout << "    " << s << "\n";
                } else {
                    std::cout << "Usage: taxonomy [--dfs | --bfs]\n";
                    continue;
                }
                hero.eventLog.push_front("taxonomy" + (rest.empty() ? "" : " " + rest));
            }
            else if (cmd == "lineage") {
                if (rest.empty()) {
                    std::cout << "Usage: lineage <name>\n";
                    continue;
                }
                const std::vector<std::string> path = taxonomy.lineage(rest);
                if (path.empty()) {
                    std::cout << "  '" << rest << "' is not in McCown's catalogue.\n";
                    hero.eventLog.push_front("lineage " + rest + " — not classified");
                } else {
                    std::cout << "  ";
                    for (std::size_t i = 0; i < path.size(); ++i) {
                        std::cout << path[i];
                        if (i + 1 < path.size()) std::cout << " > ";
                    }
                    std::cout << "\n";
                    hero.eventLog.push_front("lineage " + rest + " — depth "
                                             + std::to_string(path.size() - 1));
                }
            }
            else if (cmd == "catalogue" || cmd == "catalog") {
                // In-order walk of the BST → alphabetical. The "free sort":
                // left < node < right means an in-order traversal emerges
                // sorted, with no sort step of its own.
                std::vector<Monster> ordered = bestiaryTree.inOrder();
                if (ordered.empty()) {
                    if (bestiaryTree.empty()) {
                        std::cout << "  (the tree is empty — is BST<T>::insert written yet?)\n";
                    } else {
                        std::cout << "  (" << bestiaryTree.size()
                                  << " creatures are in the tree, but inOrder() came back"
                                  << " empty — is inOrderInto written yet?)\n";
                    }
                } else {
                    for (const Monster& m : ordered) printMonster(m);
                    std::cout << "  (" << bestiaryTree.size()
                              << " creatures, alphabetical; tree height "
                              << bestiaryTree.height() << ")\n";
                }
                hero.eventLog.push_front("catalogue — bestiary tree printed");
            }
            else if (cmd == "recall") {
                if (rest.empty()) {
                    std::cout << "Usage: recall <name>\n";
                    continue;
                }
                // Build a probe with just the name set. The BST orders by
                // name, so find() descends to the match in O(log n) — one
                // comparison per level, not a scan of the whole catalogue.
                Monster probe;
                probe.name = rest;
                const auto* node = bestiaryTree.find(probe);
                if (node == nullptr) {
                    std::cout << "  '" << rest << "' is not in McCown's catalogue.\n";
                    hero.eventLog.push_front("recall " + rest + " — not found");
                } else {
                    printMonster(node->data);
                    std::cout << "  (recalled from the tree)\n";
                    hero.eventLog.push_front("recall " + rest + " — found");
                }
            }
            else if (cmd == "forget") {
                if (rest.empty()) {
                    std::cout << "Usage: forget <name>\n";
                    continue;
                }
                Monster probe;
                probe.name = rest;
                if (bestiaryTree.remove(probe)) {
                    std::cout << "  McCown strikes '" << rest << "' from the tree. ("
                              << bestiaryTree.size() << " remain.)\n";
                    hero.eventLog.push_front("forget " + rest + " — removed");
                } else {
                    std::cout << "  '" << rest << "' is not in the tree to forget.\n";
                    hero.eventLog.push_front("forget " + rest + " — not found");
                }
            }
            // ----- Floor 10: the Registry (std::map / std::set) commands -----
            // These query the Registry, a SEPARATE index from the BST above.
            // `lookup`/`strike` are the Registry's verbs; Floor 9's
            // `recall`/`forget` stay bound to the BST and are untouched.
            else if (cmd == "knows") {
                if (rest.empty()) {
                    std::cout << "Usage: knows <name>\n";
                    continue;
                }
                // Membership in one call — registry.knows is a single
                // std::map::count, not a scan of the catalogue.
                if (registry.knows(rest)) {
                    std::cout << "  McCown knows '" << rest << "'.\n";
                    hero.eventLog.push_front("knows " + rest + " — yes");
                } else {
                    std::cout << "  McCown does not know '" << rest << "'.\n";
                    hero.eventLog.push_front("knows " + rest + " — no");
                }
            }
            else if (cmd == "lookup") {
                if (rest.empty()) {
                    std::cout << "Usage: lookup <name>\n";
                    continue;
                }
                // One std::map::find descends to the record — the one-liner
                // that replaces Floor 9's whole recursive tree descent.
                const Monster* m = registry.recall(rest);
                if (m == nullptr) {
                    std::cout << "  '" << rest << "' is not in the Hall of Names.\n";
                    hero.eventLog.push_front("lookup " + rest + " — not found");
                } else {
                    printMonster(*m);
                    std::cout << "  (looked up in the Registry)\n";
                    hero.eventLog.push_front("lookup " + rest + " — found");
                }
            }
            else if (cmd == "strike") {
                if (rest.empty()) {
                    std::cout << "Usage: strike <name>\n";
                    continue;
                }
                // registry.forget erases from the map AND keeps the derived
                // weakness set/counts consistent. It does NOT touch the Bag or
                // the BST — the three views may legitimately diverge after this.
                if (registry.forget(rest)) {
                    std::cout << "  Struck '" << rest << "' from the Hall of Names. ("
                              << registry.size() << " remain.)\n";
                    hero.eventLog.push_front("strike " + rest + " — removed");
                } else {
                    std::cout << "  '" << rest << "' is not in the Hall of Names to strike.\n";
                    hero.eventLog.push_front("strike " + rest + " — not found");
                }
            }
            else if (cmd == "names") {
                // Iterate the std::map in name order. Each element is a
                // std::pair<const std::string, Monster>; the structured binding
                // [name, m] splits it. The key half is const — you cannot
                // reassign a key in place without breaking the map's ordering.
                const auto& byName = registry.byName();
                if (byName.empty()) {
                    std::cout << "  (the Hall is empty — is Registry::index written yet?)\n";
                } else {
                    for (const auto& [name, m] : byName) {
                        (void)name;          // name == m.name here; print the record
                        printMonster(m);
                    }
                    std::cout << "  (" << byName.size()
                              << " names, alphabetical — std::map keeps them ordered)\n";
                }
                hero.eventLog.push_front("names — Registry printed");
            }
            else if (cmd == "weaknesses") {
                // The std::set of DISTINCT weaknesses, already sorted (a set is
                // a balanced tree). For each, the count comes from the map.
                const auto& weak = registry.weaknesses();
                if (weak.empty()) {
                    std::cout << "  (no weaknesses indexed — is Registry::index written yet?)\n";
                } else {
                    for (const std::string& w : weak) {
                        std::cout << "  " << w << "  ("
                                  << registry.countWeakness(w) << " monsters)\n";
                    }
                    std::cout << "  (" << weak.size() << " distinct weaknesses)\n";
                }
                hero.eventLog.push_front("weaknesses — listed");
            }
            else if (cmd == "weakness") {
                if (rest.empty()) {
                    std::cout << "Usage: weakness <w>\n";
                    continue;
                }
                // Membership against the SET, count from the MAP.
                if (registry.hasWeakness(rest)) {
                    std::cout << "  " << registry.countWeakness(rest)
                              << " monsters fear " << rest << ".\n";
                    hero.eventLog.push_front("weakness " + rest + " — present");
                } else {
                    std::cout << "  Nothing in the bestiary fears " << rest << ".\n";
                    hero.eventLog.push_front("weakness " + rest + " — absent");
                }
            }
            // ----- Floor 11: the Hall of Whispers (HashTable) commands -------
            // These query `whispers`, a SEPARATE index from the Bag, the BST,
            // and the Registry. `hash`/`buckets`/`whisper`/`banish`/`load` are
            // the HashTable's verbs; nothing earlier uses them.
            else if (cmd == "hash") {
                if (rest.empty()) {
                    std::cout << "Usage: hash <name>\n";
                    continue;
                }
                // The debug verb: show the raw hash CODE and the bucket it
                // reduces to, so "compute where the name lives" is visible. On
                // the stub these are 0 / bucket 0 of 8 until hashKey/bucketIndex
                // are written.
                std::cout << "  hash('" << rest << "') = " << whispers.hashKey(rest)
                          << "  ->  bucket " << whispers.bucketIndex(rest)
                          << " of " << whispers.bucketCount() << "\n";
                hero.eventLog.push_front("hash " + rest);
            }
            else if (cmd == "buckets") {
                // Walk the bucket array and print each non-empty bucket and the
                // names in its chain. THIS is the collision picture: some buckets
                // hold two or more names, some are empty, and the order is
                // SCRAMBLED — not alphabetical, the price of the O(1) whisper.
                const auto& bs = whispers.buckets();
                if (whispers.empty()) {
                    std::cout << "  (the Hall is silent — is HashTable::insert written yet?)\n";
                } else {
                    for (std::size_t i = 0; i < bs.size(); ++i) {
                        if (bs[i].empty()) continue;
                        std::cout << "  [" << i << "] ";
                        bool first = true;
                        for (const Monster& m : bs[i]) {
                            if (!first) std::cout << ", ";
                            std::cout << m.name;
                            first = false;
                        }
                        std::cout << "\n";
                    }
                    std::cout << "  (" << whispers.size() << " monsters across "
                              << whispers.bucketCount() << " buckets, load factor "
                              << std::fixed << std::setprecision(3) << whispers.loadFactor()
                              << ")\n";
                }
                hero.eventLog.push_front("buckets — hash table printed");
            }
            else if (cmd == "whisper") {
                if (rest.empty()) {
                    std::cout << "Usage: whisper <name>\n";
                    continue;
                }
                // One bucketIndex + a scan of ONE short chain — O(1) average,
                // not the whole-table scan of Floor 1's `search`.
                const Monster* m = whispers.find(rest);
                if (m == nullptr) {
                    std::cout << "  The Hall of Whispers has no answer for '" << rest << "'.\n";
                    hero.eventLog.push_front("whisper " + rest + " — no answer");
                } else {
                    printMonster(*m);
                    std::cout << "  (whispered in the Hall — found in one bucket)\n";
                    hero.eventLog.push_front("whisper " + rest + " — answered");
                }
            }
            else if (cmd == "banish") {
                if (rest.empty()) {
                    std::cout << "Usage: banish <name>\n";
                    continue;
                }
                // whispers.erase removes only the matching item from its bucket's
                // chain. It does NOT touch the Bag, the BST, or the Registry —
                // the four views may legitimately diverge after this.
                if (whispers.erase(rest)) {
                    std::cout << "  Banished '" << rest << "' from the Hall of Whispers. ("
                              << whispers.size() << " remain.)\n";
                    hero.eventLog.push_front("banish " + rest + " — removed");
                } else {
                    std::cout << "  '" << rest << "' is not in the Hall of Whispers to banish.\n";
                    hero.eventLog.push_front("banish " + rest + " — not found");
                }
            }
            else if (cmd == "load") {
                // The §12.4 numbers made visible: items, buckets, and their
                // ratio. This is where you watch rehash double the bucket count
                // (force growth by inserting past the threshold).
                std::cout << "  count " << whispers.size()
                          << ", buckets " << whispers.bucketCount()
                          << ", load factor " << std::fixed << std::setprecision(3)
                          << whispers.loadFactor() << "\n";
                hero.eventLog.push_front("load — hash table stats");
            }
            // ----- Floor 12: the Labyrinth (Graph) commands ------------------
            // These query `labyrinth`, a SEPARATE index from the Bag, the BST,
            // the Registry, and the HashTable — and the first one built from its
            // OWN data file, not the bestiary. `map`/`doors`/`reach`/`cross`/
            // `descend` are the Graph's verbs; nothing earlier uses them.
            else if (cmd == "map") {
                // Walk the adjacency list and print each room and the doors out
                // of it. THIS is the §13.3 adjacency-list representation made
                // visible: a room, then the rooms one passage away.
                const auto& adj = labyrinth.adjacency();
                if (labyrinth.empty()) {
                    std::cout << "  (the Labyrinth is dark — is Graph::connect written yet?)\n";
                } else {
                    std::size_t passages = 0;   // each undirected edge counted once
                    for (const auto& [room, doors] : adj) {
                        passages += doors.size();
                        std::cout << "  " << room << " -> ";
                        if (doors.empty()) {
                            std::cout << "(no doors)";
                        } else {
                            for (std::size_t i = 0; i < doors.size(); ++i) {
                                if (i) std::cout << ", ";
                                std::cout << doors[i];
                            }
                        }
                        std::cout << "\n";
                    }
                    std::cout << "  (" << labyrinth.roomCount() << " rooms, "
                              << (passages / 2) << " passages)\n";
                }
                hero.eventLog.push_front("map — Labyrinth printed");
            }
            else if (cmd == "doors") {
                if (rest.empty()) {
                    std::cout << "Usage: doors <room>\n";
                    continue;
                }
                // The rooms one passage away — a single neighbors() call.
                const std::vector<std::string> doors = labyrinth.neighbors(rest);
                if (doors.empty()) {
                    std::cout << "  " << rest << " has no doors (or is not on the map).\n";
                    hero.eventLog.push_front("doors " + rest + " — none");
                } else {
                    std::cout << "  From " << rest << ": ";
                    for (std::size_t i = 0; i < doors.size(); ++i) {
                        if (i) std::cout << ", ";
                        std::cout << doors[i];
                    }
                    std::cout << "\n";
                    hero.eventLog.push_front("doors " + rest + " — "
                                             + std::to_string(doors.size()));
                }
            }
            else if (cmd == "reach") {
                // reach <from> <to> — is there ANY path? (BFS over the Queue.)
                std::istringstream in(rest);
                std::string from, to;
                in >> from >> to;
                if (from.empty() || to.empty()) {
                    std::cout << "Usage: reach <from> <to>\n";
                    continue;
                }
                if (!labyrinth.hasRoom(from) || !labyrinth.hasRoom(to)) {
                    std::cout << "  " << (labyrinth.hasRoom(from) ? to : from)
                              << " is not on the map.\n";
                    hero.eventLog.push_front("reach " + from + " " + to + " — off map");
                    continue;
                }
                if (labyrinth.reachable(from, to)) {
                    std::cout << "  Yes — a path runs from " << from << " to " << to << ".\n";
                    hero.eventLog.push_front("reach " + from + " " + to + " — yes");
                } else {
                    std::cout << "  No path from " << from << " to " << to << ".\n";
                    hero.eventLog.push_front("reach " + from + " " + to + " — no");
                }
            }
            else if (cmd == "cross") {
                // cross <start> --bfs|--dfs — walk the whole map from <start> in
                // breadth-first or depth-first order. Same map, two orders: the
                // contrast IS the lesson (the Queue's FIFO vs the Stack's LIFO,
                // exactly like Floors 6–7). Mirrors the `taxonomy --dfs/--bfs`
                // flag-parse already used above.
                std::istringstream in(rest);
                std::string start, flag;
                in >> start >> flag;
                if (start.empty() || (flag != "--bfs" && flag != "--dfs")) {
                    std::cout << "Usage: cross <start> --bfs | --dfs\n";
                    continue;
                }
                if (!labyrinth.hasRoom(start)) {
                    std::cout << "  " << start << " is not on the map.\n";
                    hero.eventLog.push_front("cross " + start + " " + flag + " — off map");
                    continue;
                }
                const bool bfs = (flag == "--bfs");
                const std::vector<std::string> order =
                    bfs ? labyrinth.bfsOrder(start) : labyrinth.dfsOrder(start);
                std::cout << "  " << (bfs ? "breadth-first" : "depth-first")
                          << " from " << start << ":\n";
                for (const std::string& room : order) std::cout << "    " << room << "\n";
                std::cout << "  (" << order.size() << " rooms reached, "
                          << (bfs ? "level by level" : "deepest-first") << ")\n";
                hero.eventLog.push_front("cross " + start + " " + flag + " — "
                                         + std::to_string(order.size()) + " rooms");
            }
            else if (cmd == "descend") {
                // descend <from> <to> — the SHORTEST way down, in doors. Rides
                // the provided hops() (a BFS that counts distance), which in turn
                // rides your neighbors()/BFS. Pays off "the shortest way down"
                // from the Floor 11 cliffhanger. Off-by-one: same room = 0 doors,
                // an adjacent room = 1.
                std::istringstream in(rest);
                std::string from, to;
                in >> from >> to;
                if (from.empty() || to.empty()) {
                    std::cout << "Usage: descend <from> <to>\n";
                    continue;
                }
                if (!labyrinth.hasRoom(from) || !labyrinth.hasRoom(to)) {
                    std::cout << "  " << (labyrinth.hasRoom(from) ? to : from)
                              << " is not on the map.\n";
                    hero.eventLog.push_front("descend " + from + " " + to + " — off map");
                    continue;
                }
                const int n = labyrinth.hops(from, to);
                if (n < 0) {
                    std::cout << "  There is no way down from " << from
                              << " to " << to << ".\n";
                    hero.eventLog.push_front("descend " + from + " " + to + " — no way");
                } else {
                    std::cout << "  The shortest way from " << from << " to " << to
                              << " is " << n << " door" << (n == 1 ? "" : "s") << ".\n";
                    hero.eventLog.push_front("descend " + from + " " + to + " — "
                                             + std::to_string(n) + " doors");
                }
            }
            else if (cmd == "log") {
                bool oldestFirst = false;
                std::size_t n = 10;
                std::istringstream in(rest);
                std::string tok;
                bool bad = false;
                while (in >> tok) {
                    if (tok == "--oldest") {
                        oldestFirst = true;
                    } else {
                        try { n = std::stoull(tok); }
                        catch (...) { bad = true; break; }
                    }
                }
                if (bad) {
                    std::cout << "Usage: log [--oldest] [n]\n";
                    continue;
                }
                if (oldestFirst) {
                    printLog(hero.eventLog.rbegin(), hero.eventLog.rend(),
                             n, "oldest first", hero.eventLog.size());
                } else {
                    printLog(hero.eventLog.begin(),  hero.eventLog.end(),
                             n, "newest first", hero.eventLog.size());
                }
            }
            else if (cmd == "clone") {
                if (rest != "hero") {
                    std::cout << "Usage: clone hero\n";
                    continue;
                }
                std::cout << "  -- original log (newest first) --\n";
                printLog(hero.eventLog.begin(), hero.eventLog.end(),
                         5, "newest first", hero.eventLog.size());
                {
                    Hero copy = hero;
                    std::cout << "  -- cloned log (newest first) --\n";
                    printLog(copy.eventLog.begin(), copy.eventLog.end(),
                             5, "newest first", copy.eventLog.size());
                    std::cout << "  (clone is being destroyed now)\n";
                }
                std::cout << "  (clone destroyed; original event log still has "
                          << hero.eventLog.size() << " entries — try `log 3`)\n";
                hero.eventLog.push_front("clone hero — copy lived and died");
            }
            else if (cmd == "balance") {
                // Floor 9½: is the bestiary's BST balanced? Reports its height
                // and the root's balance factor. Built from monsters.txt in
                // file order, it is bushy-ish — but `benchmark balance` shows
                // what a SORTED insert would have done.
                const std::size_t n = bestiaryTree.size();
                std::cout << "  McCown's catalogue: " << n << " names, height "
                          << bestiaryTree.height()
                          << ", root balance factor " << bestiaryTree.balanceFactor()
                          << (bestiaryTree.isBalanced()
                                  ? " — balanced.\n"
                                  : " — NOT balanced (lopsided).\n");
                hero.eventLog.push_front("balance — checked the catalogue tree");
            }
            else if (cmd == "selftest") {
                if (rest == "chain") {
                    runChainSelfTest();
                } else if (rest == "iterator") {
                    runIteratorSelfTest();
                } else if (rest == "stack") {
                    runStackSelfTest();
                } else if (rest == "queue") {
                    runQueueSelfTest();
                } else if (rest == "tree") {
                    runTreeSelfTest();
                } else if (rest == "bst") {
                    runBSTSelfTest();
                } else if (rest == "balance") {
                    runBalanceSelfTest();
                } else if (rest == "registry") {
                    runRegistrySelfTest();
                } else if (rest == "whispers") {
                    runHashTableSelfTest();
                } else if (rest == "crossing") {
                    runGraphSelfTest();
                } else {
                    std::cout << "Usage: selftest chain | iterator | stack | queue | tree | bst | balance | registry | whispers | crossing\n";
                }
            }
            else if (cmd == "benchmark") {
                std::istringstream in(rest);
                std::string first;
                in >> first;
                if (first == "sort") {
                    SortBenchOptions opts;
                    std::size_t n = 0;
                    std::string tok;
                    while (in >> tok) {
                        if      (tok == "--sorted")    opts.presorted = true;
                        else if (tok == "--bad-pivot") opts.badPivot  = true;
                        else {
                            try { n = std::stoull(tok); }
                            catch (...) {
                                std::cout << "Usage: benchmark sort [N] [--sorted] [--bad-pivot]\n";
                                n = static_cast<std::size_t>(-1);
                                break;
                            }
                        }
                    }
                    if (n == static_cast<std::size_t>(-1)) continue;
                    if (n == 0) runSortBenchmarkSweep(opts);
                    else        runSortBenchmark(n, opts);
                }
                else if (first == "log") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark log [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runChainBenchmarkSweep();
                    else        runChainBenchmark(n);
                }
                else if (first == "bst") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark bst [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runBSTBenchmarkSweep();
                    else        runBSTBenchmark(n);
                }
                else if (first == "balance") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark balance [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runBalanceBenchmarkSweep();
                    else        runBalanceBenchmark(n);
                }
                else if (first == "names") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark names [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runNamesBenchmarkSweep();
                    else        runNamesBenchmark(n);
                }
                else if (first == "whispers") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark whispers [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runWhispersBenchmarkSweep();
                    else        runWhispersBenchmark(n);
                }
                else if (first == "crossing") {
                    std::size_t n = 0;
                    std::string tok;
                    if (in >> tok) {
                        try { n = std::stoull(tok); }
                        catch (...) {
                            std::cout << "Usage: benchmark crossing [N]\n";
                            continue;
                        }
                    }
                    if (n == 0) runCrossingBenchmarkSweep();
                    else        runCrossingBenchmark(n);
                }
                else if (first.empty()) {
                    runBenchmarkSweep();
                }
                else {
                    try {
                        runBenchmark(std::stoull(first));
                    } catch (const std::exception&) {
                        std::cout << "Usage: benchmark [N]   or   benchmark sort [N] [flags]\n";
                    }
                }
            }
            else if (cmd == "battle") {
                if (rest != "warden") {
                    std::cout << "Usage: battle warden\n";
                    continue;
                }
                BattleOutcome outcome = runWardenBattle(hero);
                switch (outcome) {
                    case BattleOutcome::Victory:
                        std::cout << "\nThe Warden falls. The gate opens.\n"; break;
                    case BattleOutcome::Defeat:
                        std::cout << "\nYou fall. The gate stays shut.\n"; break;
                    case BattleOutcome::Fled:
                        std::cout << "\nYou step back from the gate. It does not chase.\n"; break;
                }
            }
            else {
                std::cout << "The Bard does not know the word '" << cmd << "'. Try `help`.\n";
            }
        }
        catch (const BagException& e) {
            std::cout << "No such item. (" << e.what() << ")\n";
            hero.eventLog.push_front(std::string("error: ") + e.what());
        }
        catch (const std::exception& e) {
            std::cout << "Something went wrong: " << e.what() << "\n";
            hero.eventLog.push_front(std::string("error: ") + e.what());
        }
    }

    return 0;
}
