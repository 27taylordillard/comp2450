# Trial III — *The Masked Menagerie* — Notes

*Warden of the Trees · Midterm 3 · 60 %*

This file is the grader's map of your Tree-warden code. Help us find each piece. Be
specific — file and line numbers are encouraged.

---

## AI declaration for Trial III

(One or two sentences. *Which* tool, *what* you asked it for, *what* you did with the
result. "Did not use" is fine — but the line must be there.)

---

## Two functions I wrote without AI assistance

These are the functions Friday's quiz can ask me to modify on paper. I wrote each of these
by hand, with no autocomplete on the body. Pick functions you know cold — **three of the
four** quiz questions pull from these.

1. `<function-name>` in `<file>:<approximate line range>`
2. `<function-name>` in `<file>:<approximate line range>`

*(Good candidates: `buildMenagerie` — the `levelOrder()` filter-to-leaves; and your Ward
handler — the `hasWeakness`/`countWeakness` exposure. The quiz's highest-leverage question
is the level-order → pre-order swap, so make sure `buildMenagerie` is one of your two.)*

---

## Floor 8 – 11 ties — where to find them in my code

Help the grader find each of the four required ties. Each must be a **load-bearing** call,
not decoration.

**[F8] Tree / taxonomy / lineage.** The menagerie marshals in level-order; the Warden is
exposed by a lineage.

- `taxonomy.levelOrder()` builds the arrival order: `battle/Battle.cpp:<line>`
- The leaf filter (kept only leaves the catalogue knows): `battle/Battle.cpp:<line>`
- One-line comment noting `preOrder()` would put a different beast first: `battle/Battle.cpp:<line>`
- `taxonomy.lineage(name)` exposes the Warden: `battle/Battle.cpp:<line>`

**[F9 + F9½] BST + balance.** Recall is the slow identify; the lean makes it slower.

- `catalogue.find(probe)` on Recall (probe built with only the name set): `battle/Battle.cpp:<line>`
- Balance judgement — `isBalanced()` / `height()` / `balanceFactor()`: `battle/Battle.cpp:<line>`
- Where the lean adds the extra retaliation: `battle/Battle.cpp:<line>`

**[F10] sets & maps.** Ward a weakness from your distinct-weakness arsenal.

- `registry.hasWeakness(w)` validates the ward (a `std::set` membership): `battle/Battle.cpp:<line>`
- `registry.countWeakness(w)` — the "how many kin fear this" hint (a `const std::map` find): `battle/Battle.cpp:<line>`
- `registry.weaknesses()` prints the arsenal: `battle/Battle.cpp:<line>`

**[F11] hash table (the whisper).** Whisper is the free identify; the banish forces a Recall.

- `whispers.find(name)` — the free O(1) identify: `battle/Battle.cpp:<line>`
- The **local copy** made BEFORE the banish (`HashTable localWhispers = whispers;`): `battle/Battle.cpp:<line>`
- The banish (`localWhispers.erase(...)`) — and one line saying which beast and why: `battle/Battle.cpp:<line>`
- One sentence: confirm the shared `whispers` is never mutated, and that whispering the
  banished beast returns `nullptr` so you must Recall it: (your note)

---

## Reflection  *( ≤ 200 words )*

Which hall's tie was hardest to wire in, and why? Was it filtering `levelOrder()` down to
the leaves the catalogue knows, building the `Monster probe` for `catalogue.find`, keeping
the ward's exposure logic honest with `hasWeakness`/`countWeakness`, or making the banish
work on a **local copy** so the session-wide `whispers` stayed untouched?

(your reflection)

---

## Tuning notes (optional)

If you changed the player's starting HP, the beast lineup or count, the chip / exposed
damage, the Warden's HP / ATK, or the lean penalty, list them and their values here so the
grader can play through your battle without surprise.

(your notes, or "defaults: player 45 HP, chip 4, exposed 14, Warden 24/5, Recall +1 hit on
the lean")
