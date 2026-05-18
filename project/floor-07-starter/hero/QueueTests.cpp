// COMP 2450 — Floor 7 starter
// hero/QueueTests.cpp — the `selftest queue` harness. Provided. You do not edit this.
//
// Runs five phases against Queue<int>. Each phase prints OK or FAIL with
// a one-line diagnostic that names the likely culprit (which Queue method
// is still stubbed or wired wrong).
//
//   1. empty queue invariants            — needs size(), empty()
//   2. single enqueue                    — needs enqueue() then size()/front()
//   3. FIFO ordering on three enqueues   — needs enqueue() AND front() returning FIRST-pushed
//   4. dequeue reveals next-oldest front — needs dequeue()
//   5. drain to empty                    — needs dequeue() decrementing size()
//
// On Monday morning, before any Queue work is in, phases 2–5 all FAIL
// because enqueue() is a no-op (size never grows). Phase 1 passes by
// accident because the stubbed size() returns 0 and empty() returns true —
// which happens to be the right answer for an empty queue. That's a
// known false positive; your Monday job is to make Phase 2 onward pass
// for the right reasons.
//
// The deliberate contrast with `selftest stack`: Phase 3 here checks
// front() == 1 (FIRST enqueued is at the front), not == 3. That single
// asserted value is the LIFO vs FIFO difference encoded in a test.

#include "QueueTests.h"

#include <iostream>

#include "Queue.h"

namespace dungeon {

namespace {

void result(const char* phase, bool ok, const char* diag = nullptr) {
    std::cout << "  " << phase << ": " << (ok ? "OK" : "FAIL");
    if (!ok && diag) std::cout << " — " << diag;
    std::cout << "\n";
}

}  // anonymous namespace

bool runQueueSelfTest() {
    bool all = true;

    // --- Phase 1: empty queue invariants ---
    {
        Queue<int> q;
        const bool ok = (q.size() == 0 && q.empty());
        result("empty Queue<int>: size==0 && empty()", ok,
               "size() or empty() is stubbed wrong — both should already work");
        all = all && ok;
    }

    // --- Phase 2: single enqueue ---
    {
        Queue<int> q;
        q.enqueue(7);
        bool ok = (q.size() == 1 && !q.empty());
        // Guard the front() read in case enqueue didn't actually grow
        // the queue; dereferencing an empty queue's front would be UB.
        if (ok) {
            ok = (q.front() == 7);
        }
        result("enqueue(7): size==1 && front()==7", ok,
               "enqueue() is a no-op (size didn't grow) — wire enqueue to chain_.push_back");
        all = all && ok;
    }

    // --- Phase 3: FIFO ordering on three enqueues ---
    {
        Queue<int> q;
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        bool ok = (q.size() == 3 && !q.empty());
        if (ok) {
            // The whole point of FIFO: the FIRST enqueued is at the front.
            // (Contrast with Stack where the LAST pushed is on top.)
            ok = (q.front() == 1);
        }
        result("enqueue 1,2,3: size==3 && front()==1 (FIFO)", ok,
               "front() did not return the FIRST enqueued value — enqueue/front are on the same end (that's a stack, not a queue)");
        all = all && ok;
    }

    // --- Phase 4: dequeue reveals next-oldest front ---
    {
        Queue<int> q;
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);
        q.dequeue();
        bool ok = (q.size() == 2);
        if (ok) {
            // After dequeue, the OLDEST remaining is now at the front.
            ok = (q.front() == 2);
        }
        result("after dequeue(): size==2 && front()==2", ok,
               "dequeue() is a no-op, or dequeue targets the wrong end of chain_");
        all = all && ok;
    }

    // --- Phase 5: drain to empty ---
    {
        Queue<int> q;
        q.enqueue(1);
        q.enqueue(2);
        q.dequeue();
        q.dequeue();
        const bool ok = (q.size() == 0 && q.empty());
        result("drain to empty: size==0 && empty()", ok,
               "dequeue() does not decrement size — check size() reflects the underlying chain_.size()");
        all = all && ok;
    }

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all;
}

}  // namespace dungeon
