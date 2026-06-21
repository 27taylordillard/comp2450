// COMP 2450 — Floor 6 starter
// hero/StackTests.cpp — the `selftest stack` harness. Provided. You do not edit this.
//
// Runs five phases against Stack<int>. Each phase prints OK or FAIL with
// a one-line diagnostic that names the likely culprit (which Stack method
// is still stubbed or wired wrong).
//
//   1. empty stack invariants            — needs size(), empty()
//   2. single push                       — needs push() then size()/top()
//   3. LIFO ordering on three pushes     — needs push() AND top() returning last-pushed
//   4. pop reveals previous top          — needs pop()
//   5. drain to empty                    — needs pop() decrementing size()
//
// On Monday morning, before any Stack work is in, phases 2–5 all FAIL
// because push() is a no-op (size never grows). Phase 1 passes by accident
// because the stubbed size() returns 0 and empty() returns true — which
// happens to be the right answer for an empty stack. That's a known false
// positive; your Monday job is to make Phase 2 onward pass for the right
// reasons.

#include "StackTests.h"

#include <iostream>

#include "Stack.h"

namespace dungeon {

namespace {

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

}  // anonymous namespace

bool runStackSelfTest() {
    bool all = true;

    // --- Phase 1: empty stack invariants ---
    {
        Stack<int> s;
        const bool ok = (s.size() == 0 && s.empty());
        result("empty Stack<int>: size==0 && empty()", ok,
               "size() or empty() is stubbed wrong — both should already work");
        all = all && ok;
    }

    // --- Phase 2: single push ---
    {
        Stack<int> s;
        s.push(7);
        bool ok = (s.size() == 1 && !s.empty());
        // Guard the top() read in case push didn't actually grow the stack;
        // dereferencing an empty stack's top would be UB.
        if (ok) {
            ok = (s.top() == 7);
        }
        result("push(7): size==1 && top()==7", ok,
               "push() is a no-op (size didn't grow) — wire push to chain_.push_front");
        all = all && ok;
    }

    // --- Phase 3: LIFO ordering on three pushes ---
    {
        Stack<int> s;
        s.push(1);
        s.push(2);
        s.push(3);
        bool ok = (s.size() == 3 && !s.empty());
        if (ok) {
            // The whole point of LIFO: most recent push is on top.
            ok = (s.top() == 3);
        }
        result("push 1,2,3: size==3 && top()==3 (LIFO)", ok,
               "top() did not return the last pushed value — push/top probably target opposite ends of chain_");
        all = all && ok;
    }

    // --- Phase 4: pop reveals previous top ---
    {
        Stack<int> s;
        s.push(1);
        s.push(2);
        s.push(3);
        s.pop();
        bool ok = (s.size() == 2);
        if (ok) {
            ok = (s.top() == 2);
        }
        result("after pop(): size==2 && top()==2", ok,
               "pop() is a no-op, or pop targets the wrong end of chain_");
        all = all && ok;
    }

    // --- Phase 5: drain to empty ---
    {
        Stack<int> s;
        s.push(1);
        s.push(2);
        s.pop();
        s.pop();
        const bool ok = (s.size() == 0 && s.empty());
        result("drain to empty: size==0 && empty()", ok,
               "pop() does not decrement size — check size() reflects the underlying chain_.size()");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
