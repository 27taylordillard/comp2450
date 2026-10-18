
#include <iostream>
#include "StackQueue.h"

int main() {
    bool all = true;

    dungeon::StackQueue<int> q;
    bool ok = (q.size() == 0 && q.empty());
    std::cout << "empty StackQueue<int>: " << (ok ? "OK" : "FAIL") << "\n";
    all = all && ok;

    dungeon::StackQueue<int> q2;
    q2.enqueue(7);
    ok = (q2.size() == 1 && !q2.empty() && q2.front() == 7);
    std::cout << "enqueue(7): " << (ok ? "OK" : "FAIL") << "\n";
    all = all && ok;

    dungeon::StackQueue<int> q3;
    q3.enqueue(1);
    q3.enqueue(2);
    q3.enqueue(3);
    ok = (q3.size() == 3 && q3.front() == 1);
    std::cout << "enqueue 1,2,3 (FIFO): " << (ok ? "OK" : "FAIL") << "\n";
    all = all && ok;

    dungeon::StackQueue<int> q4;
    q4.enqueue(1);
    q4.enqueue(2);
    q4.enqueue(3);
    q4.dequeue();
    ok = (q4.size() == 2 && q4.front() == 2);
    std::cout << "after dequeue(): " << (ok ? "OK" : "FAIL") << "\n";
    all = all && ok;

    dungeon::StackQueue<int> q5;
    q5.enqueue(1);
    q5.enqueue(2);
    q5.dequeue();
    q5.dequeue();
    ok = (q5.size() == 0 && q5.empty());
    std::cout << "drain to empty: " << (ok ? "OK" : "FAIL") << "\n";
    all = all && ok;

    std::cout << "  " << (all ? "all phases OK" : "(see FAILs above)") << "\n";
    return all ? 0 : 1;
}
