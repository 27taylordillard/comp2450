---
title: The Antechamber — Field Notes
permalink: /companion/floor-00/
---

# Field Notes · The Antechamber

*Read alongside ZyBook Chapter 1. Not instead of it.*

The ZyBook will give you the definitions, the animations, and the question sets, and you
are responsible for **the whole chapter**. What it won't give you is the reason any of it
matters to the game you're about to spend fifteen weeks building, or the translation from
the language you already speak. That's what these notes are for.

Class on Monday will be brief on this material precisely because it's written down here.
Come having read both.

---

## The three words the whole semester rides on

ZyBook §1.1 and §1.2 define these. Here they are again, side by side, because it's the
*contrast* that matters and a textbook has to introduce them one at a time.

| Term | What it is |
|---|---|
| **Data structure** | A specific way of organising data in memory. |
| **Algorithm** | A finite sequence of steps that solves a problem. |
| **Abstract data type (ADT)** | A *contract*: which operations exist, and what they promise. Not how they work. |

And the headline, which is worth committing to memory in exactly these words:

> **A data structure is the HOW. An ADT is the WHAT.**

You will build a lot of data structures with your own hands this semester. Every one of
them will be an implementation of one of about eight ADTs. Learn the eight nouns and you
have a map of the entire course.

### The example that makes it click

C++'s standard library contains three things called `std::vector`, `std::list`, and
`std::deque`.

From the outside they do the same job. All three hold an ordered sequence. All three let
you add items, remove items, and walk through them from front to back.

On the inside they are nothing alike. One is a single contiguous block of memory that gets
copied wholesale into a bigger block when it fills up. One is a chain of separately
allocated nodes scattered across memory, each pointing at the next. One is a set of fixed
chunks with a directory over the top, so it can grow at *both* ends cheaply.

> *"A list"* is what you want.
> `vector`, `list`, and `deque` are three different machines for getting it.

The ADT is the promise. The data structure is the machinery. Which machine you should pick
depends entirely on what you'll do with it most — and answering *that* question rigorously
is the subject of Floor 1.

### The trap

It is tempting — especially if you're coming from Python or Java — to decide that an ADT
is just a class.

It isn't. A class is one *way* to implement an ADT, and not even a necessary one. A C++
`struct` can implement an ADT. A Python list implements an ADT. A spreadsheet column
implements an ADT. Four people standing in a line at a coffee shop **are** a queue, with
no code within a hundred metres.

An ADT lives in your head and in your design notes. A data structure lives in your source
file. Keeping them separate is what lets you swap the machinery in week 9 without
rewriting the program around it.

---

## The eight

ZyBook §1.4 and §1.5 introduce these formally. This table is your reference card — one
line each, plus something you've actually touched.

| ADT | The promise | You already know it as |
|---|---|---|
| **List** | Ordered sequence, duplicates fine | A grocery list. Order matters; two cans of beans is legal. |
| **Stack** | LIFO — last in, first out | The browser back button. Most recent page, first to return to. |
| **Queue** | FIFO — first in, first out | The line at a coffee shop. |
| **Deque** | Add and remove at *both* ends | A line where regulars join the back and VIPs slip in the front. |
| **Bag** (multiset) | Unordered, duplicates fine | A coin jar. Shake it, draw one. |
| **Set** | Unordered, no duplicates, fast membership | The unique words in a poem. |
| **Priority queue** | Highest priority leaves next | A hospital emergency room. Severity beats arrival time. |
| **Dictionary** (map) | A key looks up a value | Your phone's contacts. Name in, number out. |

You will not implement all eight. You will meet all eight by name, and every structure you
*do* build will be one of them.

<div class="callout check" markdown="1">
<p class="callout-title">Check yourself before Monday</p>

Pick the best ADT for each. Answers at the bottom of this page — genuinely try first.

1. Plates on a cafeteria tray return.
2. The line at the cinema box office.
3. The unique words used in a poem.
4. Looking up a country's capital by the country's name.
5. The next five rooms the player will enter, in order.
6. *"Does the guild roster contain anyone called Theodric?"*
</div>

---

## Abstraction and encapsulation

ZyBook §1.3 covers the relationship between data structures and algorithms; these two ideas
are the discipline that keeps that relationship manageable. They get used
interchangeably in casual conversation. They're not the same thing, and the difference is
worth holding onto.

**Abstraction** is hiding the details of how something works behind a clean interface.

You drive a car by pressing a pedal. You do not think about fuel injectors or the engine
control unit. The pedal is the interface; everything behind it is hidden. When you write
`bestiary.push_back(monster);`, that single line may allocate a new block of memory, copy
every element you already had into it, free the old block, and only then store your
monster. You thought about none of it. You wrote one line.

**Encapsulation** is bundling data together with the operations that act on it, and
restricting outside access.

Consider the difference:

| Raw array | `std::vector` |
|---|---|
| `int* data` | `push_back()` |
| `int size` | `size()` |
| `int capacity` | `at(i)` |
| *you maintain all three, correctly, forever* | *the vector maintains them* |

In the raw-array world the pointer, the size, and the capacity are three loose sheets of
paper on your desk. Any function anywhere can pick one up, change it, and leave the other
two describing a world that no longer exists. That's not a hypothetical bug; it's the
single most common source of memory corruption in C.

In the vector world all three are sealed inside one object, and the only things you can do
to them are the operations the vector chooses to expose. The invalid states are not
guarded against — they're *unreachable*.

So: **abstraction hides; encapsulation protects.** Most well-designed types do both, which
is exactly why the two words get muddled.

---

## How fast does it grow?

ZyBook §1.6 is your reading here. Floor 1 gives this formal notation and rigour. For now
we want the intuition only, in English:

| Pattern | Double the input, and the work… | Looks like |
|---|---|---|
| **Linear** | …doubles | Checking every chest in the room |
| **Logarithmic** | …grows by a tiny constant | Halving the search range each step |
| **Quadratic** | …quadruples | Comparing every pair of things |

Read that table out loud. It reappears on every floor for the rest of the semester.

The reason this is *the* question in this course, rather than a footnote about speed, is
that the differences don't stay small. Consider searching a sorted list of a million items:

- Linear: up to **1,000,000** checks.
- Logarithmic: **20**.

Not twenty percent better. Fifty thousand times better — and the gap widens as the dungeon
grows. On Friday we'll run a program that does the same arithmetic two different ways and
watch one of them take about **ten thousand times** longer than the other to reach an
identical answer. Same result, wildly different amount of work.

That's the lesson, and it's why "which data structure?" is a real question and not a
matter of taste.

<div class="callout lore" markdown="1">
<p class="callout-title">What "efficient" does not mean</p>

Efficient does **not** mean the cleverest code. It does **not** mean the shortest code.

Efficient means *the code whose cost grows slowly as the input grows*. A clumsy algorithm
that doubles when the input doubles will beat an elegant one that quadruples — every time,
once the input is big enough. And the input is always bigger than you planned for.
</div>

---

## If you're coming from Python

Most of what changes is punctuation and explicitness, not ideas.

| Python | C++ | What's actually different |
|---|---|---|
| `import x` | `#include <x>` | Angle brackets mean "the standard library" |
| *(no equivalent)* | `int main() { ... }` | Execution starts here. Mandatory. |
| `print("hi")` | `std::cout << "hi\n";` | `<<` sends the right side into the stream on the left |
| `name = "Aldric"` | `std::string name = "Aldric";` | You state the type. The compiler then guards it for you. |
| `items = []` | `std::vector<Monster> items;` | A vector holds exactly *one* type, and says which |
| `items.append(m)` | `items.push_back(m);` | Same operation, different name |
| `for m in items:` | `for (const auto& m : items)` | `auto` = infer the type; `&` = don't copy; `const` = don't modify |
| `@dataclass class M:` | `struct M { ... };` | A bundle of named fields |

The one genuinely new idea is that last row's `&`. In Python, names refer to objects and
nothing is ever silently copied. In C++, `for (auto m : items)` *does* copy every element
as it walks — quietly, and possibly expensively. Writing `const auto&` says "let me look at
it where it lies, and I promise not to touch it." It's the normal way to read a container,
and you'll type it several hundred times this semester.

---

## Before you walk in

You should arrive Monday able to answer, in one sentence each:

1. What is the difference between a data structure and an ADT?
2. Name three ADTs and a situation where each is the right tool.
3. Why can two implementations of the same ADT have wildly different performance?

I will cold-call. It's low stakes and it's how we make sure the vocabulary is actually
shared before we start building on it.

You should also arrive with a working toolchain — see
[Arming the Adventurer]({{ site.baseurl }}/setup/visual-studio/) — though if the Compile
Demon has you, come anyway. Monday has time set aside for exactly that fight.

---

<div class="callout check" markdown="1">
<p class="callout-title">Answers</p>

1. **Stack** — you take the top plate.
2. **Queue** — first in line, first served.
3. **Set** — the trap is "list"; a poem *is* a sequence of words, but the question said
   *unique*, so duplicates collapse.
4. **Dictionary/map** — key in, value out.
5. **Queue** — order is fixed and you consume from the front.
6. **Set** — you're asking about membership, not position or count.
</div>

---

*Vocabulary in hand? The [Antechamber]({{ site.baseurl }}/floors/floor-00/) is open.*
