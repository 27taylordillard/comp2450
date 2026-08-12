# Floor 0 Starter — *The Antechamber*

This is the starter code for your semester-long dungeon-crawler project. By the end of the semester, this folder's descendants will be a full game. This week, it does almost nothing. That's the point.

## What this program does

Prompts for your hero's name, then prints one line of dungeon flavor text. That's it. You are not implementing a data structure yet. You are confirming that your **compiler, editor, git, and terminal** all work together on your machine.

## What you need to do

1. **Fork the course repository and clone your fork.**
   All the starter code lives in `HogRed/comp2450`, which you can read but not write to. Fork it to your own account first, then clone *your fork* — otherwise your push at step 6 will be refused.
   Full walkthrough: **[The Chronicle](https://hogred.github.io/comp2450/setup/git/)**.

2. **Install Visual Studio** (Community 2026 is free) with the **Desktop development with C++** workload, and confirm **C++ CMake tools for Windows** is checked.
   Full walkthrough: **[Arming the Adventurer](https://hogred.github.io/comp2450/setup/visual-studio/)**.

3. **Open this folder** — `project/floor-00-starter`, the one containing `CMakeLists.txt` — with **File → Open → Folder**. Visual Studio configures it automatically. Wait for `CMake generation finished.` in the Output pane.

4. **Run it** with **Ctrl+F5** (*Start Without Debugging*), after picking `antechamber.exe` in the startup-item dropdown.
   Use Ctrl+F5, not F5 — plain F5 closes the console the instant the program ends.

5. **Edit `main.cpp`.** Find the line marked `// TODO Floor 0:` and replace the placeholder `"Change me, adventurer."` with your own flavor line. Rebuild and run to confirm it shows up.

6. **Commit and push.** Your commit message must start with `Floor 0: `. Then open your fork on github.com and confirm the commit is actually there — not pushed is not submitted.

### If you'd rather use the terminal

Everything above works from a shell too. On Windows use **Developer PowerShell for VS** (the CMake that ships with Visual Studio isn't on your normal PATH):

```
cmake -B build
cmake --build build
.\build\antechamber.exe      # Windows — the .\ is required
./build/antechamber          # macOS / Linux
```

## Deliverable for Lab 0

A terminal screenshot (or pasted output) showing your program running with *your* hero's name and *your* flavor text, plus the URL of your commit on your fork. Both go on the Canvas assignment.

## If you get stuck

The **Compile Demon** is waiting. Common fixes:

- **Opening the folder does nothing** — the **C++ CMake tools for Windows** component isn't installed. Visual Studio Installer → *Modify* → tick it.
- **`No CMAKE_CXX_COMPILER could be found`** — CMake is there but the C++ compiler isn't. Add the *Desktop development with C++* workload.
- **The startup dropdown only offers "Select Startup Item…"** — configuring failed. Read the CMake output pane for the first red line.
- **The console flashes and disappears** — you pressed F5. Press Ctrl+F5.
- **`'cout' was not declared`** — missing `#include <iostream>`, or you dropped the `std::`.
- **Mystery errors that survive a rebuild** — stale cache. **Project → Delete Cache and Reconfigure**, or from a terminal delete `build/` and re-run `cmake -B build`.
- **`Permission denied` when pushing** — you cloned my repository instead of your fork. See step 1.

Bring any error you can't defeat to Monday's class. We'll fight it together.
