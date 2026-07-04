---
title: The Warden of the Foundations
floor_number: "Warden · I"
order: 50
flavor_quote: "Four floors built you a hero. The Warden waits for the hero to use what was built."
covers: "Floors 0–3 · Midterm 1"
---

## The gate

Steil's forge has gone cold behind you. Ahead, a vaulted antechamber — empty but for a single shape standing at its centre, and a closed gate beyond it. On the far side of the gate, you can hear Foust's cart-wheel; Floor 4 is on the other side.

The shape does not move *yet*. It has no face you can name. It is the **Warden of the Foundations** — the first of four — and it does not bring a fifth lesson. It brings every one of the four above, and asks whether they took. To prove they did, you will study, you will critique, and at the end, **you will fight it inside the game you have been building**.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/warden-01.svg' | relative_url }}" alt="The Warden of the Foundations — a hooded, faceless figure in dark iron plate, only two ember-gold eye-points visible inside the deep cowl. Four small sigils on the breastplate: hourglass, magnifying glass, forge flame, anvil. Nameplate blank."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">The Warden, who has no name to give</span>

*Read. Critique. Build me into your game — and then strike me.*
*Three trials, and an encounter. Pass them all, or do not pass.*
</div>
</div>

**This page is your Midterm 1.** It replaces the timed paper exam. It is in two parts: a **take-home gauntlet of three trials** committed to your project repo, the third of which is a working boss battle inside your dungeon app; and a short **in-class quiz on Friday** that ties your submission to your own hand. Together they are 100 % of the midterm.

<div class="callout check" markdown="1">
<p class="callout-title">The schedule</p>

| Phase | When | What |
|-------|------|------|
| **Gauntlet opens** | Friday, end of Week 4 — after Floor 3's last class | The three trials below become available. Begin any time. |
| **Gauntlet closes** | Friday of Week 5, **9:00 AM** | All artifacts and battle code committed and pushed to your project repo. |
| **In-class quiz** | Friday of Week 5, during class | ~20-minute paper quiz — every question pulled from *your own* battle code. |
| **Grades returned** | Following Wednesday | Per-trial rubric notes + total. |

Plan your week. Trials I and II can be knocked out in an evening each. Trial III — the boss battle — is real engineering work and will take you several focused sittings. Start it early.
</div>

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/faith.svg' | relative_url }}" alt="Brother Faith, Abbot of the Sorted Tome — bearded monk in a dark brown robe with a brass clasp and a stopwatch on a brass cord."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Brother Faith stands with you at the gate</span>

I am not going to make you race a paper exam under my stopwatch. I am going to ask you to *use* what these four floors gave you — in the game you have been building, against the Warden in front of you. Search. Sort. Stamp. Catch. If those four words live in your fingers by Friday, you will pass. If they live only in your notes, the Warden will see it the moment you step on the floor.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">AI policy for this midterm — read once, follow always</p>

You may use AI assistants (Claude, Copilot, ChatGPT, the in-IDE completion in your editor — anything) on every trial below. Trial II **requires** you to use one. The rules are:

1. **Declare what you used, where.** Submit an `AI-USE.md` in your `warden-trials/` folder. For each trial, write one or two lines: *which* tool, *what* you asked it for, *what* you did with the result. "Did not use" is a perfectly good entry — but the line must be there.
2. **The artifact must survive a conversation about it.** Friday's quiz will pull questions directly from *your* battle code in your own handwriting. If you turned in something you cannot defend, you submitted it from the wrong author.
3. **Submitting AI output as-is, undeclared, is the failure mode** — not the AI use itself. Reading, running, fixing, and understanding AI output is *engineering*. Pasting and shipping is not.
4. **Trial II is explicit:** you will engage an LLM, paste the prompt and response, and write a critique.
5. **Trial III adds one constraint:** in your `encounter-notes.md`, name **two functions** in your battle code that you wrote *without* AI assistance — by hand, with no autocomplete on the body. These are the functions Friday's quiz can ask you to modify on paper, so pick ones you actually understand cold.

This policy is the same one your future employer will write next year. Practice now.
</div>

## The three trials

Run them in order. Trials I and II are short prep that prove your foundations are sound. Trial III is where you put them to work.

### Trial I — *The Foundations Brief* (15 %)

The Warden wants four short answers — one from each floor above, each one anchored to the battle you are about to build. No code on this trial; just clear thinking on paper-or-keyboard.

**Do.** Open `floor-03/warden-trials/trial-1-brief.md` and write **50–100 words per answer**:

1. **(Floor 0 — ADT.)** Your battle's "Use item" menu shows the *currently usable* items in your hero's inventory on this turn. Name the right ADT for that menu. Defend the choice against its closest neighbour (e.g., why `bag` instead of `set`, or `list` instead of `bag`).
2. **(Floor 1 — search & Big-O.)** Your inventory is kept sorted by healing power (in this codebase an item's `value` measures its potency, so `value` plays the healing-power role). The player types `use Healing potion`. Linear or binary search to find it by name? Justify, and give the Big-O for each.
3. **(Floor 2 — sort & comparators.)** Your "Use item" menu must be displayable sorted *either* by healing power (meaning `value`) *or* by weight. Show a one-line comparator (lambda) for each. One sentence on what language feature makes one `std::sort` call serve both orders.
4. **(Floor 3 — templates & exceptions.)** Why does `Bag<T>` live in `Bag.h` instead of `Bag.cpp`? And: when the player types `9` for a 4-option menu, where in your code should the validation **throw**, and where should it **catch**?

**Submit.** `trial-1-brief.md`.

**Full credit.** Four answers, each tight and correct. AI is welcome to *check* your answers; AI cannot *write your sentences* — these are short enough that we will recognise the LLM voice.

### Trial II — *The Critique* (15 %)

You will engage an LLM, save the conversation, and critique what it gave you. This is the AI-literacy trial: reading and judging AI code is now part of the job.

**Do.** In `floor-03/warden-trials/trial-2-critique.md`:

1. Open Claude (or your LLM of choice). Paste this **exact** prompt:
   > *Write a C++ function template that returns the largest element of a `std::vector<T>`. Handle the case where the vector is empty. Briefly explain your design choices.*
2. Paste the prompt **and** the LLM's full reply into `trial-2-critique.md`.
3. Below the reply, write a critique of **at least 150 words** addressing:
   - Does the code compile? Did you try it?
   - What does the empty-vector case actually do? Is the LLM's choice reasonable?
   - What does this require of `T`? What happens if you instantiate it with a type that doesn't support `<`?
   - Const-correctness — by value or by reference? Would you change either?
   - Did the LLM cheat by using `std::max_element`, or did it write the loop? Which would you have shipped, and why?
   - One concrete improvement you would make before merging this into a real project.

**Submit.** `trial-2-critique.md`.

**Full credit.** A critique that reads like an engineer reviewing a colleague's pull request — specific, technical, and naming what the LLM did. A vague critique is *not* full credit even if the LLM's code was great.

### Trial III — *The Encounter* (60 %)

This is the trial. You will program the Warden of the Foundations into your dungeon app as a real, playable boss battle, and you will fight it.

**Do.** The encounter lives in one fixed place: `runWardenBattle` in `battle/Battle.h` + `battle/Battle.cpp`. If you are continuing your own floor-03 project (the one you've been building all term), copy the starter's `battle/` folder in and add the one-line **`battle warden`** dispatch to your `main.cpp` — the starter README shows the exact snippets. If you are starting fresh from the warden starter drop, the dispatch is already pre-wired. Either way, how you decompose the code *inside* those two files — helpers, private types, the shape of the loop — is entirely yours. When the player types `battle warden`, the encounter begins.

**You will receive (in your starter drop).** The starter, `project/warden-01-starter/`, ships everything except the battle itself:

- a pre-wired `main.cpp` — on Path B the `battle warden` dispatch is already in place;
- `battle/Battle.h` — the ready `BattleException` and the `BattleOutcome` contract `main.cpp` depends on;
- `battle/Battle.cpp` — the requirements checklist, a suggested outline, and the grader's test script, all in its comments;
- the four `warden-trials/` templates — `AI-USE.md`, `trial-1-brief.md`, `trial-2-critique.md`, `encounter-notes.md`;
- the README, with the exact Path A wiring diffs if you are continuing your own `floor-03` project.

Even on Path A — your own project — read the starter's battle file and README first: the checklist and wiring diffs in them are the map.

<div class="callout check" markdown="1">
<p class="callout-title">Suggested campaign — five sittings</p>

A suggested order, not a mandate — the exam is yours to pace.

| Sitting | Build | You know it held when |
|--------:|-------|-----------------------|
| 1 | The bare duel — HP variables, the turn loop, Attack + retaliation only. | Both endings are reachable. |
| 2 | The menu — your F0 actions container, the numbered menu, `throw`/`catch` *inside* the loop. | Typing `9` re-prompts without crashing or ending the turn. |
| 3 | Use item — sort-at-display with your comparator [F2], the `findByName` lookup [F1], the unknown-name throw [F3]. | A potion heals; a typo errors cleanly. |
| 4 | Inspect + Flee + `encounter-notes.md` — and pick your two no-AI functions while the code is fresh. | Inspect costs no turn, and the notes name two functions you can defend cold. |
| 5 | Trials I and II, if not already done — then rehearse Friday by modifying your two functions on paper, once. | The paper rewrite takes minutes, not the evening. |

</div>

**Required mechanics — minimum viable:**

- **Stats.** Player starts at 30 HP. Warden starts at 50 HP. (Numbers are yours to tune; document them.)
- **Turn loop.** Each turn: print current state (both HPs, last action), present a numbered menu, read input, execute, repeat.
- **Menu — at least four options:**
  1. **Attack** — deal damage to the Warden. Warden retaliates the same turn.
  2. **Use item** — pick from your inventory; apply effect; turn ends.
  3. **Inspect Warden** — print the Warden's HP and one of its sigils as a hint (free action — turn does *not* end).
  4. **Flee** — leave the encounter. The Warden does not chase; the gate stays closed.
- **Win condition.** Warden HP ≤ 0 → print a victory message and the gate opens.
- **Lose condition.** Player HP ≤ 0 → print a defeat message; the run ends.

**Required ties to Floors 0 – 3** — every one of these must be present in your battle code, real and load-bearing (not pasted in for show):

- **Floor 0 (ADT):** the available menu actions for the current turn must be held in a container of your choice. In a comment above the declaration, name the ADT and defend it in one sentence — the same kind of one-sentence defence Trial I practiced, but note this is a *different* collection than Q1's items menu, so the right ADT (and the defence) may differ.
- **Floor 1 (search):** the "Use item" branch must call `findByName<Item>` (your Floor 3 function template) on the hero's inventory to look up the item the player typed.
- **Floor 2 (sort):** when the "Use item" menu is displayed, the inventory must be sorted at display time with a comparator (via `std::sort` or your Floor 2 `sortInventory`) — by `value` (the healing-power stand-in), `weight`, or `name`, your call — and the choice documented in a comment.
- **Floor 3 (templates + exceptions):** invalid menu input (out-of-range number, unknown item name) must `throw BattleException` (a ready skeleton ships in `battle/Battle.h`) — or `BagException` where a bad index really is the fault; either way, a type derived from `std::exception`. The throw must be caught **inside the battle loop** so the player gets a clean error and another menu prompt — not a crash, not an exit. The player's inventory must remain a `Bag<Item>`.

**Also required:**

- A short `floor-03/warden-trials/encounter-notes.md` with:
  - Your AI declaration for this trial (per the policy).
  - **The two functions you wrote without AI**, named by file and line. Friday's quiz can pull from these.
  - One paragraph (≤ 200 words) reflecting on the integration: which Floor's tie was hardest to wire in, and why.
- A working **build**. We will `cmake --build build` your project. If it does not compile, the Warden has won by default.

**Demo target.** Below is a captured run from the reference solution, so you can see the shape of a passing battle. It shows the required *behaviours*, not required text — your wording, your numbers, and your menu order may differ:

```
> battle warden
  -- Your turn --   you HP 30    Warden HP 50
    1. Attack
    2. Use item
    3. Inspect Warden
    4. Flee
  > 1
  You strike for 6.  Warden HP -> 44.
  The Warden retaliates for 4.  Your HP -> 26.

  -- Your turn --   you HP 26    Warden HP 44
    ...
  > 3
  Warden of the Foundations.  HP 44 / 50.  No visible weakness.  (free action)

  -- Your turn --   you HP 26    Warden HP 44
    ...
  > 9
  index 9 out of bounds for size 4 — try again.

  -- Your turn --   you HP 26    Warden HP 44
    ...
  > abc
  'abc' is not a menu number (enter 1 to 4) — try again.

  -- Your turn --   you HP 26    Warden HP 44
    ...
  > 2
  Choose an item by name:
   1.  Cloak of shadows  (wt 1.5, val 80)
   2.  Healing potion    (wt 0.5, val 12)
   3.  Rusty sword       (wt 4.0, val 5)
   4.  Loaf of bread     (wt 0.1, val 1)
   5.  Iron key          (wt 0.1, val 0)
  > Healing potion
  You drink Healing potion. HP -> 30.
  The Warden strikes while you fumble.  Your HP -> 26.

[... one failed item lookup, seven Attack turns, and two more Healing potions trimmed ...]

  -- Your turn --   you HP 14    Warden HP 2
    ...
  > 1
  You strike for 6.  Warden HP -> 0.

The Warden falls. The gate opens.
```

And the other ending, so you know both are reachable:

```
  -- Your turn --   you HP 2    Warden HP 8
    1. Attack
    2. Use item
    3. Inspect Warden
    4. Flee
  > 1
  You strike for 6.  Warden HP -> 2.
  The Warden retaliates for 4.  Your HP -> 0.

You fall. The gate stays shut.
```

<div class="callout check" markdown="1">
<p class="callout-title">The Warden's own checks — run these before you push</p>

The grader's test script ships in your starter — the header comments of `battle/Battle.cpp` — and every line of it runs at your own keyboard:

**Phase 1 — the skeleton holds.**

- `battle warden` starts the encounter from the main prompt.
- The state line — both HPs — prints at the top of every turn.
- Both endings are reachable: win one run, lose one on purpose.

**Phase 2 — the gate holds.**

- Type `9` at the menu: clean error, re-prompt, *same* turn — no crash, no damage taken.
- Type a word like `abc`: same story.
- Inspect the Warden: no retaliation, and the turn does not end.

**Phase 3 — the ties hold.**

- The items menu appears sorted by your documented criterion.
- A mistyped item name errors cleanly and re-prompts.
- Open your own file and put a finger on each of the four ties — [F0], [F1], [F2], [F3].

Friday's grader runs exactly this list. If all three phases pass at your keyboard, the build half of the grade is already earned.

</div>

**Submit.** Your `battle/Battle.{h,cpp}`, your modified `main.cpp`, and `encounter-notes.md`, all committed to your project repo.

**Full credit (rubric inside the 60 %):**

| Sub-component | Pts |
|---------------|----:|
| Battle compiles and plays through (menu, both endings reachable) | 25 % |
| All four Floor 0 – 3 ties present and meaningful                  | 25 % |
| Encounter notes complete (AI declaration + two no-AI functions named + reflection) | 5 % |
| Code quality (readable, factored, no copy-paste fog)              | 5 % |

## The Warden's Question — Friday in-class quiz (10 %)

Twenty minutes. Closed everything — no books, no laptops, no phones, no AI. You will receive **four short paper questions, each one a small modification to *your own submission*.** Three of the four will pull from the two functions you named in `encounter-notes.md` as written-without-AI. Examples:

- *"Modify your `attackWarden` function so the player's attack damage scales with their remaining HP. On paper, write the changed lines."*
- *"In your battle's menu loop, add a 'Defend' option that halves the Warden's next attack. Sketch what changes."*
- *"Your inventory comparator currently sorts by `value`. Rewrite it on paper to sort by value-per-weight."*

If you wrote your battle, this quiz takes ten minutes. If you didn't, it takes the full twenty and the gap is what we grade.

The quiz is the integrity seal. It is short by design and intentionally tied to your own submission — not because we don't trust you, but because by Friday you will know your battle cold, and a quick conversation in your own handwriting is the cleanest proof of that.

<div class="callout check" markdown="1">
<p class="callout-title">Before Friday</p>

Closed laptop, blank paper. You should be able to:

- write the signatures of your two no-AI functions from memory, and sketch each body's control flow in five lines;
- rewrite your use-item comparator to sort by a different key, on paper;
- state where your throw fires on bad input, where it is caught, and what the player sees;
- defend your F0 actions-container choice in one sentence.

If any of these takes more than two minutes, that is Thursday night's studying — located for you.

</div>

## Submission and grading

**Submission.** Commit and push the entire `floor-03/warden-trials/` folder *and* your modified battle code to your project repo by **9:00 AM Friday of Week 5**. Late commits drop one letter grade per day on the take-home portion.

**Commit cadence.** Your Trial III work must arrive in **at least three commits on at least two different days** — one commit per sitting is the natural rhythm, each message naming the link that now holds weight (*"F2: items menu sorts at display time"*). Cadence is priced inside the existing code-quality points, not a new line item — and a single Thursday-night megacommit costs those points *and* invites Friday-quiz questions you will not enjoy.

**Folder + code shape:**

```
[your project repo]/
  floor-03/
    main.cpp                       <-- one-line `battle warden` dispatch lives here
    battle/
      Battle.h, Battle.cpp         <-- runWardenBattle lives here
    hero/Bag.h, BagException.h     <-- already there from Floor 3 starter
    warden-trials/
      AI-USE.md
      trial-1-brief.md
      trial-2-critique.md
      encounter-notes.md
```

The `battle/` location is fixed — the grader, the starter's `CMakeLists.txt`, and the prompts in `encounter-notes.md` all point at `battle/Battle.cpp`. What's yours to decide is how you organise the code *inside* those two files.

**Rubric — top-level.**

| Component | Weight | Full-credit shape |
|-----------|-------:|-------------------|
| Trial I — The Foundations Brief  | 15 % | Four tight 50–100-word answers in your voice |
| Trial II — The Critique          | 15 % | ≥150-word substantive critique of the LLM's reply |
| Trial III — The Encounter        | 60 % | Working battle + 4 required Floor 0–3 ties + encounter notes (rubric above) |
| Friday in-class quiz             | 10 % | Four short paper answers modifying your own battle code |

Steil catches you at the gate on your way in — his advice is the five-sitting campaign table above, in forge-speak.

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/steil.svg' | relative_url }}" alt="Forgemaster Steil, Master of the Cold Forge — long brown hair streaked with gray, heavy beard, dark leather apron with brass anvil pin over a rust-red tunic."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Steil, leaning on his hammer at the gate</span>

When you write the battle, do not write it all at once. Write *one turn* — print, prompt, read, attack, retaliate. Compile. *Run it.* Then write the next move. Builders who write the whole forge before lighting the fire end up with a forge that does not light. The Warden knows this. So does any compiler that has shouted at you. Read the first line of every error message; ignore the rest until the first one is solved.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Warden of the Foundations</p>

**The Warden of the Foundations** — *HP: 50 (default). ATK: 4 per turn. Weakness: a hero who has actually used what the four floors taught.*
The first of four wardens. Cataloguers caught its silhouette but never its name; the page they pinned to its entry shows armour, four sigils, and two ember points where eyes should be. Below the figure, the name slot is struck through. The Warden fights at the strength of whichever floor you understand *least* — a hero who is strong on three floors and weak on one is a hero the Warden picks apart at the seam.

The Warden does not punish you for using your tools. It punishes you for **letting your tools punish your understanding**. A hero who wrote their battle with AI help, ran it, broke it, fixed it, named two functions they wrote alone, and can defend them on Friday — passes. A hero who pasted the whole battle from an LLM and shipped it without reading — does not.

You will fight this creature inside the game you wrote. That fact is the test.

Counter by:

- **Catch *inside* the loop, not around it.** A `catch` outside the loop turns one typo into a forfeit — the player is ejected from the encounter instead of re-prompted.
- **Validate before you mutate.** Read the input, validate it, *then* apply damage — or a bad turn half-executes.
- **Inspect must not retaliate.** Free means free — test it.
- **Sort at display time, not at startup.** A battle changes the inventory under you; a menu sorted once in `main` lies by the third turn.

</div>

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix has a study tip</span>

*Friend!* The boss battle — easy! Ask the **machine** to write the *whole* thing, paste it in, push it, do not fill in the AI form, name two random functions as "yours," and on Friday — just *look surprised.* Look very surprised, friend! Works *every time.* (It does not work any time. Grix has tried. Grix's hero has been at zero HP since Floor 1.)
</div>
</div>

---

*The battle ends one way or the other — by Friday morning. The Warden does not announce the result; it falls, or you do. The judgement happens during Friday's class, in handwriting, on modifications the Warden asks you to make to your own code. Beyond the gate, the stair to Floor 4 — the **Chain Vault** — descends into the dark. Foust is already there with his cart, sorting maps.*
