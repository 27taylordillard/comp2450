// COMP 2450 — Floor 6 starter
// hero/Stack.h — LIFO stack template. THE WORK IS HERE.
//
// One header, five one-line method bodies, one day:
//   Monday — push, pop, top, size, empty.
//
// This week's lesson, on one slide: a stack is NOT a data structure.
// It is an *ADT* — a contract that says "the last thing pushed is the
// next thing returned." Many data structures can implement that
// contract. We pick one (your Floor-4½ Chain<T>) and we ADAPT it —
// five trivial delegates is the entire implementation.
//
// `std::stack<T>` is exactly this pattern: by default it adapts
// `std::deque<T>`. We're recreating the standard library's own move.
//
//
// =====================================================================
// Why a Chain<T> and not a Bag<T>?
// =====================================================================
// Either would WORK (both have push_back / pop_back). The Chain<T>
// choice is deliberate because Chain<T>::push_front and pop_front are
// both O(1), and our Stack<T>'s top will be the *front* of the chain.
// On a Bag<T> (which wraps std::vector), pop_back is O(1) but
// pop_front would be O(n). Choosing the right underlying container is
// the entire point of an adapter; we want O(1) push and pop on both
// ends, but specifically on the SAME end. front-of-Chain wins.
//
// =====================================================================
// Why the bodies are TODOs even though they are one line each.
// =====================================================================
// Brevity IS the lesson. Five trivial bodies look like nothing, but
// the choice you make — which Chain operation maps to which Stack
// operation — determines whether your Stack is actually LIFO. If you
// pick push_back here and pop_front for pop(), you have built a queue
// instead. Reading the test failures is how you find out.

#pragma once

#include <cstddef>

#include "Chain.h"

namespace dungeon {

template <typename T>
class Stack {
public:
    using value_type = T;

    // Default ctor, dtor, copy, and assign are all fine: Chain<T> has
    // a working Rule of Three (Floor 4½). Stack just inherits that
    // behavior transitively.
    Stack()                              = default;
    ~Stack()                             = default;
    Stack(const Stack& other)            = default;
    Stack& operator=(const Stack& other) = default;

    // ----- THE FIVE THINGS YOU WRITE THIS WEEK -----------------------

    // Push `value` onto the top of the stack. O(1).
    //
    // TODO Floor 6 (Monday). Body is one line.
    //     chain_.push_front(value);
    void push(const T& value) {
        (void)value;   // silence unused-parameter warning while stubbed
        // TODO Monday
    }

    // Remove the top of the stack. O(1). No-op on empty (matches std::stack
    // behavior — calling pop on an empty stack is undefined, but our
    // Chain::pop_front is already a safe no-op on empty).
    //
    // TODO Floor 6 (Monday). Body is one line.
    //     chain_.pop_front();
    void pop() {
        // TODO Monday
    }

    // Read the top of the stack. O(1). Stubbed bodies return through
    // chain_.head()->data — that's already the right implementation;
    // it just relies on push() actually having put something there.
    //
    // TODO Floor 6 (Monday). Body is one line each — and the same line.
    //     return chain_.head()->data;
    T& top() {
        // TODO Monday — this stub will dereference nullptr if push() is
        // not implemented yet. The selftest stack harness checks empty()
        // first to avoid that, so the build stays runnable.
        return chain_.head()->data;
    }
    const T& top() const {
        // TODO Monday — same body as the non-const version above.
        return chain_.head()->data;
    }

    // Number of items currently on the stack.
    //
    // TODO Floor 6 (Monday). Body is one line.
    //     return chain_.size();
    std::size_t size() const {
        return 0;   // TODO Monday
    }

    // True iff size() == 0.
    //
    // TODO Floor 6 (Monday). Body is one line — and you don't even need
    // to touch chain_; you can delegate to your own size().
    //     return size() == 0;
    bool empty() const {
        return true;   // TODO Monday
    }

private:
    Chain<T> chain_;
};

}  // namespace dungeon
