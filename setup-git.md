---
title: The Chronicle
permalink: /setup/git/
---

# The Chronicle

*Git, GitHub, and getting your work where I can see it.*

Every floor you clear ends the same way: you commit your work and push it. That commit
is the proof — it's what gets graded, and it's a timestamped record of a semester's
descent that you'll still have years from now.

If you have never used git before, this page is the whole of what you need. Read it once
now, and come back to the [weekly rhythm](#the-weekly-rhythm) section every time you
finish a floor.

## Git and GitHub in three sentences

**Git** is a program on your computer that records snapshots of a folder over time. Each
snapshot is a **commit**, and it remembers what changed, when, and why you said you did it.

**GitHub** is a website that stores a copy of that history in the cloud, so it survives
your laptop dying and so I can see your work.

**GitHub Desktop** is a small app that drives git by clicking instead of typing. That's
what we'll use.

---

## 1 · Fork the course repository

All the starter code lives in one repository: **`HogRed/comp2450`**. That one is mine —
you can read it, but you can't write to it. You need your own copy that you *can* write to.

That copy is called a **fork**.

1. Go to [github.com/HogRed/comp2450](https://github.com/HogRed/comp2450).
2. Sign in (or make a free account — use whatever email you like, it needn't be Harding's).
3. Click **Fork**, top right.
4. Leave the settings alone and click **Create fork**.

You now have `github.com/<your-username>/comp2450` — a complete copy, yours, writable.

<div class="callout lore" markdown="1">
<p class="callout-title">Why a fork and not just a download</p>

A fork keeps a permanent link back to the original. That link is what lets you pull down
each new floor as I publish it, all semester, without losing a line of your own work.
A downloaded ZIP is a dead end; a fork stays connected.
</div>

**Do this before you clone anything.** Cloning my repository instead of your fork is the
single most common week-1 mistake, and you won't discover it until you try to push and
git tells you permission is denied.

## 2 · Install GitHub Desktop

Download it from [desktop.github.com](https://desktop.github.com/) and install it.

On first launch it asks you to sign in to GitHub — do that, and let it configure your name
and email when it offers. That's the "first-time git setup" step; if you've heard you need
to run `git config --global user.name`, GitHub Desktop just did it for you.

## 3 · Clone your fork

In GitHub Desktop: **File → Clone repository**.

Under the **GitHub.com** tab you'll see your repositories listed. Pick
**`<your-username>/comp2450`**.

> Check the username on it. If it says `HogRed/comp2450`, that's mine — go back to step 1.

Choose where to put it (the default `Documents\GitHub\comp2450` is fine, and a path with
no spaces or accents in it will save you grief later), then **Clone**.

If it asks *"How are you planning to use this fork?"*, choose **To contribute to the parent
project**. Either answer works for this course; that one keeps the upstream link tidiest.

You now have the whole dungeon on your disk. Each floor's starter is a folder under
`project/` — `project/floor-00-starter`, `project/floor-01-starter`, and so on. That
folder is what you open in Visual Studio.

## 4 · Commit and push

Work on the floor. When it runs the way you want:

1. Switch to **GitHub Desktop**. Your edits are listed down the left, with the exact lines
   you changed shown on the right. Take ten seconds to look — this is a free proofread and
   it catches an astonishing amount of accidental damage.
2. Tick the files you mean to include. (Normally all of them.)
3. In the **Summary** box, write the commit message. **It must begin with the floor:**

   ```
   Floor 0: set hero flavor line
   ```

   That prefix is how your work gets found and graded. `Floor 0: ` — the word, the number,
   a colon, a space, then a few words about what you actually did.
4. Click **Commit to main**.
5. Click **Push origin** at the top.

Not pushed is not submitted. The commit lives only on your laptop until you push it.

<div class="callout check" markdown="1">
<p class="callout-title">Confirm it landed</p>

Open `github.com/<your-username>/comp2450` in a browser. Your commit message should be
sitting right there on the front page with a timestamp. If you can see it on the website,
so can I.
</div>

## 5 · Pulling down each new floor

As the semester goes on I publish new floors into `HogRed/comp2450`. Your fork doesn't
update itself — you have to ask for the new material. Do this at the start of each week.

**The reliable way**, from the browser:

1. Go to your fork, `github.com/<your-username>/comp2450`.
2. If it says *"This branch is N commits behind"*, click **Sync fork → Update branch**.
3. Switch to GitHub Desktop and click **Pull origin**.

**The quicker way**, if GitHub Desktop offers it: **Branch → Update from upstream/main**,
then **Push origin**.

This is safe. Each floor's starter is a self-contained folder of its own, so new floors
arrive as new folders and never touch the ones you've been working in. You should not see
a conflict all semester. If you somehow do, stop and bring it to office hours rather than
guessing — conflicts are easy to resolve and easy to make much worse.

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · The Chronicler's Curse</p>

*Strikes those who commit faithfully and never push.* Symptoms: a perfect local history,
an empty GitHub page, and a zero in the gradebook. **Counter:** after every *Commit to
main*, look for **Push origin** and click it. Then check the website.
</div>

---

## The weekly rhythm

Once you're set up, every floor is the same five moves:

1. **Sync fork → Pull origin** — collect the new floor.
2. Open `project/floor-NN-starter` in Visual Studio.
3. Do the work. Build, run, get it right.
4. **Commit** with a `Floor NN: ` message.
5. **Push origin**, then glance at the website to confirm.

That's it. It becomes muscle memory by about Floor 3.

## When things go wrong

| What you see | What it means | The counter |
|---|---|---|
| `Permission denied` / `403` on push | You cloned my repository, not your fork | Fork it (step 1), then clone the fork. Your existing edits can be copied across by hand. |
| Push is greyed out | Nothing committed yet | Write a summary and click **Commit to main** first. |
| The website doesn't show your work | Committed but not pushed | Click **Push origin**. |
| Hundreds of files listed as changed | `build/` output leaked in | Don't commit them. The `.gitignore` normally hides these; if it isn't, check you're in the course repo and not some folder you made yourself. |
| *"This branch is 14 commits behind"* | New floors are waiting | **Sync fork → Update branch**, then **Pull origin**. |
| You edited the wrong floor's folder | Easily done | Just move the work into the right folder and commit again. Nothing is lost. |

## For the terminal-inclined

Nothing here requires the command line, but the equivalents are short, and you'll meet
them in the wider world:

| GitHub Desktop | git |
|---|---|
| Clone repository | `git clone https://github.com/<you>/comp2450.git` |
| *(the changes list)* | `git status` |
| Tick files | `git add project/floor-00-starter/main.cpp` |
| Commit to main | `git commit -m "Floor 0: set hero flavor line"` |
| Push origin | `git push` |
| Pull origin | `git pull` |
| Sync fork | `git fetch upstream && git merge upstream/main` |

One caveat if you go this way on Windows: run these in **Git Bash** or **Developer
PowerShell for VS**, not a bare `cmd` window, and remember that `git add` paths are
relative to where you currently are, not to the top of the repository.

---

*Chronicle in order? Then the [Antechamber]({{ site.baseurl }}/floors/floor-00/) is
waiting.*
