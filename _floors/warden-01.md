---
title: The Warden of the Foundations
floor_number: "Warden · I"
flavor_quote: "Four floors are carved into stone behind you. The Warden was carved from the same stone, by the same hand."
covers: "Floors 0–3 · Midterm 1"
---

## The gate

Steil's forge has gone cold behind you. Ahead, a vaulted antechamber — empty but for a single shape standing at its centre, and a closed gate beyond it. On the far side of the gate, you can hear Foust's cart-wheel; Floor 4 is on the other side.

The shape does not move. It has no face you can name. It is the **Warden of the Foundations** — the first of four — and it does not bring a fifth lesson. It brings every one of the four above, and asks whether they took.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/warden-01.svg' | relative_url }}" alt="The Warden of the Foundations — a hooded, faceless figure in dark iron plate, only two ember-gold eye-points visible inside the deep cowl. Four small sigils on the breastplate: hourglass, magnifying glass, forge flame, anvil. Nameplate blank."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">The Warden, who has no name to give</span>

*Speak the names. Walk the path. Pivot, and explain. Stamp the mould.*
*Pass the four — and pass.*
</div>
</div>

**This page is your Midterm 1.** It replaces the timed paper exam other classes give for Floors 0 – 3. It is in two parts: a **take-home gauntlet of four trials** that you do over the long weekend, committed to your project repo; and a short **in-class quiz on Friday** that ties your submission to your own hand. Together they are 100 % of the midterm.

<div class="callout check" markdown="1">
<p class="callout-title">The schedule</p>

| Phase | When | What |
|-------|------|------|
| **Gauntlet opens** | Friday, end of Week 4 — after Floor 3's last class | The four trials below become available. Begin any time. |
| **Gauntlet closes** | Friday of Week 5, **9:00 AM** | All artifacts committed and pushed to your project repo. |
| **In-class quiz** | Friday of Week 5, during class | ~20-minute paper quiz tied to your submission. |
| **Grades returned** | Following Wednesday | Per-trial rubric notes + total. |

You have a full week of calendar time. Plan to spend roughly an afternoon on the take-home if you are ready, and several afternoons if you are not.
</div>

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/faith.svg' | relative_url }}" alt="Brother Faith, Abbot of the Sorted Tome — bearded monk in a dark brown robe with a brass clasp and a stopwatch on a brass cord."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Brother Faith stands with you at the gate</span>

I have shown you the stopwatch all term. I am not going to make you race a paper exam under it now. The trials below are slower and more honest. You will have your books, your editor, your machine, and your tools — including the ones the Order did not have when I made my own descent. Use them. **Tell me what you used.** And then sit in front of me Friday and walk me through what you did.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">AI policy for this midterm — read once, follow always</p>

You may use AI assistants (Claude, Copilot, ChatGPT, the in-IDE completion in your editor — anything) on every trial below. Trial IV **requires** you to use one. The rules are:

1. **Declare what you used, where.** Submit an `AI-USE.md` in your gauntlet folder. For each trial, write one or two lines: *which* tool, *what* you asked it for, *what* you did with the result. "Did not use" is a perfectly good entry — but the line must be there.
2. **The artifact must survive a conversation about it.** The Friday quiz will ask you about your submission in your own handwriting. If you turned in code or a paragraph you cannot defend, you submitted it from the wrong author.
3. **Submitting AI output as-is, undeclared, is the failure mode** — not the AI use itself. If your code came from an LLM and you read it, ran it, fixed what was wrong, and understand it line-by-line, that is *engineering*. If you pasted it without reading and shipped it, that is *not*.
4. **Trial IV is explicit:** you will use an LLM, paste the prompt and response, and write a critique. Reading and judging AI output is now part of the job; we are practicing it on purpose.

This policy is the same one your future employer will write next year. Practice now.
</div>

## The four trials

Run them in order. Each trial says what to **do**, what to **submit** (in your repo), and what counts as full credit.

### Trial I — *The Naming* (Floor 0) — 15 %

The Warden offers you a pile of loot from the four floors above and asks: *what shape does each thing want to live in?*

**Do.** Open `floor-03/warden-trials/trial-1-naming.md` in your project repo.

1. Write **five short scenarios in your own words** — each one a real-world or in-game situation that needs a container. The scenarios should be **specific to your life or to your dungeon project**, not generic textbook examples. *"The order of the last ten commands the player typed in my game"* is good. *"A list of things"* is not.
2. For each scenario, name the **one ADT** that fits best (list, stack, queue, deque, bag, set, priority queue, dictionary). Justify in **one sentence** why that ADT and not its closest neighbour.
3. Now invert: pick three ADTs you did *not* use, and for each invent a fresh scenario from your project that demands it.

**Submit.** `trial-1-naming.md`.

**Full credit.** Eight scenarios in your own voice; eight ADT pairings, each defended in one tight sentence; the scenarios reference your hero, your inventory, your bestiary, your commands — not generic stock. AI is welcome to *check* your pairings; it is not welcome to *write your scenarios* — those have to be from your world.

### Trial II — *The Sorted Path* (Floor 1) — 25 %

**Do.** Open `floor-03/warden-trials/trial-2-search/searches.cpp` (a fresh file — no copy-paste from Floor 1's starter).

1. Write `int linearSearch(const std::vector<int>&, int target)` from scratch. Compile. Test on a small vector.
2. Write `int binarySearchIter(const std::vector<int>&, int target)`. Compile. Test on the same vector after sorting it.
3. Write `int binarySearchRec(const std::vector<int>&, int target, int lo, int hi)`. Compile. Test.
4. For each of the three, put a one-line comment above the function with its **Big-O** and its **precondition** (or "none").
5. Take a vector you know is **not sorted**, and call `binarySearchIter` on a value you know is in it. In `trial-2-search/notes.md`, write **one short paragraph** describing what you observed and why it is dangerous.

**Submit.** `searches.cpp` and `notes.md`.

**Full credit.** All three searches compile and pass your tests. Big-O and precondition comments present. The unsorted-binary paragraph is in your own voice and names the precondition explicitly.

### Trial III — *The Pivot* (Floor 2) — 25 %

This trial is on **paper**, not in code. The Warden specifically wants to see your hand.

**Do.**

1. Trace **merge sort** on `[5, 2, 8, 1, 9, 3, 7, 4]`. Show every split and every merge. Count comparisons during merging.
2. Trace **quicksort** on `[5, 2, 8, 1, 9, 3, 7, 4]` using the **last** element of each partition as pivot. Show every partition. Count comparisons.
3. Trace **quicksort** on `[1, 2, 3, 4, 5, 6, 7, 8]` using the **first** element of each partition as pivot. Show every partition. Count comparisons.
4. Compare the three counts. In `trial-3-pivot/paragraph.md`, write **three to four sentences** explaining what happened in step 3 and why, in language a friend who hasn't read Floor 2 could follow. The paragraph must use the words **pivot** and **already**.

**Submit.** A photo or scan of all three traces (`trial-3-pivot/traces.pdf` or `traces.jpg`) **and** `paragraph.md`.

**Full credit.** Three legible traces in **your handwriting**; comparison counts circled; the paragraph short, clear, mechanically correct, and uses both required words. AI cannot do this trial for you — the artifact is a photograph of paper, and we will be reading what your hand wrote.

### Trial IV — *The Stamped Mould* (Floor 3) — 25 %

This trial **requires** AI use. You will engage an LLM, save the conversation, and critique what it gave you.

**Do, part A — your own code.** In `floor-03/warden-trials/trial-4-templates/templates.cpp`:

1. Write a function template `template<typename T> T sumAll(const std::vector<T>& v)` that returns the sum of the elements. Compile and call it on a `vector<int>` and a `vector<double>`.
2. Now call it on a `vector<std::string>`. Predict what will happen *before* you compile. Compile. Note in `notes.md` whether your prediction matched.
3. Write `class BagException : public std::exception` with a `what()` override returning a useful message.
4. Write a `main` that creates a small `std::vector<int>`, calls `v.at(99)` inside a `try` block, catches `const std::exception& e`, and prints `e.what()`. Confirm the program exits cleanly without crashing.
5. Change `at(99)` to `operator[](99)`. Run again. Note in `notes.md` what changed.

**Do, part B — engage and critique an LLM.** In `trial-4-templates/ai-critique.md`:

1. Open Claude (or your LLM of choice). Paste this **exact** prompt:
   > *Write a C++ function template that returns the largest element of a `std::vector<T>`. Handle the case where the vector is empty. Briefly explain your design choices.*
2. Paste the prompt **and** the LLM's full reply into `ai-critique.md`.
3. Below the reply, write a critique of **at least 200 words**. The critique must address:
   - Does the code compile? Did you try it?
   - What does the empty-vector case actually do? Is the LLM's choice reasonable?
   - What requirements does this place on `T`? What would happen if you instantiated it with a type that doesn't support `<`?
   - Is it const-correct? By value or by reference? Would you change either?
   - Did the LLM cheat by using `std::max_element`, or did it write the loop? Which would you have shipped, and why?
   - One concrete improvement you would make before merging this into a real project.

**Submit.** `templates.cpp`, `notes.md`, `ai-critique.md`.

**Full credit.** Code from part A compiles and behaves correctly. The critique in part B is **substantive and specific** — it names what the LLM did, identifies real issues (or argues there are none, with reasoning), and reads like an engineer evaluating a colleague's pull request. A vague critique ("looks fine to me, but it could be better") is *not* full credit even if the code is great.

## The Warden's Question — Friday in-class quiz (10 %)

Twenty minutes. Closed everything — no books, no laptops, no phones, no AI. You will receive **four short questions on paper, one per trial.** Each one will look something like *"In your Trial II submission, you wrote `binarySearchRec`. On paper, modify it to count and return the number of probes it took"* — a small extension of the work you already submitted. If you actually did the trials, this quiz takes ten minutes. If you didn't, it takes the full twenty and the gap shows.

The quiz is the integrity seal. It is short by design and intentionally tied to your own submission — not because we don't trust you, but because by Friday you will know your trials cold, and a quick conversation in your own handwriting is the cleanest proof of that. There is no curve; there is no surprise material.

## Submission and grading

**Submission.** Commit and push the entire `floor-03/warden-trials/` folder to your project repo by **9:00 AM Friday of Week 5**. Late commits drop one letter grade per day on the take-home portion (the in-class quiz is on its scheduled day or it is a zero — same as any in-class quiz).

**Folder shape:**

```
floor-03/warden-trials/
  AI-USE.md
  trial-1-naming.md
  trial-2-search/
    searches.cpp
    notes.md
  trial-3-pivot/
    traces.pdf            (or .jpg / .png — must be legible)
    paragraph.md
  trial-4-templates/
    templates.cpp
    notes.md
    ai-critique.md
```

**Rubric.**

| Component | Weight | Full-credit shape |
|-----------|--------|-------------------|
| Trial I — The Naming           | 15 % | 8 scenarios in your voice, defended in one sentence each, drawn from your project |
| Trial II — The Sorted Path     | 25 % | 3 working searches; Big-O + precondition comments; unsorted-binary paragraph |
| Trial III — The Pivot          | 25 % | 3 hand-traces, counts circled, 3–4 sentence explanatory paragraph using *pivot* and *already* |
| Trial IV — The Stamped Mould   | 25 % | Templates + exceptions code working; ≥200-word substantive critique of the LLM reply |
| Friday in-class quiz           | 10 % | Four short answers in your own hand on extensions of your trials |

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/steil.svg' | relative_url }}" alt="Forgemaster Steil, Master of the Cold Forge — long brown hair streaked with gray, heavy beard, dark leather apron with brass anvil pin over a rust-red tunic."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Steil, leaning on his hammer at the gate</span>

When the compiler shouts at you on Trial IV — *good.* The wall of errors is the compiler showing its work. One line in it tells you what is wrong; the rest is scaffolding it walked through to find that line. The same is true of your AI: read its first claim, judge that, and only then read what it built on top of it. If you accept the first claim without judging it, the AI has not helped you — it has *replaced* you, and the Warden will know.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Warden of the Foundations</p>

**The Warden of the Foundations** — *HP: equal to your weakest floor. Damage: cumulative.*
The first of four wardens. Cataloguers caught its silhouette but never its name; the page they pinned to its entry shows armour, four sigils, and two ember points where eyes should be. Below the figure, the name slot is struck through. The Warden fights at the strength of whichever floor you understand *least* — a hero who is strong on three floors and weak on one is a hero the Warden picks apart at the seam.

The Warden is also the first creature in this dungeon that does not punish you for using your tools. It punishes you for **letting your tools punish your understanding**. A hero who reads what their AI wrote, ran it, broke it, fixed it, and understood the fix passes the Warden. A hero who pasted and shipped fails it — even if the code compiled.
</div>

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix has a study tip</span>

*Friend!* The trials — easy! Just ask the **machine** to do all four! Paste, paste, paste, *commit, push.* Don't fill in the AI form! No one will check, friend! And when they ask you on Friday — just say the machine *was* you. Friend? *Friend?* (Grix has tried this. Grix did not pass.)
</div>
</div>

---

*The Warden does not move when you finish. It will not say whether you have passed. The judgement happens on Friday, in handwriting, in front of the only person whose opinion of your work matters this term — yourself. Make it a good handwriting day. Beyond the gate, the stair to Floor 4 — the **Chain Vault** — descends into the dark. Foust is already there with his cart, sorting maps.*
