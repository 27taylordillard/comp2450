// COMP 2450 — Floor 7 starter
// hero/Queue.h — FIFO queue template. THE WORK IS HERE.
//
// One header, five one-line method bodies, one day:
//   Monday — enqueue, dequeue, front, size, empty.
//
// This week's lesson, on one slide: a queue is a stack with the OPPOSITE
// end-pairing. Last week your Stack<T>'s push and pop touched the SAME
// end of chain_ (push_front + pop_front). This week your Queue<T>'s
// enqueue and dequeue touch OPPOSITE ends (push_back to add, pop_front
// to remove). One word changed; the contract flipped from LIFO to FIFO.
//
// `std::queue<T>` is the same adapter pattern — by default it wraps
// std::deque<T>. We are recreating the standard library's own move,
// for the second week in a row, on top of the same Chain<T> we wrote
// in Floor 4½. The pattern outlives the example.
//
// =====================================================================
// Why a Chain<T>, again?
// =====================================================================
// We need O(1) insert at one end and O(1) remove at the OTHER end.
// Our doubly-linked Chain<T> has both push_back (O(1) at the tail) and
// pop_front (O(1) at the head). A Bag<T> (which wraps std::vector)
// would make push_back O(1) but pop_front O(n) — every dequeue would
// shift the rest of the array down. That is why std::queue picks
// std::deque underneath, not std::vector. Pick the storage to match
// the access pattern, every time.
//
// =====================================================================
// Why the bodies are TODOs even though they are one line each.
// =====================================================================
// Brevity IS the lesson. Five trivial bodies look like nothing, but
// the choice — which end of chain_ becomes the front of the queue —
// determines whether your Queue is actually FIFO. If you pick
// push_front for enqueue, you have built a stack instead. The
// selftest queue harness asks "is the first thing in also the first
// thing out?" and the answer must be yes.

#pragma once

#include <cstddef>

#include "Chain.h"

namespace dungeon {

template <typename T>
class Queue {
public:
    using value_type = T;

    // Default ctor, dtor, copy, and assign are all fine: Chain<T> has
    // a working Rule of Three (Floor 4½). Queue just inherits that
    // behavior transitively — same as Stack last week.
    Queue()                              = default;
    ~Queue()                             = default;
    Queue(const Queue& other)            = default;
    Queue& operator=(const Queue& other) = default;

    // ----- THE FIVE THINGS YOU WRITE THIS WEEK -----------------------

    // Enqueue `value` at the back of the queue. O(1).
    //
    // TODO Floor 7 (Monday). Body is one line.
    //     chain_.push_back(value);
    void enqueue(const T& value) {
        (void)value;   // silence unused-parameter warning while stubbed
        // TODO Monday
    }

    // Remove the front of the queue. O(1). No-op on empty (Chain's
    // pop_front is already a safe no-op on empty).
    //
    // TODO Floor 7 (Monday). Body is one line.
    //     chain_.pop_front();
    void dequeue() {
        // TODO Monday
    }

    // Read the front of the queue (the oldest enqueued item). O(1).
    // Stubbed bodies already return through chain_.head()->data; that
    // is the right body, it just relies on enqueue having put something
    // there. The selftest queue harness guards empty() first so this
    // does not crash on Monday morning.
    //
    // TODO Floor 7 (Monday). Body is one line each — and the same line.
    //     return chain_.head()->data;
    T& front() {
        return chain_.head()->data;
    }
    const T& front() const {
        return chain_.head()->data;
    }

    // Number of items currently in the queue.
    //
    // TODO Floor 7 (Monday). Body is one line.
    //     return chain_.size();
    std::size_t size() const {
        return 0;   // TODO Monday
    }

    // True iff size() == 0.
    //
    // TODO Floor 7 (Monday). Body is one line — delegate to size() or
    // to chain_.empty(), your choice.
    //     return size() == 0;
    bool empty() const {
        return true;   // TODO Monday
    }

private:
    Chain<T> chain_;
};

}  // namespace dungeon
