# Trial I — *The Foundations Brief*

*Warden of the Foundations · Midterm 1 · 15 %*

Four short answers, one from each floor above. **50–100 words per answer.** No code on this trial (except the lambdas in Q3); just clear thinking.

AI is welcome to *check* your answers; it is not welcome to *write your sentences*. These answers are short enough that an LLM voice is recognisable.

---

## 1. Floor 0 — ADT

> Your battle's "Use item" menu shows the *currently usable* items in your hero's inventory on this turn. Name the right ADT for that menu. Defend the choice against its closest neighbour (e.g., why `bag` instead of `set`, or `list` instead of `bag`).

(your answer — 50–100 words):
I believe that the right ADT for the "Use item" menu would be a bag. The menu shows items that can be used from the hero's inventory. A bag would make sense because the items do not need to be in a specific order and there can be duplicates. For example, the hero could have more than one loaf of bread. A set would not work as well because duplicates are not allowed.  

---

## 2. Floor 1 — search & Big-O

> Your inventory is kept sorted by healing power (in this codebase an item's `value` measures its potency, so `value` plays the healing-power role). The player types `use Healing potion`. Linear or binary search to find it by name? Justify, and give the Big-O for each.

(your answer — 50–100 words):
I would use a linear search because the inventory is sorted by healing power, but the player is searching for the item by name. Linear searches one by one until the name is found. Its Big-O is O(n). Binary search would work better if the inventory was sorted by name instead of by value. The Big-O for binary is O(log n). 

---

## 3. Floor 2 — sort & comparators

> Your "Use item" menu must be displayable sorted *either* by healing power (meaning `value`) *or* by weight. Show a one-line comparator (lambda) for each. One sentence on what language feature makes one `std::sort` call serve both orders.

*Note: `Item` has no healing field — an item's `value` measures its potency, so `value` plays the healing-power role here and in your battle.*

```cpp
// by healing power — i.e. by value
auto byValue  = [](const Item& a, const Item& b) {
    return a.value < b.value;
};

// by weight
auto byWeight = [](const Item&a, const Item& b) {
    return a.weight < b.weight;
};
```

(one-sentence answer — what language feature?)
Lambdas makes one 'std::sort' call serve both orders because they allow us to change the comparsion rule.

---

## 4. Floor 3 — templates & exceptions

> Why does `Bag<T>` live in `Bag.h` instead of `Bag.cpp`? And: when the player types `9` for a 4-option menu, where in your code should the validation **throw**, and where should it **catch**?

(your answer — 50–100 words)
Bag<T>  lives in 'Bag.h' because it is a template, meaning that it needs to be in the header sp the compiler can see it when it creates the class for a specific type. When the player types '9', the validation should throw an exception when it checks to see if the number is valid. The validation shuld throw where the error is detected and catch where the error can be handled.
