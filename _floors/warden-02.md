---
title: The Warden of the Middle Gates
floor_number: "Warden · II"
flavor_quote: "Four floors taught you to hold the line. The Warden asks whether the line holds when more than one enemy arrives at once."
covers: "Floors 4 – 7 · Midterm 2"
---

## The gate

Beyond Steward Quill's desk a heavy door opens onto a marshalling yard. Torchlight on wet stone. Four figures stand at the corners, one at each compass point — **Mavren** with her chain-ledger, **Vael** with her lens lifted, **Resh** mouthing the last words spoken to him, **Quill** with his ticket in hand — and at the centre, in dark iron plate, the second Warden waits. Four sigils on its breastplate this time: a chain link, a lens, a coiled echo, a stamped ticket.

It does not move *yet*. It does not need to. Four floors built you a hero who can hold a line of *one*. The Warden of the Middle Gates asks whether the line holds when enemies arrive in waves, when actions need to be undone, when an inventory has to be walked one item at a time without anybody telling you the index.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/warden-02.svg' | relative_url }}" alt="The Warden of the Middle Gates — a hooded, faceless figure in dark iron plate. Where the first Warden showed two ember-gold eye-points inside the deep cowl, this one shows a single horizontal ember slit — the visor of a great-helm worn beneath the hood. Four small sigils on the breastplate: a chain link, a polished lens, a coiled echo, a stamped ticket. Behind the figure, a marshalling yard with four torch-lit corners."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">The Warden, who has no name to give</span>

*Four keepers stand with me. Each one will see whether their lesson took.*
*One adapter held; you faced one enemy. Now the line moves. Hold it.*
</div>
</div>

**This page is your Midterm 2.** It replaces the timed paper exam. Same shape as Warden 1: a **take-home gauntlet of three trials** committed to your project repo, the third of which is a working wave-survival encounter inside your dungeon app; and a short **in-class quiz on Friday** that ties your submission to your own hand. Together they are 100 % of the midterm.

<div class="callout check" markdown="1">
<p class="callout-title">The schedule</p>

| Phase | When | What |
|-------|------|------|
| **Gauntlet opens** | Friday, end of Week 9 — after Floor 7's last class | The three trials below become available. Begin any time. |
| **Gauntlet closes** | Friday of Week 10, **9:00 AM** | All artifacts and battle code committed and pushed to your project repo. |
| **In-class quiz** | Friday of Week 10, during class | ~20-minute paper quiz — every question pulled from *your own* battle code. |
| **Grades returned** | Following Wednesday | Per-trial rubric notes + total. |
</div>

Plan your week. Trials I and II can be knocked out in an evening each. Trial III — the wave-survival encounter — is real engineering work and will take you several focused sittings. Start it early. Floor 8 (Trees) opens the same week; do not let the new floor eat your Warden time.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/faith.svg' | relative_url }}" alt="Brother Faith, Abbot of the Sorted Tome — bearded monk in a dark brown robe with a brass clasp and a stopwatch on a brass cord."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Brother Faith stands with you at the gate</span>

Last midterm I asked you to *use* what four floors gave you against one Warden. This midterm I will ask you to *compose* what four floors gave you against more than one foe at a time. Chain. Walk. Echo. Line. If those four words live in your fingers by Friday, you will pass. If they live only in your notes, the marshalling yard will see it the moment the first wave arrives.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">AI policy — unchanged from Warden 1</p>

You may use AI assistants on every trial below. Trial II **requires** you to use one. The rules are the same four you read at the first gate, and you will sign the same `AI-USE.md`:

1. **Declare what you used, where.** Submit `AI-USE.md` in your `warden-2-trials/` folder. One or two lines per trial. "Did not use" is a fine entry; the line must be there.
2. **The artifact must survive a conversation about it.** Friday's quiz pulls questions from *your* battle code in your own handwriting.
3. **Submitting AI output as-is, undeclared, is the failure mode** — not the AI use itself.
4. **Trial II is explicit:** engage an LLM, paste the prompt and response, write a critique.
5. **Trial III adds one constraint:** in your `encounter-notes.md`, name **two functions** in your battle code that you wrote *without* AI assistance — by hand, with no autocomplete on the body. Friday's quiz can ask you to modify those on paper.

If you wrote a real Warden 1 submission, you already know this policy. Nothing new.
</div>

## The three trials

Run them in order. Trials I and II are short prep that prove your four containers are sound in your head. Trial III is where you put them together.

### Trial I — *The Keepers' Brief* (15 %)

Four short answers, each anchored to the encounter you are about to build. No code on this trial; just clear thinking on paper-or-keyboard. (Floors 4 and 4½ share a single answer below — the keepers do not separate the singly-linked chain from the doubly-linked one when they speak; neither will you.)

**Do.** Open `floor-07/warden-2-trials/trial-1-brief.md` and write **50–100 words per answer**:

1. **(Floors 4 / 4½ — linked lists.)** Your encounter's **event log** (every "Aric strikes Imp for 6", every "Imp retaliates for 3") is appended to `hero.eventLog`, which is a `Chain<std::string>`. Defend that against a `Bag<std::string>` (vector-backed) in terms of one operation the battle actually performs — naming the Big-O of that operation in `Chain` and in `Bag`. *Hint: think about what `Bag` has to do the moment its capacity is exceeded.*
2. **(Floor 5 — iterators.)** When the player picks the **Inspect log** menu option, your encounter calls `printLog(hero.eventLog.begin(), hero.eventLog.end(), 5, ...)` — the same function template you wrote on Floor 5. (Because `eventLog` is built with `push_front` everywhere in the program, the *front* of the chain is the most recent entry; the forward iterator walks newest-to-oldest naturally.) Show **one line** of C++ that does the equivalent walk by hand using an explicit `Chain<std::string>::iterator` loop. Then one sentence: why is this iterator *bidirectional* (and what becomes impossible the moment you ask for random-access on it)?
3. **(Floor 6 — stacks.)** Your `undo` command pops the most recent action off a `Stack<UndoAction>` and restores its snapshot. Defend the choice of stack against a queue or a deque in one sentence — referencing the *contract* of LIFO (not the implementation). Then one extra sentence: name one piece of battle state that your `undo` *intentionally does not* restore, and explain why the player won't mind.
4. **(Floor 7 — queues.)** Each wave's enemies sit in a `Queue<Enemy>` in arrival order. Explain — in one sentence — what *visibly* changes for the player if you swap `Queue<Enemy>` for `Stack<Enemy>`: which enemy gets attacked first, and in what order the rest are picked off.

**Submit.** `trial-1-brief.md`.

**Full credit.** Four answers, each tight and correct. AI is welcome to *check* your answers; AI cannot *write your sentences* — these are short enough that we will recognise the LLM voice.

### Trial II — *The Critique* (15 %)

You will engage an LLM, save the conversation, and critique what it gave you. This is the AI-literacy trial: reading and judging AI code is now part of the job. The prompt this time targets the adapter pattern you just wrote *twice* (Stack on Floor 6, Queue on Floor 7), and asks the LLM to write the third such adapter — which is a trap if you aren't paying attention.

**Do.** In `floor-07/warden-2-trials/trial-2-critique.md`:

1. Open Claude (or your LLM of choice). Paste this **exact** prompt:
   > *Write a C++ class template `Deque<T>` that wraps a doubly-linked list and supports `push_front`, `push_back`, `pop_front`, `pop_back`, `front`, `back`, `size`, and `empty`. Each method should be O(1). Briefly explain your design choices.*
2. Paste the prompt **and** the LLM's full reply into `trial-2-critique.md`.
3. Below the reply, write a critique of **at least 150 words** addressing:
   - Does the code compile? Did you try it?
   - Are all eight operations *actually* O(1) — or did the LLM quietly use `std::list::end()` increment chains, `std::advance`, or a singly-linked list dressed up as doubly-linked?
   - What does `pop_front()` or `pop_back()` do on an empty deque? Is that reasonable? Compare to what your own `Queue<T>::dequeue` does in your Floor 7 code.
   - Did the LLM compose the deque from your own `Chain<T>` (or `std::list`), or did it reimplement the linked list from scratch? Which would you have shipped, and why?
   - Const-correctness on `front` and `back` — did the LLM give you both overloads, only the non-const one, or neither?
   - One concrete improvement you would make before merging this into a real project.

**Submit.** `trial-2-critique.md`.

**Full credit.** A critique that reads like an engineer reviewing a colleague's pull request — specific, technical, and naming what the LLM did. A vague critique is *not* full credit even if the LLM's code was great.

### Trial III — *The Marshalling Yard* (60 %)

This is the trial. You will program a **wave-survival encounter** into your dungeon app — the Warden does not fight you directly; the Warden sends waves at you, and the waves are how the four floors get exercised at once.

**Do.** In your floor-07 project (the one you've been building through stacks and queues), add a **`battle waves N`** command to `main.cpp` — where `N` is the number of waves the player wants to face. Factor the encounter into `battle/Battle.cpp` (the file already exists from Warden 1; add a second function alongside `runWardenBattle`). When the player types `battle waves 3`, the encounter begins with three waves queued up.

**Required mechanics — minimum viable:**

- **Stats.** Player starts at 40 HP. Each enemy has its own HP (suggested: imp 8, rat 5, skeleton 12 — numbers are yours to tune; document them in `encounter-notes.md`).
- **Wave loop.** For each wave 1..N: enqueue a small group of enemies (suggested: wave *i* spawns *i + 1* enemies; pick the lineup however you like). The wave is over when its queue is empty. Then the next wave starts.
- **Turn loop within a wave.** Each turn: print current state (player HP, wave *i*/*N*, enemies remaining in this wave, the **front** enemy and its HP), present a numbered menu, read input, execute, repeat.
- **Menu — at least five options:**
  1. **Attack** — deal damage to the *front* enemy of the current wave. If its HP drops to 0 or below, dequeue it; the next-oldest enemy becomes the new front. If it survives, *it* retaliates this turn.
  2. **Use item** — display the inventory (which is still `hero.inventory`, a `Bag<Item>` — same as every prior floor) and prompt for an item name; apply the item's effect; turn ends. The front enemy retaliates after.
  3. **Inspect log** — print the last few entries of `hero.eventLog` in reverse-chronological order via the same `printLog` function template you wrote on Floor 5. **Free action** — does not consume a turn.
  4. **Undo** — reverse the most recent turn by popping the action stack and restoring its snapshot (player HP + inventory). **Free action** — does not consume a turn, and the front enemy does not retaliate. Maximum one undo per turn (no chained undo-undo loops). The wave queue is *not* rewound; see the F6 tie below for why.
  5. **Flee** — leave the encounter. The Warden does not chase; the marshalling yard remains closed and any uncleared waves remain.
- **Win condition.** All N waves cleared (every queue emptied) → print a victory message and the inner gate opens. Note your hero's surviving HP in the closing line.
- **Lose condition.** Player HP ≤ 0 → print a defeat message; the run ends.

**Required ties to Floors 4 – 7** — every one of these must be present in your battle code, real and load-bearing (not pasted in for show):

- **Floors 4 / 4½ (linked lists):** every turn — both the player's action and the front enemy's retaliation — must append a one-line string to `hero.eventLog` (already a `Chain<std::string>` from Floor 4½). The log grows forward, never backward; even `undo` appends an "(undo) reverted: <description>" entry rather than rewinding the chain. The log is what the **Inspect log** menu option reads.
- **Floor 5 (iterators):** the **Inspect log** menu option must call your Floor 5 `printLog` function template over `hero.eventLog`. Because every push elsewhere in the program is `push_front`, the most recent entry is at the chain's head — `begin()/end()` is newest-first; `rbegin()/rend()` is oldest-first (a "battle replay" framing). Pick whichever direction matches what Inspect log should mean for your encounter and document the choice in a one-line comment. The point is *iterator pair into `printLog`* — no subscripting the chain, no `chain.at(i)`, no manual `head()`-chase anywhere in your battle.
- **Floor 6 (stacks):** the undo history must be a `Stack<UndoAction>`. Every attack and every use-item pushes one `UndoAction` (with a snapshot of player HP and inventory) onto the stack *before* executing. `undo` pops the top entry and restores both. A comment above the stack's declaration must say in one line: *why a stack and not a queue.* A second comment above the `undo` body must name **one thing undo intentionally does not restore** (the wave queue is the obvious answer — pick that or another, and defend it).
- **Floor 7 (queues):** the current wave's enemies must be held in a `Queue<Enemy>`, with `front()` always returning the currently-targeted enemy and `dequeue()` only called when that enemy dies. The waves themselves may *also* be a `Queue<Wave>` (encouraged) — if you do that, document the nested-queue choice in `encounter-notes.md`.

**Also required:**

- A short `floor-07/warden-2-trials/encounter-notes.md` with:
  - Your AI declaration for this trial (per the policy).
  - **The two functions you wrote without AI**, named by file and line. Friday's quiz can pull from these.
  - One paragraph (≤ 200 words) reflecting on the integration: which Floor's tie was hardest to wire in, and why. Was it picking the iterator direction for **Inspect log**, the snapshot-and-restore on `Stack<UndoAction>`, the front-of-queue targeting, or deciding what to leave OUT of the undo snapshot?
- A working **build**. We will `cmake --build build` your project. If it does not compile, the Warden has won by default.

**Submit.** Your modified `main.cpp` + `battle/Battle.cpp` (and any helpers you split out) + `encounter-notes.md`, all committed to your project repo.

**Full credit (rubric inside the 60 %):**

| Sub-component | Pts |
|---------------|----:|
| Encounter compiles and plays through (waves clear, both endings reachable, undo works) | 25 % |
| All four Floor 4 – 7 ties present and meaningful                                       | 25 % |
| Encounter notes complete (AI declaration + two no-AI functions named + reflection)     | 5 %  |
| Code quality (readable, factored, no copy-paste fog)                                   | 5 %  |

## The Warden's Question — Friday in-class quiz (10 %)

Twenty minutes. Closed everything — no books, no laptops, no phones, no AI. You will receive **four short paper questions, each one a small modification to *your own submission*.** Three of the four will pull from the two functions you named in `encounter-notes.md` as written-without-AI. Examples:

- *"Modify your attack function so the front enemy's retaliation damage is halved if its HP is below 25 %. On paper, write the changed lines."*
- *"In your wave loop, add a 'peek' option that prints the names of the next three enemies in the queue without dequeuing them. Sketch what changes."*
- *"Your `undo` currently restores player HP and inventory but not the wave queue. On paper, sketch how you'd extend `UndoAction` to also snapshot the front enemy's HP — and explain in one line why that's still cheaper than snapshotting the whole queue."*

If you wrote your encounter, this quiz takes ten minutes. If you didn't, it takes the full twenty and the gap is what we grade.

## Submission and grading

**Submission.** Commit and push the entire `floor-07/warden-2-trials/` folder *and* your modified battle code to your project repo by **9:00 AM Friday of Week 10**. Late commits drop one letter grade per day on the take-home portion.

**Folder + code shape:**

```
[your project repo]/
  floor-07/
    main.cpp                          <-- new `battle waves N` command lives here
    battle/Battle.h, Battle.cpp       <-- add runWaveSurvival(...) alongside runWardenBattle
    hero/Chain.h                      <-- already there from Floor 4/4½
    hero/Stack.h, Queue.h             <-- already there from Floors 6, 7
    hero/UndoAction.h                 <-- already there from Floor 6
    warden-2-trials/
      AI-USE.md
      trial-1-brief.md
      trial-2-critique.md
      encounter-notes.md
```

**Rubric — top-level.**

| Component | Weight | Full-credit shape |
|-----------|-------:|-------------------|
| Trial I — The Keepers' Brief      | 15 % | Four tight 50–100-word answers in your voice |
| Trial II — The Critique           | 15 % | ≥150-word substantive critique of the LLM's `Deque<T>` |
| Trial III — The Marshalling Yard  | 60 % | Working wave-survival + 4 required Floor 4–7 ties + encounter notes (rubric above) |
| Friday in-class quiz              | 10 % | Four short paper answers modifying your own battle code |

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/mavren.svg' | relative_url }}" alt="Quartermistress Mavren, Keeper of the Chain Vault — a tall, narrow-shouldered keeper in a slate-grey coat, an iron link-ledger on a chain at her belt, hands clasped behind her back."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Mavren, from the north corner of the yard</span>

Write your encounter the way you write the ledger: *one link at a time*. Wire the wave queue first and run the battle — *just* the attack against one wave of one enemy, no items, no undo, no log. Compile. Run. *Then* push to `eventLog` on every turn and add the **Inspect log** option that walks it in reverse. Compile. Run. *Then* the stack of snapshots and the `undo` that restores HP and inventory. Compile. Run. *Then* multi-enemy waves and the `use item` branch. Compile. Run. A chain that is forged in one piece is a chain that breaks in one piece. *Add the next link only after the previous one held weight.*
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Warden of the Middle Gates</p>

**The Warden of the Middle Gates** — *HP: not measured here; the Warden does not fight the hero directly. Strength: equal to whichever floor of 4, 4½, 5, 6, 7 the hero understands least. Weakness: a hero whose four containers compose cleanly.*

The second of four wardens. Cataloguers caught its silhouette in the marshalling yard at the bottom of the Cisterns; the page they pinned to its entry shows armour, four sigils — chain link, lens, coiled echo, stamped ticket — and two ember points where eyes should be. The Warden does not strike; it *commands the yard*. Waves of small enemies arrive in arrival order, retaliate when struck, and only the hero whose inventory walks cleanly under an iterator and whose mistakes can be undone from a stack will see the inner gate open.

The Warden does not punish you for choosing the wrong container; the *yard* does. Pick `Bag<Item>` for the inventory and the iterator question is still answerable but the *reason* you needed iterators evaporates. Pick a queue for the undo stack and the most-recent action is the *last* one available, not the first — your `undo` will reverse the wrong turn at the worst time. The Warden watches. The Warden does not explain.

You will fight this creature inside the game you wrote. That fact, again, is the test.
</div>

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix has a study tip</span>

*Friend!* Wave survival — easy! Just make the wave a `std::vector<Enemy>`, attack `enemies[0]`, and when it dies, `enemies.erase(enemies.begin())`. Same shape, friend! Who needs a queue! (It is the same shape until the third wave, when erasing the front of a vector for the hundredth time has copied every later enemy a hundred times. The yard will not notice; the *grader* will. Grix's hero has been stuck on wave one since Floor 4.)
</div>
</div>

---

*The yard ends one way or the other — by Friday morning of Week 10. The Warden does not announce the result; the inner gate opens, or it does not. The judgement happens during Friday's class, in handwriting, on modifications the Warden asks you to make to your own code. Beyond the inner gate, the stair to Floor 8 — the **Branching Halls** — climbs back up. McCown is waiting at the top with his bestiary; this time, his entries have children.*
