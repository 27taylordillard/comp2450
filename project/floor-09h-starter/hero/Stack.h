// COMP 2450 — Floor 7 starter
// hero/Stack.h — LIFO stack template, complete reference (your Floor 6 work).
//
// As-of state at the start of Floor 7: Stack<T> is a thin adapter over
// Chain<T>. Five one-line method bodies. You wrote this last week.
//
// You will not edit this file on Floor 7. It is here both as a working
// reference and so the new Queue<T> can sit alongside it for direct
// comparison — same pattern, different end-pairing.
//
// =====================================================================
// One thing to notice before you move on:
//
// `push` maps to `chain_.push_front`.
// `pop`  maps to `chain_.pop_front`.
// `top`  maps to `chain_.head()->data`.
//
// All three operations touch the SAME END of the chain. That is the
// LIFO contract in code form: the most recently inserted element is
// the one returned and removed next.
//
// In Queue<T> this week, you will keep insert and remove on OPPOSITE
// ends — that one change is the difference between a stack and a queue.

#pragma once

#include <cstddef>

#include "Chain.h"

namespace dungeon {

template <typename T>
class Stack {
public:
    using value_type = T;

    Stack()                              = default;
    ~Stack()                             = default;
    Stack(const Stack& other)            = default;
    Stack& operator=(const Stack& other) = default;

    void push(const T& value) { chain_.push_front(value); }
    void pop()                { chain_.pop_front(); }

    T&       top()       { return chain_.head()->data; }
    const T& top() const { return chain_.head()->data; }

    std::size_t size()  const { return chain_.size(); }
    bool        empty() const { return size() == 0; }

private:
    Chain<T> chain_;
};

}  // namespace dungeon
