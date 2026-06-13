// COMP 2450 — Floor 8 starter
// hero/Potato.cpp — hotPotato body, complete reference (your Floor 7 work).
//
// The classic Josephus-style elimination: people stand in a circle and
// count off by k; the k-th is out; repeat until one survivor remains. A
// Queue<T> models the circle — rotate k-1 people from the front to the back
// (they're safe this round), then dequeue the unlucky one now at the front.

#include "Potato.h"

#include "Queue.h"

namespace dungeon {

std::string hotPotato(const std::vector<std::string>& names, std::size_t k) {
    // The empty-input guard prevents an off-by-one crash: without it the
    // size() > 1 loop is simply skipped, but the final front() would
    // dereference head_ == nullptr.
    if (names.empty()) return "";

    Queue<std::string> q;
    for (const std::string& n : names) q.enqueue(n);

    // Run until ONE survivor remains. Each round: k-1 rotations move "safe"
    // people to the back, then the front person is dequeued (eliminated).
    // For k == 1 there are zero rotations — each round just drops the front.
    // For large k, most of each round is rotation.
    while (q.size() > 1) {
        for (std::size_t i = 0; i < k - 1; ++i) {
            q.enqueue(q.front());
            q.dequeue();
        }
        q.dequeue();
    }
    return q.front();
}

}  // namespace dungeon
