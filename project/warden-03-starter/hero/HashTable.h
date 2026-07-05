// COMP 2450 — Warden of the Trees (Midterm 3) starter — post-Floor-11 reference
// hero/HashTable.h — a separate-chaining hash table.
// COMPLETE Floor 11 reference (your Hall of Whispers work).
//
// You built this on Floor 11 (The Hall of Whispers): a hash table whose buckets
// are your own Chain<Monster> from Floor 4 — a hash table IS an array of the
// linked list you wrote five floors earlier. The six bodies you wrote (hashKey,
// bucketIndex, insert, find, erase, rehashIfNeeded) plus the one re-file line in
// the rehash driver are all filled in here. It ships finished so the Warden's
// encounter can lean on it — the whisper is your F11 tie's instant identify.
//
// The Hall of Names answered in O(log n) by keeping everything in ORDER — a
// balanced tree the library walked for you. The Hall of Whispers answers in
// O(1) AVERAGE by giving up order entirely: it COMPUTES a bucket from the name
// (a hash) and goes straight there. A whisper, not a walk.
//
// =====================================================================
// What this class is — an array of the linked lists you wrote on Floor 4
// =====================================================================
// A separate-chaining hash table. The bucket array is a std::vector, and each
// slot is one of YOUR OWN Chain<Monster> linked lists (hero/Chain.h, Floor 4)
// holding every monster that hashes to that index. When two different names land
// in the same bucket — a COLLISION, which is not a bug but inevitable (more
// names than buckets) — the bucket's Chain simply holds both, and you scan a
// short list of two instead of all 15.
//
// It is keyed on the monster's NAME (a std::string) and STORES the whole Monster
// as the value, so find() hands back a const Monster* exactly like Floor 10's
// Registry::recall. It is added IN PARALLEL to the working Bag<Monster>,
// BST<Monster> (Floor 9), and Registry (Floor 10) — nothing else is touched. `banish`-ing a monster from this table does NOT
// touch any other index; the independent indexes can legitimately diverge after
// a mutation.
//
// =====================================================================
// Two traps the floor warned about (kept here as reference)
// =====================================================================
// (1) bucketIndex reduces the hash code with `% bucketCount`, NEVER an unchecked
//     index. The constructor guarantees bucketCount() >= 1, so a correct
//     `% bucketCount` can never divide by zero.
// (2) find compares the NAME, not just the bucket. Two names can share a bucket
//     (a collision); returning the first item in the bucket is the bug.

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Chain.h"
#include "../bestiary/Monster.h"

namespace dungeon {

class HashTable {
public:
    // The starting bucket count. Small ON PURPOSE: with 15 monsters this fills
    // past the load-factor threshold and the table GROWS itself during load —
    // the whole point of Floor 11 made visible (see rehashIfNeeded).
    static constexpr std::size_t kInitialBuckets = 8;

    // The load-factor trigger (ZyBook §12.4). When items / buckets exceeds this,
    // rehashIfNeeded() doubles the bucket count so chains stay short.
    static constexpr double kMaxLoadFactor = 0.75;

    // ----- construction (complete) -----------------------------------
    // Size the bucket array and zero the count. We std::max with 1 so the table
    // ALWAYS has at least one bucket — that is what lets bucketIndex do
    // `% bucketCount()` without ever dividing by zero, even if someone asks for
    // 0 buckets.
    explicit HashTable(std::size_t initialBuckets = kInitialBuckets)
        : buckets_(initialBuckets < 1 ? 1 : initialBuckets), count_(0) {}

    // Build the table from McCown's bestiary: walk it in FILE order and insert()
    // each monster. The exact Registry pattern.
    explicit HashTable(const Bag<Monster>& bestiary)
        : buckets_(kInitialBuckets), count_(0) {
        for (const Monster& m : bestiary) insert(m);
    }

    // ----- trivial accessors (complete) ------------------------------
    std::size_t size()        const { return count_; }
    bool        empty()       const { return count_ == 0; }
    std::size_t bucketCount() const { return buckets_.size(); }

    // The §12.4 load factor: items divided by buckets.
    double loadFactor() const {
        return static_cast<double>(count_) / static_cast<double>(buckets_.size());
    }

    // Read-only view of the bucket array, so the `buckets` REPL command and the
    // self-test can print WHICH names share a bucket and iterate the whole table.
    const std::vector<Chain<Monster>>& buckets() const { return buckets_; }

    // ----- the resize DRIVER (complete) ------------------------------
    // Allocate a fresh bucket vector of newBucketCount, then RE-FILE every
    // existing item into it (an item's home bucket CHANGES when the bucket count
    // changes). We build a fresh vector and swap rather than mutating buckets_
    // while iterating it (mutating a container you are walking is an aliasing
    // bug). count_ is untouched: rehash MOVES items, it does not add or drop any.
    void rehash(std::size_t newBucketCount) {
        if (newBucketCount < 1) newBucketCount = 1;
        std::vector<Chain<Monster>> grown(newBucketCount);
        for (const Chain<Monster>& bucket : buckets_) {
            for (const Monster& m : bucket) {
                grown[hashKey(m.name) % newBucketCount].push_back(m);
            }
        }
        buckets_.swap(grown);
    }

    // ===== the six method bodies (your Floor 11 work, complete) ======

    // Compute a hash CODE for a name — a MULTIPLICATIVE STRING HASH (§12.5).
    // h = h * 31 + c over the characters mixes EVERY character, so similar names
    // land far apart. The static_cast to unsigned char matters: a plain char can
    // be negative. Overflow is fine — size_t wraps mod 2^64, part of the mixing.
    std::size_t hashKey(const std::string& name) const {
        std::size_t h = 0;
        for (char c : name) {
            h = h * 31 + static_cast<unsigned char>(c);
        }
        return h;
    }

    // Reduce the hash code to a valid bucket index — the MODULO HASH step
    // (§12.5). Always `% bucketCount`; the constructor guarantees bucketCount()
    // >= 1, so this can never divide by zero.
    std::size_t bucketIndex(const std::string& name) const {
        return hashKey(name) % buckets_.size();
    }

    // Add m to its home bucket. If the name is already in that bucket's chain,
    // UPDATE it in place and return false (no double-count); else push_back and
    // ++count_ (return true). Then rehashIfNeeded() so the table grows if it just
    // got too crowded.
    bool insert(const Monster& m) {
        std::size_t i = bucketIndex(m.name);
        for (Monster& existing : buckets_[i]) {
            if (existing.name == m.name) {
                existing = m;
                return false;
            }
        }
        buckets_[i].push_back(m);
        ++count_;
        rehashIfNeeded();
        return true;
    }

    // Return a pointer to the named monster, or nullptr — compute the bucket,
    // then scan ONLY that one chain for a node whose NAME matches. find compares
    // the name, not just the bucket: two names can collide into one bucket.
    const Monster* find(const std::string& name) const {
        std::size_t i = bucketIndex(name);
        for (const Monster& m : buckets_[i]) {
            if (m.name == name) return &m;
        }
        return nullptr;
    }

    // Remove the named monster from its bucket's chain via the provided
    // Chain::eraseFirst(pred); --count_ only if something was removed. Removes
    // ONE item; colliding neighbors stay put.
    bool erase(const std::string& name) {
        std::size_t i = bucketIndex(name);
        bool removed = buckets_[i].eraseFirst(
            [&](const Monster& m) { return m.name == name; });
        if (removed) --count_;
        return removed;
    }

    // If the load factor has crept above the threshold (§12.4), DOUBLE the
    // bucket count and rehash; return the (possibly new) bucket count. This is
    // what keeps O(1) from decaying toward O(n) as you insert.
    std::size_t rehashIfNeeded() {
        if (loadFactor() > kMaxLoadFactor) {
            rehash(buckets_.size() * 2);
        }
        return buckets_.size();
    }

private:
    std::vector<Chain<Monster>> buckets_;   // the bucket array: an array of YOUR Chain
    std::size_t                 count_;      // items stored (so size()/loadFactor() are O(1))
};

}  // namespace dungeon
