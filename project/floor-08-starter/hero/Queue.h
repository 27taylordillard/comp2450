// COMP 2450 — Floor 7 INSTRUCTOR SOLUTION
// hero/Queue.h — FIFO queue template, complete reference.
//
// Drop-in replacement for the starter's stubbed Queue.h. Five method
// bodies, one line each. Lines tagged `INSTRUCTOR:` are notes to you,
// NOT for live-typing.
//
// INSTRUCTOR: Open this alongside the starter on the projector. Reveal
// one method body at a time. On enqueue, ask the class "which `Chain<T>`
// method moves an item to the BACK of the line?" before showing
// `push_back`. On dequeue, ask "which one removes from the FRONT?"
// before showing `pop_front`. The point is that the class hears
// themselves narrate FIFO before they see the code.

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

    // INSTRUCTOR: enqueue at the back; oldest item drifts toward front.
    void enqueue(const T& value) { chain_.push_back(value); }

    // INSTRUCTOR: dequeue from the front; opposite end from enqueue.
    // This single asymmetry is what makes the data structure FIFO.
    void dequeue() { chain_.pop_front(); }

    // INSTRUCTOR: front is the OLDEST element still in the queue.
    // Contrast with Stack<T>::top — same physical end (head_), but
    // here it is the *oldest* element because we enqueue at the
    // tail. The chain's geography did not change; the contract did.
    T&       front()       { return chain_.head()->data; }
    const T& front() const { return chain_.head()->data; }

    std::size_t size()  const { return chain_.size(); }
    bool        empty() const { return size() == 0; }

private:
    Chain<T> chain_;
};

}  // namespace dungeon
