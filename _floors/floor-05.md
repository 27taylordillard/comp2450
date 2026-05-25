---
title: The Eye of Scrying
floor_number: Floor 5
flavor_quote: "The lens does not know the shelf. The lens only knows how to move and how to stop."
week: 7
zybook: "Chapter 6 (continued) — class focus §6.14 (iterators)"
---

## The briefing

Beyond Mavren's vault the corridor narrows into a circular chamber lit by a single hanging brass lantern. The walls are smooth stone, unmarked except for a wide unlidded eye carved at the apex of the ceiling. A woman stands at the centre with a strip of pale cloth tied across her eyes. A small glass lens hangs on a brass chain at her sternum; she does not lift it for you.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/vael.svg' | relative_url }}" alt="Sister Vael, the Lensbearer — pale woman in deep-violet robes with silver-thread trim at the collar, a strip of unbleached linen tied across her eyes with a small knot at her right temple, dark hair pulled back tight, a polished glass lens the size of a coin in a brass rim hanging on a brass chain at her sternum."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Sister Vael, the Lensbearer</span>

I cannot see your inventory. I cannot see Mavren's chain. The world above ground I have not seen in thirty years. And yet I can walk any list in this dungeon from end to end and find what I am looking for in it. *How.* — Because I do not look at the list. I look through a lens, and the lens knows two things only: how to advance, and where to stop. The shelf becomes the bag. The bag becomes the chain. The chain becomes the tree, on a floor you have not yet seen. The lens does not change. Build me one lens, and you build it for every shape the dungeon will ever hand you.
</div>
</div>

For two floors now you have walked the chain by hand. `for (auto* p = chain.head(); p != nullptr; p = p->next)` for newest-first. `for (auto* p = chain.tail(); p != nullptr; p = p->prev)` for oldest-first. And for the bag, on every floor before that, you wrote `for (std::size_t i = 0; i < bag.size(); ++i)`. Three different loop shapes for the *same operation*: visit each element in turn.

This week you collapse all three into one. You build a small type — `Chain<T>::iterator` — that wraps the messy details of "follow the next pointer until you hit nullptr" inside the four operators every C++ programmer already knows: `*`, `->`, `++`, `==`. Once that type exists, `for (const auto& event : hero.eventLog)` walks the chain. `std::find(eventLog.begin(), eventLog.end(), needle)` searches it. And `printLog` collapses from two near-identical functions into one templated function that takes a begin and an end and does not care which container they came from.

You will learn three things this week, and they sit on top of each other:

1. **What an iterator IS.** A small object that points at a position. Five operations define a *forward iterator*: dereference (`*it` returns the element), member access (`it->name`), advance (`++it` moves to the next position), and equality / inequality (`it == end` tells you when to stop). That is the entire contract. Implement those five, and the standard library — `std::find`, `std::copy`, range-based `for`, everything — works on your container without knowing what your container is.
2. **`begin()` and `end()`.** The container hands you two iterators: one at the first position, one *one past the last*. That "one past the last" is a sentinel — you never dereference it. The loop runs while `it != end`, advancing each step. Range-based `for` is sugar for exactly this.
3. **Iterator categories.** Not all iterators are equal. A `Chain<T>::iterator` can only move forward (`++`) — it is a **forward iterator**. A `Bag<T>::iterator` (which is really a `std::vector::iterator` underneath) can move forward, backward, and jump by an integer — it is a **random-access iterator**. `std::find` works on both. `std::sort` requires random access. When you try `std::sort(eventLog.begin(), eventLog.end())` the compiler will tell you, in a wall of templates, that you've asked the wrong question.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/iterator-as-lens.svg' | relative_url }}" alt="A hand holds a small circular glass lens on a brass chain. Behind the lens, dimmed and out of focus, sits a row of three boxes connected by arrows — the container — and below that, a row of cells with indices — another container. Arrows from the lens show it moving rightward step by step. Two captions: 'advance' on the lens's motion, 'stop' on a small marker beyond the last cell of either row."/>
  <figcaption>The iterator is a lens. It knows how to advance (<code>++it</code>) and where to stop (<code>it == end</code>). It does <em>not</em> know whether the shape behind it is a chain or a bag — and the code that uses it does not need to know either.</figcaption>
</figure>

<figure class="diagram">
  <img src="{{ '/assets/diagrams/iterator-categories.svg' | relative_url }}" alt="A five-rung ladder. From bottom to top: Input (read once, advance); Output (write once, advance); Forward (multi-pass, advance only); Bidirectional (advance and retreat); Random-access (jump by integer in O(1)). Beside the ladder, a column lists which standard algorithms unlock at each rung: std::find at Input/Forward; std::reverse at Bidirectional; std::sort at Random-access."/>
  <figcaption>Iterator categories — each rung adds an operation. <code>Chain&lt;T&gt;::iterator</code> reaches the Forward rung (or Bidirectional, with <code>prev</code>). <code>Bag&lt;T&gt;::iterator</code> reaches Random-access. <code>std::find</code> needs almost nothing; <code>std::sort</code> needs the top rung.</figcaption>
</figure>

`Chain<T>` already has `begin()`-shaped behavior under another name — `head()` is an iterator-like handle. This week you wrap it in a real iterator type and finally retire the hand-walking.

## Objectives

By the end of Floor 5 you will be able to:

- Define a nested `iterator` type inside a container template and implement the five operations of a forward iterator (`*`, `->`, `++`, `==`, `!=`).
- Define a separate `const_iterator` and explain *why* — what `const Chain<T>&` would otherwise let you do that it shouldn't.
- Wire `begin()`, `end()`, `cbegin()`, `cend()` on `Chain<T>` and walk a chain with `for (const auto& s : chain) ...`.
- Replace `printLog` and `printLogOldest` with a *single* templated `printLog(Begin, End, label)` that does not know whether it's walking a chain or a bag.
- Use `std::find` and `std::find_if` against `Chain<std::string>`, `Bag<Item>`, and `Bag<Monster>` from one source line each.
- Read a `std::sort` template error against a forward iterator and name the missing operation it asked for.
- State the five iterator categories in order and identify which one your container's iterator belongs to.

## Pre-class

### Reading (ZyBook Ch. 6, continued — the iterator sections)

**The whole of Chapter 6 was assigned back on Floor 4** — this week we reach its iterator material (§6.14). If you read the chapter through on Floor 4, this is review; if not, read the rest now. The pointers below are what *this week's* class leans on.

Before **Monday**: §6.14.1 What an iterator is; §6.14.2 Defining `begin` and `end` on a custom container
Before **Wednesday**: §6.14.3 Iterator categories — Input through Random-access; §6.14.5 Using `std::find` and `std::find_if` with custom iterators
Before **Friday**: §6.14.6 Const iterators — why a separate type; skim §6.14.7 (L-values vs R-values — we'll touch this without dwelling)

Watch the §6.14.1 animation that draws the iterator as an arrow advancing across the cells. Then immediately compare the same animation on a vector versus a list — same arrow, different storage. That is the abstraction you're building.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | The five operations + `begin`/`end` | Live-code `Chain<T>::iterator` from the starter declaration: a single `Node* p_` member, then `operator*`, `operator->`, `operator++` (pre- and post-), and `operator==` / `!=`. Wire `begin()` and `end()` on `Chain`. Write `for (const auto& s : hero.eventLog) std::cout << s << "\n";` on the projector — students see it Just Work. |
| **W** | `const_iterator` and `std::find`     | Live-code `Chain<T>::const_iterator`. Discussion: why two types, why not one with `const T*`. Then rewrite `printLog` as a single templated function and delete `printLogOldest`. Then call `std::find(chain.begin(), chain.end(), needle)` on the event log to find a past command — one source line, works because the iterator satisfies the contract. |
| **F** | Iterator categories                  | Try `std::sort(chain.begin(), chain.end())`. Read the compiler error together — locate the line that names `random_access_iterator_tag`. Then `std::sort(inventory.begin(), inventory.end())` works (the Bag iterator delegates to the vector's random-access iterator). Discuss what category buys what. |

## The project — Floor 5

This week's project increment is **one `search` command that walks three containers without caring which**.

You will receive (in your starter drop):

- Everything through Floor 4½, fully working — `Bag<T>`, `Chain<T>` (doubly-linked, full Rule of Three), `BagException`, `findByName<T>`, every benchmark, the boss battle, `clone hero`.
- An upgraded `hero/Chain.h` with **stubbed nested types** — `iterator`, `const_iterator` — and stubbed `begin()`, `end()`, `cbegin()`, `cend()` member functions. The TODOs walk you through each operation in order.
- A `hero/Hero.h` whose `printLog` is now a function template: `template <typename It> void printLog(It first, It last, const char* label)`. The Floor 4½ `printLogOldest` is gone — `main.cpp` now calls `printLog(eventLog.rbegin(), eventLog.rend(), "oldest first")` for the backward walk. (You will write `rbegin()`/`rend()` on Wednesday using a small reverse-iterator wrapper provided in the starter; the implementation is short.)
- A `main.cpp` with a new `search` command behavior: one call to `findByName` on the bestiary, one on the inventory, one on the event log — all using the *same* templated function, which itself uses `std::find_if` internally.
- A new `selftest iterator` harness that builds a `Chain<int>`, calls `std::find`, `std::distance`, and range-based-`for` on it, and reports pass/fail.

You will write:

1. **Monday:** `Chain<T>::iterator` — `operator*`, `operator->`, `operator++` (pre- and post-), `operator==`, `operator!=`. Then `begin()` and `end()` on `Chain`. Convert `printLog`'s body to use range-based `for` (or explicit `it != end`).
2. **Wednesday:** `Chain<T>::const_iterator`. Then `cbegin()` / `cend()`. Then rewrite `findByName<T>` to call `std::find_if` internally (one line). Then call it from `search` against all three containers.
3. **Friday:** Iterator categories. Try `std::sort` on a `Chain` iterator — paste the compiler error into lab notes. Then try `std::reverse(chain.begin(), chain.end())` — also fails (needs bidirectional). Then implement bidirectional support by adding `operator--` to your iterator and watch `std::reverse` start working. Finally, run `selftest iterator` and confirm.

Demo target (Friday):

```
> search Goblin
  Goblin            HP 8   ATK 2   weakness: fire
  (found in bestiary)
> search "Iron key"
  Iron key          (wt 0.1, val 0)
  (found in inventory)
> search "inspect 99"
  inspect 99 — failed (index out of bounds)
  (found in event log, entry #4 from the top)
> log
   1.  search "inspect 99" — found in event log
   2.  search "Iron key" — found in inventory
   3.  search Goblin — found in bestiary
   ...
> selftest iterator
  Chain<int>::iterator: range-for OK   std::find OK   std::distance OK
  Chain<int>::const_iterator: range-for OK on const ref   OK
  std::sort on Chain iterator: refused at compile time as expected (Friday lab)
  std::reverse on Chain iterator: OK   (you implemented operator--)
> quit
  The lens dims. The lens does not remember what it saw — only how it moved.
```

### Lab 5 — *One Lens, Three Shelves* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-05/lab-notes.md` to your project repo with:

1. A transcript of the demo above.
2. **Plant the bug.** In `operator++`, advance `p_ = p_->prev` instead of `p_ = p_->next`. Build, run `log`, paste what happens. Restore.
3. **Skip `end()`.** Change `end()` to return an iterator wrapping `head_` instead of `nullptr`. Run a range-based `for` over a non-empty chain. Paste what happens. Restore. In one sentence — what invariant of the iterator contract did the broken `end()` violate?
4. **The `std::sort` error.** Paste the full compiler error from `std::sort(chain.begin(), chain.end())`. Find and quote the line that names an iterator category requirement. In one sentence: which operation is the standard library asking for that your iterator doesn't provide?
5. **`auto` vs spelled-out types.** Rewrite one of your range-based loops using the spelled-out iterator type — `Chain<std::string>::const_iterator` — instead of `const auto&`. Both compile. In two sentences: which version is easier to maintain when you later change the container type, and why?
6. **One-paragraph reflection.** You wrote two versions of `printLog` on Floor 4½ — one walking forward, one walking backward — and they were structurally identical except for three substitutions. This week they collapsed into one function. State, in your own words, what abstraction the iterator type provides that lets that collapse happen.

Your commit history this week should show at least three commits — Mon (iterator + begin/end), Wed (const_iterator + std::find_if), Fri (categories + operator-- + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist offers yet another shortcut</span>

*Iterators!* Friend, who needs them? **Indexing works on EVERYTHING.** Just write `for (int i = 0; i < chain.size(); ++i) { chain[i]; }` — same thing, faster to type, looks just like vector code! Grix uses indexing on chains all the time. Programs run fine! Until — well, you'll see when the chain gets long. *Strange how it slows down. Must be the weather.*
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 5</p>

**The Quadratic Mire** — *HP: grows with the size of your container. Damage: invisible at small N, ruinous at large N.*
Strikes when a programmer reaches for indexed access on a container that doesn't support it cheaply. `chain[i]` on a linked chain is O(i) — to reach index `i` you walk from the head `i` times. Put that inside a `for (i = 0; i < n; ++i) chain[i]`, and the whole loop is O(n²) — the chain is walked once for index 0, twice for index 1, three times for index 2, and so on. On a 100-element chain you do 5,000 walks. On a 10,000-element chain you do 50,000,000. The program does not crash. It just gets slower in a way that scales with no obvious cause.

Counter by:

- **Use the iterator.** `for (auto it = chain.begin(); it != chain.end(); ++it)` is O(n) total — each `++it` is O(1), and you visit each node exactly once. The same loop on a `Bag<T>` is also O(n). Same code, same cost. That is the contract.
- **Range-based `for`** is iterator-based under the hood. `for (const auto& x : chain)` is the right default; reach for explicit iterators only when you need them (deletion, partial walks, comparing positions).
- **Read the iterator category before you reach for `[i]`.** Random-access iterators (`Bag`, `std::vector`, `std::array`, `std::deque` with caveats) make `[i]` cheap. Forward and bidirectional iterators (`Chain`, `std::list`, `std::forward_list`) do not. If a container's iterator can't even *be* random-access, indexing into it is a sign you're paying a hidden tax.
</div>

<div class="callout check" markdown="1">
<p class="callout-title">Check for understanding</p>

Before you descend to Floor 6, you should be able to answer these without looking:

1. Name the five operations a forward iterator must support. Which two are *invariants of position* (asking the iterator about itself) and which three are *acts of looking or moving*?
2. `end()` returns an iterator that points *one past the last element*. Why not just have it return an iterator that points *at the last element*? What does the off-by-one convention buy you in your loops?
3. Range-based `for` — `for (const auto& x : chain)` — is sugar. Write the equivalent explicit `for` loop with `begin()`, `end()`, and `++it`. Which operations of the iterator did you have to call by name?
4. `Chain<T>::iterator` is a forward iterator. `Bag<T>::iterator` is random-access. `std::find` works on both. `std::sort` works only on the latter. Why does `std::find` need so much less? What does it actually do with the iterator?
5. Your container has a `const_iterator` separate from `iterator`. What does the existence of the separate type *prevent* a caller from doing through a `const Chain<T>&`?
6. The Quadratic Mire bestiary entry. Translate it into Big-O: what is the cost of `for (i = 0; i < n; ++i) chain[i]` on a linked chain? On a `Bag<T>`? Why are they different?

Answers are discussed in the Monday Floor 6 warmup.
</div>

---

*Sister Vael turns the lens between her fingers without lifting it. The lens does not change its shape for any container; it never will. The corridor narrows again, descending. From below comes the sound of stacked plates settling — many of them, balanced one upon another. The stair to Floor 6 — the **Stacks** — opens.*
