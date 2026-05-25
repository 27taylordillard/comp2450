// COMP 2450 — Warden of the Foundations starter (post-Floor-3 reference state)
// hero/Bench.cpp — sort benchmark, retargeted at Bag<Item> to match
// the post-Floor-3 signatures of mergeSort / quicksort.

#include "Bench.h"
#include "Sort.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

namespace dungeon {

namespace {

// Build a synthetic inventory of N items with reproducible pseudo-random
// names, weights, and values. Deterministic across runs because we seed
// the RNG with a FIXED value (0xC0FFEE).
Bag<Item> makeSynthetic(std::size_t n) {
    std::mt19937_64 rng(0xC0FFEE);  // 64-bit Mersenne Twister, fixed seed
    std::uniform_real_distribution<double> weightDist(0.1, 50.0);
    std::uniform_int_distribution<int>     valueDist(0, 1000);

    Bag<Item> v;
    for (std::size_t i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "Item_" << std::setfill('0') << std::setw(7) << i;
        v.push_back({oss.str(), weightDist(rng), valueDist(rng)});
    }
    return v;
}

// The default ascending-weight comparator used throughout the bench.
const Comparator kCmpWeight = [](const Item& a, const Item& b) {
    return a.weight < b.weight;
};

// A first-element-pivot quicksort that lives ONLY inside the benchmark
// harness, so we can demonstrate the Pivot Wraith without relying on
// the student's own (middle-pivot) implementation. Lomuto partition.
void badQuicksortImpl(Bag<Item>& v,
                      std::size_t lo, std::size_t hi,
                      const Comparator& cmp) {
    if (lo >= hi) return;
    std::swap(v[lo], v[hi]);
    const Item pivot = v[hi];
    std::size_t store = lo;
    for (std::size_t i = lo; i < hi; ++i) {
        if (cmp(v[i], pivot)) {
            std::swap(v[store], v[i]);
            ++store;
        }
    }
    std::swap(v[store], v[hi]);

    std::size_t p = store;
    if (p > lo) badQuicksortImpl(v, lo, p - 1, cmp);
    badQuicksortImpl(v, p + 1, hi, cmp);
}

void badQuicksort(Bag<Item>& v, const Comparator& cmp) {
    if (v.size() < 2) return;
    badQuicksortImpl(v, 0, v.size() - 1, cmp);
}

// A volatile sink the compiler cannot prove unused. We write the bag's
// address into it after each sort so the timing loop doesn't get
// eliminated by dead-store optimization.
static volatile const void* g_benchSink = nullptr;

// Time a sort callable (which receives a fresh copy of `base` each run)
// across `iterations` iterations and return the average wall-clock ms.
//
// Notice the copy INSIDE the loop: `Bag<Item> v = base;`. This is
// deliberate. If we sorted `base` once and reused it, the second
// iteration would already be sorted — we'd be timing "detect sorted,"
// not "perform sort." Every timed run starts from identical state.
double avgMillis(const Bag<Item>& base,
                 const std::function<void(Bag<Item>&)>& sortFn,
                 std::size_t iterations) {
    double totalMs = 0.0;
    for (std::size_t i = 0; i < iterations; ++i) {
        Bag<Item> v = base;          // fresh unsorted copy per run (untimed)
        auto t0 = std::chrono::high_resolution_clock::now();
        sortFn(v);
        auto t1 = std::chrono::high_resolution_clock::now();
        totalMs += std::chrono::duration<double, std::milli>(t1 - t0).count();
        g_benchSink = &v;            // defeat dead-store elimination
    }
    return totalMs / static_cast<double>(iterations);
}

void printRow(std::size_t n,
              double mergeMs, double quickMs, double stdMs,
              bool badPivot) {
    std::cout << "  N=" << std::setw(7) << std::right << n
              << "  mergeSort=" << std::setw(8) << std::fixed
                                << std::setprecision(3) << mergeMs << " ms"
              << "  " << (badPivot ? "quicksort*=" : "quicksort= ")
              << std::setw(8) << quickMs << " ms"
              << "  std::sort=" << std::setw(8) << stdMs << " ms"
              << "\n";
    std::cout.unsetf(std::ios::fixed);
}

}  // anonymous namespace

void runSortBenchmark(std::size_t n,
                      SortBenchOptions opts,
                      std::size_t iterations) {
    Bag<Item> base = makeSynthetic(n);
    if (opts.presorted) {
        std::sort(base.begin(), base.end(), kCmpWeight);
    }

    double m  = avgMillis(base,
                          [&](Bag<Item>& v) { mergeSort(v, kCmpWeight); },
                          iterations);
    double q  = avgMillis(base,
                          [&](Bag<Item>& v) {
                              if (opts.badPivot) badQuicksort(v, kCmpWeight);
                              else               quicksort(v, kCmpWeight);
                          },
                          iterations);
    double s  = avgMillis(base,
                          [&](Bag<Item>& v) {
                              std::sort(v.begin(), v.end(), kCmpWeight);
                          },
                          iterations);

    printRow(n, m, q, s, opts.badPivot);
}

void runSortBenchmarkSweep(SortBenchOptions opts) {
    std::cout << "\n-- Race the Sorts (" << (opts.presorted ? "pre-sorted input" : "random input")
              << (opts.badPivot ? ", BAD PIVOT quicksort" : "")
              << ") --\n";

    for (std::size_t n : {std::size_t{10}, std::size_t{100}, std::size_t{1000},
                          std::size_t{10000}, std::size_t{100000}}) {
        runSortBenchmark(n, opts);
    }

    if (opts.badPivot) {
        std::cout << "\n(quicksort* = first-element pivot. Compare to a middle-pivot\n"
                  << " run, especially on --sorted input. The Pivot Wraith is real.)\n\n";
    } else {
        std::cout << "\nNotice: std::sort usually beats both of yours — smaller constant,\n"
                  << "cache-aware inner loops, introsort fallback. Your two exist to\n"
                  << "TEACH the shape; std::sort exists to SHIP.\n\n";
    }
}

}  // namespace dungeon
