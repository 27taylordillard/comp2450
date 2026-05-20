// COMP 2450 — Floor 7 starter
// hero/Lint.cpp — bracket balancer, complete reference (your Floor 6 work).
//
// Classic stack-balancing algorithm over Stack<char>. Reference code as
// of Floor 7; you will not edit this file. It is preserved so the `lint`
// command still works and so this week's Potato.cpp has a near-twin to
// compare itself against (one classical Stack use; one classical Queue use).

#include "Lint.h"

#include "Stack.h"

namespace dungeon {

bool isBalanced(const std::string& input) {
    Stack<char> s;
    for (char c : input) {
        switch (c) {
            case '(': case '[': case '{':
                s.push(c);
                break;
            case ')':
                if (s.empty() || s.top() != '(') return false;
                s.pop();
                break;
            case ']':
                if (s.empty() || s.top() != '[') return false;
                s.pop();
                break;
            case '}':
                if (s.empty() || s.top() != '{') return false;
                s.pop();
                break;
            default:
                break;
        }
    }
    return s.empty();
}

}  // namespace dungeon
