---
title: The Antechamber
floor_number: Floor 0
order: 10
flavor_quote: "The gate is open. Before you descend, you must learn the names of the things that hunt in the dark."
week: 1
zybook: "Chapter 1 — full chapter"
---

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/faith.svg' | relative_url }}" alt="Brother Faith, Abbot of the Sorted Tome — bearded monk in a dark brown robe with a brass clasp and a stopwatch on a brass cord."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">The Abbot greets you at the gate</span>

The Antechamber. You will not be harmed here. Take this week and learn the names of the things below — *list, stack, queue, set, map.* The dungeon will not wait for your vocabulary.
</div>
</div>

## The briefing

You stand in the **Antechamber** — the first room of the dungeon. Nothing threatens you here yet. Before you descend, you must learn the **language of the dungeon**: the names of the things that adventurers carry, the things they fight, and the way their efforts are measured.

This week, you learn three vocabularies:

1. **Abstract Data Types (ADTs)** — the *interfaces* your adventurers use to organize loot, spells, and enemies. Every tool you'll carry this semester is an instance of one of these.
2. **Abstraction & encapsulation** — the *discipline* of keeping implementation details hidden behind those interfaces.
3. **Algorithm efficiency** — an intuition for how *costly* an operation is as the dungeon grows. (Full Big-O follows on Floor 1.)

You will not implement anything new this week. You are being given a map of what's coming.

## Objectives

By the end of Floor 0 you will be able to:

- Define *data structure*, *algorithm*, and *abstract data type* in your own words.
- Name the common ADTs (list, stack, queue, deque, bag, set, priority queue, dictionary) and describe what operations each supports.
- Explain why two implementations of the same ADT can have very different performance.
- Estimate, informally, whether an algorithm's cost grows *slowly* or *quickly* as input grows.
- Compile and run a C++ program on your machine (confirming your dev environment works).

## Pre-class

### Before the first class: arm yourself

Three tools, one afternoon. Do this before Monday if you possibly can — everything else
this semester assumes it.

- **[Arming the Adventurer]({{ site.baseurl }}/setup/visual-studio/)** — install Visual Studio and the C++ compiler, and find the one checkbox everybody misses.
- **[The Chronicle]({{ site.baseurl }}/setup/git/)** — fork the course repository, install GitHub Desktop, and learn the commit-and-push rhythm you'll use every week.
- **[Why the Forge Needs a Blueprint]({{ site.baseurl }}/setup/cmake/)** — what CMake is and why we build every floor with it. Read this one for understanding, not for steps.

If the Compile Demon defeats you, come to Monday's class anyway. There is time set aside
for exactly that fight — but arrive having *tried*.

### Reading (ZyBook Ch. 1 — the whole chapter)

**Read all of Chapter 1.** The reading assignment is the entire chapter, even the parts class doesn't cover directly. The day-by-day pointers below are the sections *class time* leans on — not the boundary of what you're responsible for.

Before **Monday**: §1.1 Data structures, §1.2 Introduction to algorithms
Before **Wednesday**: §1.3 Relation between data structures and algorithms, §1.4 Abstract data types, §1.5 Applications of ADTs
Before **Friday**: §1.6 Algorithm efficiency

Work the Question Sets and Animations inside the ZyBook — they count toward your participation grade.

### Field Notes — the companion to the reading

**[The Antechamber · Field Notes]({{ site.baseurl }}/companion/floor-00/)** goes deeper than
class will on the vocabulary: the three terms, the eight ADTs, abstraction versus
encapsulation, growth rates in plain English, and a Python-to-C++ translation table. Read
it *alongside* Chapter 1 — it explains why the chapter's contents matter to the game you're
building, which is not the textbook's job.

Monday's class is deliberately brief on this material because it's written down there.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Course entry, what is a DS?     | Syllabus & dungeon tour; *Unplugged ADT* card activity; **dev environment triage** (Visual Studio, CMake, git) — anything unfinished must be done before Wednesday |
| **W** | Common ADTs; first C++ syntax   | *ADT Triage* — match scenarios to ADTs in pairs; live-code two tiny C++ scripts together (`hello.cpp`, `vector_demo.cpp`) so everyone has touched the compiler at least once |
| **F** | Algorithm efficiency, intuition | *Race the dungeon* — hand-trace two algorithms on inputs of size 10, 100, 1000; predict; then run a small `growth.cpp` sample together to see the numbers |

There are no pre-class videos this week. Class time is for setup, discussion, and live coding together.

## Lab — folded into the project

**Lab 0 — "First light."** No separate handout. The project task below *is* the lab: fork, build, run, change one string, commit. By Friday you should have a working compiler, your own fork cloned, and one commit pushed.

Deliverable: a commit pushed to **your fork**, with your own flavor line set in `main.cpp` and a commit message beginning `Floor 0: `.

## The project — Floor 0

This week's project increment is **environment-only**. You will not implement any data structures yet. What you will do:

1. **Fork** `HogRed/comp2450` to your own GitHub account, then clone *your fork* — see [The Chronicle]({{ site.baseurl }}/setup/git/). Cloning mine instead of your own is the classic week-1 mistake; you'll only find out when the push is refused.
2. Open `project/floor-00-starter` in Visual Studio (**File → Open → Folder**) and let CMake configure.
3. Run it with **Ctrl+F5**. It prompts for your hero's name and echoes a single line of dungeon flavor text.
4. Find the line marked `// TODO Floor 0:` in `main.cpp` and change the string constant so the flavor text is yours.
5. Commit with a message starting `Floor 0: `, then **push** — and check your GitHub page to confirm it arrived.

Why so little? Because environment setup — compilers, paths, build tools — is the single most common reason students fall behind in week 1. We flush out those gremlins before we need to fight real ones.

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 0</p>

**The Compile Demon** — *HP: your patience.*
Strikes when your code is fine but your *environment* isn't. Symptoms: opening the folder produces no CMake output at all, `No CMAKE_CXX_COMPILER could be found`, `'cout' was not declared`, a console window that flashes and vanishes, or linker errors with no line number. Counter by:

- Confirming **C++ CMake tools for Windows** is installed — the usual culprit.
- Opening the *starter folder*, not the repository root and not a single file.
- `#include <iostream>`, and `std::cout` (or `using namespace std;`).
- **Ctrl+F5**, never plain F5, or the window closes before you can read it.
- **Project → Delete Cache and Reconfigure** when mystery errors survive a rebuild.

The full field guide is in [Arming the Adventurer]({{ site.baseurl }}/setup/visual-studio/).
</div>

<div class="callout check" markdown="1">
<p class="callout-title">The Keeper's Check</p>

This floor ends the way every floor ends: with a short stack of paper questions in class — closed notes, closed laptop, answers in your own hand — drawn from everything this floor taught. If you did the reading, wrote the code, and can say *why* this week's work earns its place in the game, it takes ten minutes.
</div>

---

*The Antechamber is quiet. The stair to Floor 1 is marked in the map. Rest well.*
