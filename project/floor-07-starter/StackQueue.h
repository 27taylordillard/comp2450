
#pragma once

#include <cstddef>
#include "hero/Stack.h"

namespace dungeon {

template <typename T>
class StackQueue {
public:
    using value_type = T;

    StackQueue() = default;

    void enqueue(const T& value) {
        ins_.push(value);
    }

    void dequeue() {
        moveIfNeeded();
        if (!outs_.empty()) {
            outs_.pop();
        }
    }

    T& front() {
        moveIfNeeded();
        return outs_.top();
    }

    const T& front() const {
        moveIfNeeded();
        return outs_.top();
    }

    std::size_t size() const {
        return ins_.size() + outs_.size();
    }

    bool empty() const {
        return ins_.empty() && outs_.empty();
    }

private:
    mutable Stack<T> ins_;
    mutable Stack<T> outs_;

    void moveIfNeeded() const {
        if (outs_.empty()) {
            while (!ins_.empty()) {
                outs_.push(ins_.top());
                ins_.pop();
            }
        }
    }
};

}

