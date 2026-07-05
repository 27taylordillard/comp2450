# Trial I — *The Trees Brief*

*Warden of the Trees · Midterm 3 · 15 %*

Four short answers, one for each hall above. **50–100 words per answer.** Floors 9
and 9½ are **merged into a single answer** (Q2) — the search tree and its balance are
one idea here. No code on this trial except the one-line slot in Q1; just clear thinking.

AI is welcome to *check* your answers; it is not welcome to *write your sentences*. These
answers are short enough that an LLM voice is recognisable.

---

## 1. Floor 8 — trees / traversal / lineage

> Your menagerie marshals in the order `taxonomy.levelOrder()` returns; the Warden is
> exposed only when you trace a beast's full `lineage`.
>
> **(a)** Name the beast that steps forward **first**, and which beast steps first if you
> build the line from `preOrder()` **instead**.
> **(b)** In one sentence: why can a **general** tree answer `lineage(name)` at all, but
> not answer it *fast*?

```
(a) level-order (BFS) first beast: __________     pre-order (DFS) first beast: __________
```

(your answer — 50–100 words)

---

## 2. Floors 9 / 9½ (merged) — BST + balance

> Recall calls `catalogue.find` and costs an **extra hit** whenever `catalogue.isBalanced()`
> is false — and the real catalogue reports **height 6, balance factor −3, not balanced**.
>
> **(a)** Why does an unbalanced BST make `find` slower — tie it to **height**.
> **(b)** Why is the **whisper** free while **recall** is not?
> **(c)** Name the one property that would *guarantee* recall stays cheap, and why you
> nonetheless reach for the library / the hash table instead of hand-rolling it.

(your answer — 50–100 words)

---

## 3. Floor 10 — sets & maps

> To Ward a beast you call `registry.hasWeakness(w)` (a `std::set` membership) and
> `registry.countWeakness(w)` (a `std::map` lookup).
>
> **(a)** Why is the weakness arsenal a `std::set` and not a raw `std::vector` of the
> weakness column?
> **(b)** Why does `countWeakness` use `find()` and get declared **`const`**, instead of
> `weaknessCounts_[w]`? **Name the bug** the `const` prevents.

(your answer — 50–100 words)

---

## 4. Floor 11 — hash tables

> Whisper calls `whispers.find(name)` and is **free** — no retaliation — while Recall (the
> tree walk) costs a beat; and the Warden **banishes** one beast so whisper returns
> `nullptr` and you must recall it.
>
> **(a)** Name the **three computed steps** from a name to its record that make whisper
> average **O(1)**.
> **(b)** Why can you still recall the banished beast from the **catalogue tree** — what
> does "four independent indexes" mean?
> **(c)** What single thing turns your O(1) whisper back into an O(n) walk?

(your answer — 50–100 words)
