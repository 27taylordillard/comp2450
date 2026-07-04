---
title: The Chain Vault
floor_number: Floor 4
order: 60
flavor_quote: "The mold made a shape that fit a known size. The chain holds whatever you bring it."
week: 5
zybook: "Chapter 6 — full chapter; class focus §6.1–6.5"
---

## The briefing

The Warden lies broken behind you. Beyond the gate, the air goes cold and the floor changes — no more cut stone, no more brass forge-tools. You are in a long, low chamber lined with **iron lockboxes**, each one cuffed to the next by a single hand-forged link. There is no shelf. There is no mold. Boxes hang one from another in a chain that vanishes into the dark.

A woman steps from the shadow with a coil of fine chain across one shoulder.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/mavren.svg' | relative_url }}" alt="Quartermistress Mavren, Keeper of the Chain Vault — silver-streaked dark hair braided through iron rings, slate-grey tunic under a chainmail half-apron, a coiled iron chain crossing from her right shoulder down to her belt, a brass tally-tag on a thin chain at her collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Quartermistress Mavren, Keeper of the Chain Vault</span>

The Forgemaster pressed you a shape that fit a known size. I keep things whose number we do not know yet — every blow your hero strikes, every name they hear, every door they open. We do not re-cast the chain. *We add a link.* And when the chain is done with us — when the lights go out — every link we forged, we melt back down with our own hands. You leak no iron in my vault.
</div>
</div>

Last week the Forgemaster gave you `Bag<T>` — one mold, any metal. Useful, but it was a thin wrapper over `std::vector<T>`: the storage was the standard library's, not yours. This week you build your **own** container, from raw nodes and pointers, with no `std::vector` underneath.

You will build a **`Chain<T>`** — a singly-linked list. And you will use it to give your hero something the dungeon has been missing: a **scrying log**, a running record of every notable event the hero causes or suffers. Every command the hero issues, every monster they fight, every bad index they pass to `inspect` — the log records it. The newest entry sits at the *head* of the chain; older entries trail behind. New events are O(1) to record. *That* is what a linked list buys you.

You will learn three things this week, and they are inseparable:

1. **Nodes and pointers.** A linked list is just a chain of small objects, each holding one piece of data and one pointer to the next. The list itself owns a `head` pointer; the last node's `next` is `nullptr`. To walk the chain, you start at `head` and follow `next` until you fall off the end. There is no `chain[i]` shortcut.
2. **`new`, `delete`, and the destructor.** Each node is allocated with `new Node`. *Something* has to free those nodes — and it cannot be the caller. It is the `Chain<T>` destructor's job to walk its own chain at end-of-life and `delete` every node. If it doesn't, every event your hero ever logs leaks for the lifetime of the program.
3. **The Rule of Three.** Once you write a destructor, the compiler-generated copy constructor and copy assignment are *broken* — they would copy the head pointer and let two `Chain`s share the same nodes, then both delete them. You must either implement copy properly (a deep copy that walks the source chain and clones each node) or **delete** them so the compiler refuses to compile a copy. You will do the latter this week and the former on Floor 4½.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/linked-list-anatomy.svg' | relative_url }}" alt="A row of three nodes. Each node has a data cell on the left and a 'next' cell on the right. A 'head' arrow points into the leftmost node. The 'next' cells of the first two nodes point right into the next node; the rightmost node's 'next' points to a nullptr ground symbol."/>
  <figcaption>A chain is just nodes. Each node owns one piece of data and one pointer to the next. The chain ends where a <code>next</code> is <code>nullptr</code> — the ground symbol. <code>head</code> is the only handle into the whole structure.</figcaption>
</figure>

`Chain<T>` is *not* a replacement for `Bag<T>`. The hero's inventory and the keep's bestiary stay on `Bag<Item>` and `Bag<Monster>` — you still need `std::sort`, you still need random-access `inspect <n>`, and `Bag<T>` is right for those. The chain is right for the **log**, where the only operations are *prepend a new event*, *count*, and *walk the most recent N*.

## Objectives

By the end of Floor 4 you will be able to:

- Define a `Node<T>` struct with a `T` payload and a `Node<T>* next` pointer, and explain why `next` cannot be a `Node<T>` by value.
- Implement `Chain<T>::push_front` in O(1) and explain *why* it is O(1) — what does it not have to do that `std::vector::insert(begin())` does?
- Walk a chain manually with `for (const Node<T>* p = chain.head(); p != nullptr; p = p->next) ...` and state what each piece of that loop is doing.
- Implement `Chain<T>`'s **destructor** — a loop that walks the chain, captures `next` *before* deleting the current node, and continues until the chain is empty.
- State the **Rule of Three** and apply it: when you write a destructor, you must also handle copy construction and copy assignment, or `= delete` them.
- Verify under a leak detector (or `valgrind`, or AddressSanitizer) that your `Chain<T>` leaks zero bytes when the program exits cleanly.
- Compare the Big-O of `push_front`, indexed access, and full traversal between `Chain<T>` and `Bag<T>` and pick the right container for a given access pattern.

## Pre-class

### Reading (ZyBook Ch. 6 — the whole chapter)

**Read all of Chapter 6.** The reading assignment is the entire chapter — including the doubly-linked-list and iterator sections we won't reach in class until Floors 4½ and 5. Reading them now means you meet them twice: once cold here, once in depth later. The day-by-day pointers below are what *this week's* class leans on.

Before **Monday**: §6.1 *List abstract data type* (the operations any list supports) and §6.2 *Singly-linked lists* (nodes, head and tail, the `next` pointer, append and prepend).
Before **Wednesday**: §6.3 *Singly-linked lists: Insert* and §6.4 *Singly-linked lists: Remove* — the splice-in / splice-out pointer surgery behind `push_front`, `pop_front`, and friends.
Before **Friday**: §6.5 *Linked list search* (walking the chain to find a key). The Rule of Three — the destructor and deep copy your `Chain<T>` needs — is class material this floor; the textbook doesn't give it a section. The doubly-linked sections (§6.6–6.8) are Floor 4½'s preview.

The animation in §6.2 — the one that draws each `next` arrow as a node is added — is the picture you want in your head when you write `push_front`. Watch it twice.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Nodes, head pointer, push_front | Live-code `Chain<T>` from the starter declaration: `Node`, `head_`, `size_`, `push_front`, `size`, `empty`, `head()`. Walk the chain by hand on the projector. Add the `log` command — every other command in `main` already calls `hero.eventLog.push_front(...)`; today we make the log *visible*. |
| **W** | Destructor + Rule of Three | Live-code `~Chain()` together. Demonstrate the leak with a sanitizer (or with a counter on `Node` ctor/dtor). Discussion: why the compiler-generated copy ctor is wrong here. Add `Chain(const Chain&) = delete;` and `Chain& operator=(const Chain&) = delete;`. Try to copy a chain — read the compiler error together. |
| **F** | Big-O, when to choose linked  | Add the `benchmark log` command and time `push_front` on a `Chain<std::string>` versus `Bag<std::string>::insert(begin(), ...)` on N = 1,000 / 10,000 / 100,000. Read the table together. Discussion: why this chapter is not "linked lists are always better." |

## The project — Floor 4

This week's project increment is **the scrying log** — a running record of significant events, prepended one link at a time.

You will receive (in your starter drop):

- Everything through the Warden, fully working — `Bag<T>`, `BagException`, `findByName<T>`, `mergeSort`/`quicksort`, all four benchmarks, the boss battle.
- A header `hero/Chain.h` with the class template declaration — `template<typename T> class Chain`, a private `Node<T>* head_`, signatures for `push_front`, `size`, `empty`, `clear`, `head()`, the destructor, and the deleted copy operations.
- A `hero/Hero.h` updated so `Hero` now owns a `Chain<std::string> eventLog`.
- A `main.cpp` whose every command branch already calls `hero.eventLog.push_front(...)` with a one-line description of what just happened. The log is being *written* the moment the program starts. It is just not yet being *read*.
- A `log` command in `main.cpp` that calls `printLog(hero, n)` — a function declared in `Hero.h` that **you write**.
- A `benchmark log` command, fully implemented, that compares `Chain<std::string>::push_front` against `Bag<std::string>::insert(begin(), ...)`.

You will write:

1. **Monday:** `Chain<T>::push_front`, `size`, `empty`, `head()`, plus `printLog(const Hero&, std::size_t n)` in `hero/Hero.cpp`. By the end of Monday's class, `log 5` prints the five most recent events your hero has caused, newest first.
2. **Wednesday:** `Chain<T>::~Chain()` and `Chain<T>::clear()`. Verify with the test harness in `hero/ChainTests.cpp` (built into the binary as the `selftest` command) that allocations equal deallocations. Read the failure message; fix it; commit only when the count is balanced.
3. **Friday:** Add `Chain(const Chain&) = delete;` and `operator=(const Chain&) = delete;`. Run `benchmark log` and paste the table into your lab notes. Reflect on the result.

Demo target (Friday):

```
> search Goblin
  Goblin   HP 8   ATK 2   weakness: fire
> inventory
  1.  Rusty sword       (wt 4.0, val 5)
  2.  Healing potion    (wt 0.5, val 12)
  3.  Iron key          (wt 0.1, val 0)
  4.  Loaf of bread     (wt 0.1, val 1)
  5.  Cloak of shadows  (wt 1.5, val 80)
> sort inventory by value desc
  1.  Cloak of shadows  (wt 1.5, val 80)
  2.  Healing potion    (wt 0.5, val 12)
  3.  Rusty sword       (wt 4.0, val 5)
  4.  Loaf of bread     (wt 0.1, val 1)
  5.  Iron key          (wt 0.1, val 0)
> inspect 99
  No such item. (index 98 out of bounds for size 5)
> log 8
  1.  error: index 98 out of bounds for size 5
  2.  sort inventory by value desc
  3.  inventory — listed 5 items
  4.  search Goblin — found in bestiary
  5.  began session as "Aric"
  (newest first; chain length 5)
> benchmark log 100000
  N= 100000   Chain::push_front =     9.04 ms   Bag::insert(begin) = 13318.22 ms
  (numbers vary by machine; the ~1000x gap is the point — O(1) vs O(n) per insert)
> selftest chain
  Chain<int> allocations:  1000   deallocations:  1000   leaked:     0   OK
> quit
  The forge cools. The chain dissolves link by link.
  (the Chain destructor runs silently here — verify it with `selftest chain`)
```

### Lab 4 — *One Link at a Time* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-04/lab-notes.md` to your project repo with:

1. A transcript of the demo above. Paste it from your terminal.
2. **Comment out your destructor body** so it does nothing. Run `selftest chain` again. Paste the output. How many nodes leaked? Restore the destructor and confirm the count returns to zero.
3. **Try to copy a chain.** Add this two-liner to `main.cpp` somewhere harmless and try to compile it: `Chain<int> a; a.push_front(1); Chain<int> b = a;`. Paste the compiler error. Which line of `Chain.h` is the compiler pointing at? In one sentence, explain *why* the compiler refuses — what would go wrong at end-of-program if it had let you?
4. **Time `push_front` against `insert(begin())`.** Paste your `benchmark log 100000` output. Both inserts do "the same thing" — put the new element at the front. Why does the vector version cost ~1000× more for N = 100,000? Answer in terms of what `std::vector::insert(begin())` is *physically* doing to memory.
5. **One-paragraph reflection.** Mavren keeps the bestiary in a `Bag<Monster>` but the event log in a `Chain<std::string>`. Defend her choice for *each* container — what access pattern does each face, and what would go wrong if you swapped them?

Your commit history this week should show at least three commits — Mon (push_front + log command), Wed (destructor), Fri (deleted copies + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist offers a clever shortcut</span>
*Listen*, friend! Why bother with a destructor? The program *ends*, yes? When program ends, **operating system gets all the memory back!** Free! Automatic! Why write the loop? Grix never writes the loop. Grix's program runs *fine*. Until — well. *You'll see.*
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 4</p>

**The Drip-Wraith** — *HP: grows by 1 every allocation. Damage: silent, deferred, fatal at scale.*
Strikes when a programmer trusts the operating system to "clean up at the end" instead of writing a destructor. The short-lived program looks fine — `valgrind` reports leaks the developer never reads, the test suite passes, the demo works. The Drip-Wraith waits. It manifests in production: a long-running server process, a game loop running for hours, an embedded device with no OS to bail it out. Memory grows. The process slows. Eventually the process is killed by the OS, or — worse — the embedded device hangs and someone has to drive out and reboot it.

Counter by:

- **Writing the destructor whenever you write a `new`.** Every allocation owes a deallocation. The destructor is where you pay that debt. If your class doesn't allocate, it doesn't need a destructor — that's the *Rule of Zero*, and it is the rule you should reach for first. The Rule of Three is what you do when Rule of Zero isn't available.
- **Walking the chain with `next` saved first.** `Node<T>* p = head_; while (p) { Node<T>* n = p->next; delete p; p = n; }`. If you `delete p` and *then* read `p->next`, you have read freed memory — undefined behaviour, sometimes a crash, sometimes silently wrong.
- **Running with a leak detector at least once.** AddressSanitizer (`-fsanitize=address` on Clang/GCC) or Visual Studio's CRT debug heap will tell you in the first 30 seconds what your eyes will miss for a year. The test target in this week's `CMakeLists.txt` enables it for you.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.
</div>

---

*Mavren snaps the brass tally-tag onto a fresh link and lets the chain hang loose. The vault still extends into darkness — there are forms of chain you have not yet been shown. The stair to Floor 4½ — the **Chain Vault, Deeper** — opens only when you can walk a chain in both directions.*
