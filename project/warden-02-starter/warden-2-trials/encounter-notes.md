# Trial III — *The Marshalling Yard* — Notes

*Warden of the Middle Gates · Midterm 2 · 60 %*

This file is the grader's map of your wave-survival code. Help us find each piece. Be specific — file and line numbers are encouraged.

---

## AI declaration for Trial III

(One or two sentences. *Which* tool, *what* you asked it for, *what* you did with the result. "Did not use" is fine — but the line must be there.)

---

## Two functions I wrote without AI assistance

These are the functions Friday's quiz can ask me to modify on paper. I wrote each of these by hand, with no autocomplete on the body. Pick functions you know cold — three of the four quiz questions pull from these.

1. `<function-name>` in `<file>:<approximate line range>`
2. `<function-name>` in `<file>:<approximate line range>`

---

## Floor 4 – 7 ties — where to find them in my code

Help the grader find each of the four required ties.

**Floor 4 / 4½ (linked lists).** Every turn appends to `hero.eventLog` (a `Chain<std::string>`):

- Engagement entry: `battle/Battle.cpp:<line>`
- Per-turn attack entry: `battle/Battle.cpp:<line>`
- Per-turn retaliation entry: `battle/Battle.cpp:<line>`
- Undo entry (intentional FORWARD push, not a rewind): `battle/Battle.cpp:<line>`

**Floor 5 (iterators).** Inspect-log branch calls `printLog` with an iterator pair:

- `battle/Battle.cpp:<line>` — `printLog(hero.eventLog.<begin/rbegin>(), hero.eventLog.<end/rend>(), N, ...)`
- Direction choice + defence (one line): (newest-first via `begin/end`, or oldest-first "replay" via `rbegin/rend` — say which and why)

**Floor 6 (stacks).** Undo history stored in:

- Container type: `Stack<UndoAction>`
- Declared at: `battle/Battle.cpp:<line>` (local? or `hero.undoStack`? — say which and why)
- Pushed at: `battle/Battle.cpp:<line>` (before Attack), `battle/Battle.cpp:<line>` (before Use item)
- Popped at: `battle/Battle.cpp:<line>` (in Undo case)
- One-line defence (stack vs queue/deque): (your comment, or quote the comment in the code)
- One-line "what undo intentionally does NOT restore": (your comment, or quote the comment in the code)

**Floor 7 (queues).** Current wave held in:

- Container type: `Queue<Enemy>` (or `Queue<EnemyPtr>` if you used pointers)
- Built at: `battle/Battle.cpp:<line>` (via your `buildWave` helper or inline)
- `front()` called at: `battle/Battle.cpp:<lines>` (to read the active target; should NOT mutate the queue)
- `dequeue()` called at: `battle/Battle.cpp:<line>` (only when the target's HP <= 0)

---

## Reflection  *( ≤ 200 words )*

Which Floor's tie was hardest to wire in, and why? Was it picking the iterator direction for Inspect log, the snapshot-and-restore on `Stack<UndoAction>`, the front-of-queue targeting, or deciding what to leave OUT of the undo snapshot?

(your reflection)

---

## Tuning notes (optional)

If you changed the player's starting HP, the per-wave enemy lineup, the damage numbers, or added any new tunables (item healing amounts, between-wave heals, etc.), list them and their values here so the grader can play through your battle without surprise.

(your notes, or "defaults")
