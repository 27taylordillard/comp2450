# Trial III — *The Encounter* — Notes

*Warden of the Foundations · Midterm 1 · 60 %*

This file is the grader's map of your battle code. Help us find each piece. Be specific — file and line numbers are encouraged.

---

## AI declaration for Trial III

(One or two sentences. *Which* tool, *what* you asked it for, *what* you did with the result. "Did not use" is fine — but the line must be there.)
used chatgpt to fix sytax errors, such as capitaliztion discrepancies in my items. 
used code we wrote together in class

---

## Two functions I wrote without AI assistance

These are the functions Friday's quiz can ask me to modify on paper. I wrote each of these by hand, with no autocomplete on the body. Pick functions you know cold — three of the four quiz questions pull from these.
functions that we wrote together in class:
1. `<printMenu()>` in `<battle.cpp>:<approximate line range>`
2. `<useItem()>` in `<battle.cpp>:<approximate line range>`

---

## Floor 0–3 ties — where to find them in my code

Help the grader find each of the four required ties.

**Floor 0 (ADT).** The available menu actions are stored in:

- Container type: `Bag<MenuOption>`
- Declared at: `battle/Battle.cpp:<196>`
- ADT name + one-sentence defence: Bag stores the available turn actions because we need a simple collection to loop through.

**Floor 1 (search).** `findByName<Item>` is called at:

- `battle/Battle.cpp:<165>` (Use-item branch)

**Floor 2 (sort).** The at-display sort (`std::sort` with a comparator, or your Floor 2 `sortInventory`) is called at:

- `battle/Battle.cpp:<152>` (before displaying the items menu)
- Sort criterion: `<...>` (e.g., descending value — the healing-power stand-in — or ascending weight)
- One-sentence reason: (why this criterion?) Higher value items appear first when the inventory is displayed.

**Floor 3 (templates + exceptions).** `BattleException` (or `BagException` where a bad index is the fault) is:

- Thrown at: `battle/Battle.cpp:<130>` on `<invalid input>`
- Caught at: `battle/Battle.cpp:<255>` (must be **inside** the battle loop, not in main)

---

## Reflection  *( ≤ 200 words )*

Which Floor's tie was hardest to wire in, and why? Was anything obvious in hindsight that wasn't obvious when you started?

(your reflection)
For me, Floor 3's tie was the hardest for me to understand how and why to wire it in. This is because I had to understand how throwing and catching exceptions worked. I was confused at first about where the exception needed to be thrown and where it needed to be caught. The other floors where easier for me to understand because we have worked with those concepts more. 

---

## Tuning notes (optional)

If you changed any of the tunables (`kPlayerStartHP`, `kWardenStartHP`, `kPlayerAttackDmg`, `kWardenAttackDmg`), or added new ones (item healing amounts, etc.), list them and their values here so the grader can play through your battle without surprise.

(your notes, or "defaults")
