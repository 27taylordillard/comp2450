// COMP 2450 — Floor 2 starter
// hero/Sort.cpp — YOU implement this file.
//
// Three functions to write. Read Sort.h for their contracts.
//
// The big idea this week: sorting is not one algorithm, it is a family
// of tradeoffs. Merge sort is predictable but copies. Quicksort is fast
// on average but betrays you on bad inputs. std::sort is what you
// actually ship. You will write the first two, race all three in
// `benchmark sort`, and argue — in a commit message — which one the
// game should call.
//
// Tips for the Pivot Wraith:
//   * If you pick the FIRST element as your quicksort pivot, a sorted
//     input becomes O(n^2). The `benchmark sort --bad-pivot --sorted`
//     harness exists to show you exactly that.
//   * The middle element is the cheapest defense. Good enough for this
//     week. Real production code (std::sort) does median-of-three and
//     switches algorithms on bad recursion depth.
//
// Submit when:  `sort inventory by weight` produces ascending weight,
//               `sort inventory by name desc` produces reverse alphabetical,
//               and `benchmark sort` gives three timing columns on every row.

#include "Sort.h"
#include <algorithm>  // you will want std::sort in sortInventory



namespace dungeon {

    namespace{
        void merge(std::vector<Item>& v, 
        std::size_t low,
        std:: size_t mid,
        std::size_t high,
        const Comparator& cmp){
            //[low, mid)
            //[mid, high)
            std::vector<Item> scratch;
            scratch.reserve(high-low);
            //allocating capacity for high - low items
            // up front

            //two cursors for each side
            std::size_t i = low; // walk the left half
            std:: size_t j = mid; //walk right half

            //merge loop
            //while both halves still have items,
            //pick the smaller front-of-queue and append
            while(i < mid && j < high) {
                if(!cmp(v[j], v[i])) {
                    scratch.push_back(v[i++]);
                }
                else {
                    scratch.push_back(v[j++]);
                }
            }
            //one half is drained but the other still has
            //items
            while(i < mid) scratch.push_back(v[i++]);
            while(j < high) scratch.push_back(v[j++]);

            //copy the merge result back into v
            // at the positions [low,high)
            for(std::size_t k = 0; k < scratch.size(); ++k) {
                v[low + k] = std::move(scratch[k]);
            }
        }

        void mergeSortImpl(std::vector<Item>& v,
        std::size_t low, std::size_t high,
        const Comparator& cmp) {
            //base case
            if(high - low < 2) return;
            //recursion!
            std::size_t mid = low + (high - low) / 2;
            mergeSortImpl(v, low, mid, cmp);
            mergeSortImpl(v, mid, high, cmp);
            merge(v, low, mid, high, cmp);
        }

        std::size_t partition(std::vector<Item>& v,
        std::size_t low, std::size_t high,
        const Comparator& cmp) {
            // high is our last index(inclusive)
            //1.) pick the pivot
            std::size_t mid = low + (high - low) / 2;
            std::swap(v[mid], v[high]);
            const Item pivot = v[high];
            //compue middle index
            //std::swap exchanges two items
            // w/o copying the whole struct
            //In lomuto , assumes the pivot 
            // lives at high, so be moving
            // our pivot there, we can follow
            // classic Lomuto 

            //lomuto scan
            std::size_t store = low;
            for(std::size_t i = low; i < high; ++i) {
                if(cmp(v[i], pivot)) {
                    std::swap(v[store], v[i]);
                    ++store;
                }
            }
            std::swap(v[store], v[high]);
            return store;
        }

    }

// ---- 1. Merge sort ------------------------------------------------------

void mergeSort(std::vector<Item>& inventory, const Comparator& cmp) {
    mergeSortImpl(inventory, 0, inventory.size(), cmp);
}

// ---- 2. Quicksort -------------------------------------------------------

void quicksort(std::vector<Item>& inventory, const Comparator& cmp) {
    // TODO Floor 2 (Wed): implement quicksort.
    //
    // Think before you type:
    //   - Quicksort's whole performance story depends on the PIVOT. If
    //     the pivot splits the range roughly in half each time, you get
    //     O(n log n). If the pivot always ends up at one end (everything
    //     goes to one side), you get O(n^2). Why does the FIRST element
    //     cause that on sorted input? Sketch it on paper for [1,2,3,4,5].
    //   - Your fix is the MIDDLE element. It's not bulletproof — an
    //     adversary could still construct a worst-case input — but it
    //     kills the most common pathology (sorted / reverse-sorted data),
    //     which is exactly the shape real users produce.
    //   - `std::size_t` is unsigned. When `p == 0`, what is `p - 1`?
    //     That wrap-around will send your left-side recursion to index
    //     18 quintillion. Guard it.
    //   - Is quicksort stable? (Answer: no — and that is why production
    //     std::sort is ALSO not stable. If you need stability, reach for
    //     std::stable_sort or your mergeSort.)
    //
    // If you need structural hints — helpers in an anonymous namespace:
    //
    //   static std::size_t partition  (std::vector<Item>& v,
    //                                  std::size_t lo, std::size_t hi,
    //                                  const Comparator& cmp);
    //   static void        quicksortImpl(std::vector<Item>& v,
    //                                    std::size_t lo, std::size_t hi,
    //                                    const Comparator& cmp);
    //
    // Closed range convention for quicksort: [lo, hi] — both inclusive.
    // Textbooks use this for Lomuto partition; it is fine here. Guard
    // the recursive call `quicksortImpl(v, lo, p - 1, cmp)` with
    // `if (p > lo) ...` so you do not underflow when p == 0.
    //
    // PIVOT: use the middle element — `lo + (hi - lo) / 2`. Move it to
    //        the end (swap it with v[hi]) and then do the standard
    //        Lomuto scan with the pivot now at v[hi].
    //
    // If you are curious what the FIRST-element pivot looks like: the
    // benchmark harness has a `--bad-pivot` option that runs exactly
    // that. You do NOT need to implement it yourself; the harness
    // ships its own copy for Lab purposes.
    (void)inventory;
    (void)cmp;
}

// ---- 3. sortInventory (the seam) ----------------------------------------

bool sortInventory(Hero& hero, const std::string& criterion) {
   (void)criterion;
   Comparator byWeight = [](const Item& a, const Item& b) {
        return a.weight < b.weight;
   };
   mergeSort(hero.inventory, byWeight);
   return true;
}

}  // namespace dungeon
