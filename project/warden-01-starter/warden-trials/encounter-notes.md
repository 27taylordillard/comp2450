# Trial III — *The Encounter* — Notes

*Warden of the Foundations · Midterm 1 · 60 %*

This file is the grader's map of your battle code. Help us find each piece. Be specific — file and line numbers are encouraged.

---

## AI declaration for Trial III

(One or two sentences. *Which* tool, *what* you asked it for, *what* you did with the result. "Did not use" is fine — but the line must be there.)

---

## Two functions I wrote without AI assistance

These are the functions Friday's quiz can ask me to modify on paper. I wrote each of these by hand, with no autocomplete on the body. Pick functions you know cold — three of the four quiz questions pull from these.

1. `<function-name>` in `<file>:<approximate line range>`
2. `<function-name>` in `<file>:<approximate line range>`

---

## Floor 0–3 ties — where to find them in my code

Help the grader find each of the four required ties.

**Floor 0 (ADT).** The available menu actions are stored in:

- Container type: `<...>`
- Declared at: `battle/Battle.cpp:<line>`
- ADT name + one-sentence defence: (paste your comment here, or quote the comment in the code)

**Floor 1 (search).** `findByName<Item>` is called at:

- `battle/Battle.cpp:<line>` (Use-item branch)

**Floor 2 (sort).** The at-display sort (`std::sort` with a comparator, or your Floor 2 `sortInventory`) is called at:

- `battle/Battle.cpp:<line>` (before displaying the items menu)
- Sort criterion: `<...>` (e.g., descending value — the healing-power stand-in — or ascending weight)
- One-sentence reason: (why this criterion?)

**Floor 3 (templates + exceptions).** `BattleException` (or `BagException` where a bad index is the fault) is:

- Thrown at: `battle/Battle.cpp:<line>` on `<condition>`
- Caught at: `battle/Battle.cpp:<line>` (must be **inside** the battle loop, not in main)

---

## Reflection  *( ≤ 200 words )*

Which Floor's tie was hardest to wire in, and why? Was anything obvious in hindsight that wasn't obvious when you started?

(your reflection)

---

## Tuning notes (optional)

If you changed any of the tunables (`kPlayerStartHP`, `kWardenStartHP`, `kPlayerAttackDmg`, `kWardenAttackDmg`), or added new ones (item healing amounts, etc.), list them and their values here so the grader can play through your battle without surprise.

(your notes, or "defaults")
