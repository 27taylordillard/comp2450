// COMP 2450 — Floor 8 starter
// hero/Queue.h — FIFO queue template, complete reference (your Floor 7 work).
//
// As-of state from Floor 7 onward: Queue<T> is a thin adapter over Chain<T>,
// exactly like Stack<T> — a handful of one-line method bodies. The one
// difference that makes it a QUEUE and not a stack: insert and remove happen
// at OPPOSITE ends of the chain.
//
// You will not edit this file on later floors. It is here as working
// reference — Floor 8's levelOrder (BFS) and this floor's demos reuse it.
//
// =====================================================================
// Why this is FIFO:
//
// `enqueue` maps to `chain_.push_back`     — new items join at the BACK.
// `dequeue` maps to `chain_.pop_front`     — the OLDEST item leaves the FRONT.
// `front`   maps to `chain_.head()->data`  — peek the oldest item.
//
// enqueue and dequeue touch OPPOSITE ends. Contrast Stack<T>, where push
// and pop touch the same end. The chain's geography never changed; only
// which end each operation uses — and that asymmetry is the whole contract.

#pragma once

#include <cstddef>

#include "Chain.h"

namespace dungeon {

template <typename T>
class Queue {
public:
    using value_type = T;

    Queue()                              = default;
    ~Queue()                             = default;
    Queue(const Queue& other)            = default;
    Queue& operator=(const Queue& other) = default;

    // enqueue at the back; the oldest item drifts toward the front.
    void enqueue(const T& value) { chain_.push_back(value); }

    // dequeue from the front — the opposite end from enqueue. This single
    // asymmetry is what makes the data structure FIFO.
    void dequeue() { chain_.pop_front(); }

    // front is the OLDEST element still in the queue. Same physical end as
    // Stack<T>::top (head_), but here it is the oldest element, because we
    // enqueue at the tail.
    T&       front()       { return chain_.head()->data; }
    const T& front() const { return chain_.head()->data; }

    std::size_t size()  const { return chain_.size(); }
    bool        empty() const { return size() == 0; }

private:
    Chain<T> chain_;
};

}  // namespace dungeon
