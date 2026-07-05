# Trial II — *The Critique*

*Warden of the Trees · Midterm 3 · 15 %*

This trial **requires** you to use an LLM. You will paste the prompt and the reply, then
critique what you got. You built exactly this structure by hand on Floor 11 (the Hall of
Whispers), and the floor named its traps out loud — so this is an audit of AI code against
**your own hands**.

---

## The prompt

Open Claude (or your LLM of choice) and paste **this exact prompt**:

> *Write a C++ class template `HashTable<K, V>` that stores key–value pairs using separate
> chaining (each bucket is a linked list). Support `insert(key, value)` (update in place if
> the key already exists), `get(key)` (return a pointer to the value or nullptr),
> `erase(key)`, and `size()`. Automatically double the number of buckets and rehash when
> the load factor exceeds 0.75. Use `std::hash<K>` for hashing. Make every operation average
> O(1), and briefly explain your design choices.*

Tell us which tool you used and the date:

- **Tool:** (e.g., Claude / ChatGPT / Copilot Chat / …)
- **Date of the conversation:** YYYY-MM-DD

---

## The LLM's reply

Paste the **full reply**, code and prose. Use a fenced code block for the code. Do not edit
the reply — we want to see what you actually got.

```cpp
// (paste the LLM's code here)
```

(paste any prose explanation here, verbatim)

---

## My critique  *( ≥ 150 words )*

Your critique must read like an engineer reviewing a colleague's pull request — specific,
technical, and naming exactly what the LLM did. A vague critique ("looks fine to me") is
*not* full credit even if the code was great. Full credit on the Engineering-judgement axis
requires catching the **rehash-mod trap** *or* the **compare-the-key trap** — the two that
map onto code you wrote by hand.

Address each of these:

- **Rehash correctness.** On a rehash, does it recompute each item's home as
  `hash(key) % newBucketCount`, or copy chains wholesale into the same index? The latter is
  *silently wrong* — the exact trap your Floor 11 rehash driver's comments named.
- **`get`/`find` compares the KEY, not just the bucket.** On a collision it must scan the
  chain comparing keys; returning the first item in the bucket is the bug.
- **`insert` updates in place vs. appends a duplicate.** Re-inserting an existing key should
  update, not double-count (your own `insert` returns `false` and updates).
- **`% 0` guard.** Does the constructor guarantee ≥ 1 bucket so the modulo never divides by
  zero? Yours does.
- **`std::hash<K>` requirements on `K`.** A key type with no `std::hash` specialization won't
  compile. Contrast your concrete multiplicative string hash.
- **"O(1)" is *average*, not worst-case.** A clumping hash or an un-triggered rehash decays
  to O(n). Did the prose overclaim without the load-factor caveat?
- **Memory / Rule of Three.** If the LLM hand-rolled raw-pointer buckets, is there a
  destructor and deep-copy, or a leak? Yours composes over `Chain<Monster>`, which owns its
  nodes. Did the LLM reinvent a leaky list instead of composing a safe one?
- **`erase` removes only the matching node**, leaving colliding neighbors intact (not
  clearing the whole bucket).
- **One concrete improvement.** Name one specific change you would make before merging — not
  "make it better," a concrete code change.

(your critique — at least 150 words)
