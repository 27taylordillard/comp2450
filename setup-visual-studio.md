---
title: Arming the Adventurer
permalink: /setup/visual-studio/
---

# Arming the Adventurer

*Installing Visual Studio, and the one checkbox everybody misses.*

You cannot fight what you cannot compile. Before Floor 0 you need three things on your
machine: a **C++ compiler**, **CMake**, and **git**. The good news is that Visual Studio
installs the first two together, in one pass, if you tick the right box.

This page covers the compiler and CMake. Git and GitHub live on their own page —
[The Chronicle]({{ site.baseurl }}/setup/git/) — and you'll want that one too before
Lab 0 is due.

> **Windows is the documented path.** Every demo in class is Visual Studio on Windows.
> If you're on a Mac or Linux, or you'd rather live in a terminal, skip to
> [the other roads](#the-other-roads) at the bottom — you are welcome there, you'll just
> be translating a few clicks into commands on your own.

---

## 1 · Download

Get **Visual Studio Community 2026** from
[visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/).

Community edition is **free** for students. Take care to grab the right thing:

- **Visual Studio** — the full IDE. This is what you want.
- *Visual Studio Code* — a different, lighter product with a confusingly similar name.
  It can be made to work, but nothing in this course's instructions will match what you
  see on screen.

The download is a small installer that then fetches the real payload. Budget some time
and some disk — a full install runs several gigabytes.

## 2 · Tick the workload

The installer opens on a grid of **workloads** — bundles of tools for different kinds of
programming. You need exactly one:

> ### ☑ Desktop development with C++

Tick it. Ignore the rest — you can always come back and add more later.

## 3 · Now the checkbox everybody misses

With the workload ticked, look at the **right-hand pane**, labelled *Installation details*.
It lists the individual components the workload will install. Expand it and confirm this
one is checked:

> ### ☑ C++ CMake tools for Windows

It is normally checked by default as part of the workload — but *normally* is not
*always*, and this is the single most common reason a student's Visual Studio opens our
project folder and does nothing at all.

<div class="callout lore" markdown="1">
<p class="callout-title">Why this one matters</p>

Our project isn't a Visual Studio project. It's a **CMake** project — a platform-neutral
description of how to build the code, so the same source tree works on my Windows machine
and your classmate's MacBook. Without this component, Visual Studio doesn't know how to
read `CMakeLists.txt`, and the starter folder just looks like a pile of text files.

If you want to know what CMake actually *is* and why we bothered, read
[Why the Forge Needs a Blueprint]({{ site.baseurl }}/setup/cmake/).
</div>

Click **Install**. Go get a coffee.

## 4 · Open a starter folder

Here's the part that trips people up: **do not use File → Open → Project/Solution.** We
don't have a solution file. Instead:

1. Launch Visual Studio.
2. On the start window, choose **Open a local folder**
   *(or, once the IDE is open: **File → Open → Folder…**)*.
3. Select the starter folder itself — the one that directly contains `CMakeLists.txt`.
   For Floor 0 that's `project/floor-00-starter`.

Pick the **folder**, not a file inside it. Visual Studio looks for `CMakeLists.txt` at the
top of whatever folder you hand it.

## 5 · Watch it configure itself

Visual Studio spots `CMakeLists.txt` and immediately starts configuring. Look at the
**Output** window at the bottom; if you don't see it, open it with **View → Output**, then
set the *Show output from* dropdown to **CMake**.

You're looking for something close to this:

```
1> Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
1> Configuring done
1> Generating done
1> Build files have been written to: .../floor-00-starter/out/build/x64-Debug
1> CMake generation finished.
```

**`CMake generation finished.`** is the phrase that means you're safe. If you see it, your
compiler and CMake are both installed and talking to each other, and you're past the hard
part.

## 6 · Build and run

1. Find the **startup item** dropdown in the toolbar — the one with the green ▶ arrow
   next to it. Click it and select **`antechamber.exe`**.
   *(It's named after whatever the floor's program is called. Floor 0's is `antechamber`.)*
2. Press **Ctrl+F5** — *Start Without Debugging*.

Use **Ctrl+F5**, not plain F5. Both build and run, but plain F5 attaches the debugger and
slams the console window shut the instant your program ends, so you never see the output.
Ctrl+F5 leaves it open with a friendly *"Press any key to close this window."*

You should get:

```
=== THE ANTECHAMBER ===

A torch sputters. The air smells of iron.
What is your name, adventurer?
```

Type a name, press Enter, and the Antechamber will greet you.

<div class="callout check" markdown="1">
<p class="callout-title">You are armed when…</p>

- The CMake output pane ends with **CMake generation finished.**
- The startup dropdown offers you a real `.exe`, not *"Select Startup Item…"*
- **Ctrl+F5** prints `=== THE ANTECHAMBER ===` and waits for your name.

If all three are true, stop here. You're done. Go do
[The Chronicle]({{ site.baseurl }}/setup/git/) next.
</div>

## 7 · Debug and Release

There's a dropdown in the toolbar reading **x64-Debug**. Leave it there.

**Debug** builds keep your code arranged the way you wrote it, so the debugger can show
you real variables and real line numbers. **Release** builds let the optimiser rewrite
your code aggressively for speed.

You'll want Debug for essentially everything this semester. On Floor 0 there's a specific
reason: on Friday we run a program that measures how long two algorithms take, and the
optimiser is clever enough to notice the slow one is computing something predictable and
skip the work entirely — which makes the slow algorithm look instant and ruins the
demonstration. Debug builds run what you wrote.

---

## When the Compile Demon strikes

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · The Compile Demon</p>

*Strikes when your code is fine but your environment isn't.* Symptoms are confusing
because the error is never where the problem is.
</div>

| What you see | What it means | The counter |
|---|---|---|
| Opening the folder does nothing; no CMake output at all | **C++ CMake tools for Windows** isn't installed | Windows Start → *Visual Studio Installer* → **Modify** → tick the component → **Modify** |
| `No CMAKE_CXX_COMPILER could be found` | CMake is present, the C++ compiler isn't | You installed CMake but skipped the **Desktop development with C++** workload. Add it in the installer. |
| Startup dropdown only says *"Select Startup Item…"* | Configure hasn't finished, or it failed | Read the CMake output pane top to bottom for the first red line. **Project → Delete Cache and Reconfigure** usually clears it. |
| The console window flashes and vanishes | You pressed **F5** | Press **Ctrl+F5** instead. |
| You opened the repo root and see no CMake activity | Wrong folder | Open the *starter* folder — the one directly containing `CMakeLists.txt`. |
| Edits don't seem to take effect | You're running a stale build | Ctrl+F5 rebuilds first. If it persists, **Build → Rebuild All**. |
| Mystery errors that survive rebuilding | Corrupt CMake cache | **Project → Delete Cache and Reconfigure**. This is safe — the cache is generated, never your work. |

Still stuck? Screenshot the **whole** Output pane — not just the last line — and bring it
to office hours or post it. We do not fix compiler errors by skipping them. We fix them by
reading them.

---

## Working from the terminal instead

You never need the terminal in this course, but some of you will want it, and the class
notes sometimes quote `cmake` commands.

There's a catch worth knowing: **the CMake that ships with Visual Studio is not on your
system PATH.** Open a normal PowerShell, type `cmake --version`, and you'll be told the
command doesn't exist — even though it's sitting right there on your disk.

The fix is to use the shell that knows where everything lives. From the Start menu, open
**Developer PowerShell for VS 2026**. Inside it:

```
cd path\to\project\floor-00-starter
cmake -B build
cmake --build build
.\build\antechamber.exe
```

The `.\` on that last line isn't decoration — Windows won't run an executable out of the
current folder without it.

## The other roads

Not on Windows, or not interested in an IDE? That's fine — the project is plain C++17 and
CMake, with nothing Microsoft-specific in it.

| Platform | Compiler | CMake |
|---|---|---|
| macOS | `xcode-select --install` | `brew install cmake` |
| Linux (Debian/Ubuntu) | `sudo apt install g++` | `sudo apt install cmake` |
| Windows, terminal-first | [MSYS2](https://www.msys2.org/) → `pacman -S mingw-w64-ucrt-x86_64-gcc` | `winget install Kitware.CMake` |

Then, from any starter folder:

```
cmake -B build
cmake --build build
./build/antechamber
```

Everything the course asks you to submit works identically this way. The only thing you
give up is that when the class is looking at a screen together, your screen will look
different from mine.

---

*Armed? Good. The Antechamber is [this way]({{ site.baseurl }}/floors/floor-00/).*
