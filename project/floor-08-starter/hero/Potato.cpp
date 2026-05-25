// COMP 2450 — Floor 7 INSTRUCTOR SOLUTION
// hero/Potato.cpp — hotPotato body, complete reference.
//
// Drop-in replacement for the starter's stubbed Potato.cpp.
//
// INSTRUCTOR: Before writing the function, do the algorithm on the board
// with names = {A,B,C,D,E} and k = 3. Slot by slot, who moves to the
// back? Who gets dequeued? By the time you have eliminated three names,
// the algorithm is already in everyone's head — the function is just
// the formal write-down. Then run `potato A B C D E 3` and confirm the
// survivor is D.

#include "Potato.h"

#include "Queue.h"

namespace dungeon {

std::string hotPotato(const std::vector<std::string>& names, std::size_t k) {
    // INSTRUCTOR: the empty-input guard is the off-by-one prevention.
    // Without it, the size() > 1 loop is fine on empty (never enters),
    // but the final front() dereferences head_ == nullptr → segfault.
    if (names.empty()) return "";

    Queue<std::string> q;
    for (const std::string& n : names) q.enqueue(n);

    // INSTRUCTOR: the outer loop runs until ONE survivor remains.
    // Inside: k-1 rotations move "safe" people to the back, then the
    // unlucky one at the front is dequeued. For k = 1 this is zero
    // rotations: each round just drops the front. For k = names.size()
    // (large k), most of every round is rotation.
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
