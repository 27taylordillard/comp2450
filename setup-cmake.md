---
title: Why the Forge Needs a Blueprint
permalink: /setup/cmake/
---

# Why the Forge Needs a Blueprint

*What CMake is, why this course uses it, and why you must never commit `build/`.*

Every floor of this dungeon ships with a file called `CMakeLists.txt`. You will never be
asked to write one from scratch. But you will look at one every week for fifteen weeks, so
it's worth five minutes to understand what it's for.

## The problem

Here is what compiling C++ actually looks like, by hand:

```
cl /std:c++17 /EHsc main.cpp bestiary\Search.cpp bestiary\Bench.cpp /Fe:antechamber.exe
```

That's the Windows version. On a Mac it's a different program with different flags. On
Linux, different again. And that command is for a project with *three* source files — by
Floor 9 you'll have a dozen, and typing that line correctly every single time is not a
skill, it's a tax.

So: put the command in a file. That's a build system. It's an old idea.

The harder problem is that the *shape* of that command changes per machine. Different
compiler, different flag spellings, different library paths, different everything. Write
the build once for Windows and your classmate on a MacBook can't build it. Write it three
times and you now maintain three of them, and two are always out of date.

## What CMake does

CMake steps up one level of abstraction. You don't describe *how* to build. You describe
**what** you have and **what** you want:

```cmake
cmake_minimum_required(VERSION 3.15)
project(antechamber LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(antechamber main.cpp)
```

Read it in English: *"This project is called antechamber. It's C++. Use the C++17 standard.
Build one executable, also called antechamber, out of `main.cpp`."*

Notice what is **not** in there: no compiler name, no `/EHsc`, no `-o`, no paths. Those
are details of *your* machine, and stating them is exactly the mistake.

CMake reads that description and *generates* the real build system for whatever machine
it's running on — a Visual Studio project on your Windows laptop, a Makefile on your
classmate's Mac, all from the same five lines. That's why the same starter folder in the
same repository works for the whole class.

<div class="callout lore" markdown="1">
<p class="callout-title">The blueprint, not the building</p>

`CMakeLists.txt` is a **blueprint**. It is small, it is human-readable, and it is the
thing that gets committed to git.

The `build/` folder is the **building** — thousands of generated files, specific to your
machine, reproducible from the blueprint in about two seconds. It is never committed.
More on that below, because it's the one mistake that reliably causes trouble.
</div>

## The two commands

Everything CMake does, all semester, is two steps:

```
cmake -B build          # 1. configure: read the blueprint, generate a real build system
cmake --build build     # 2. build: run that build system, produce the .exe
```

**Configure** inspects your machine — which compilers exist, which version, where the
standard library lives — and writes out a build system tailored to what it found. Slow-ish.
You do it once.

**Build** runs the thing configure produced. Fast. You do it constantly.

You only re-run configure when the blueprint changes — a new floor, a new source file
added to `CMakeLists.txt`. The rest of the time you're just building. Visual Studio
notices `CMakeLists.txt` changing and re-configures for you.

## If you're clicking, not typing

You will mostly never type either command — Visual Studio runs them for you. But the class
notes quote commands sometimes, so here's the map between the two worlds:

| In Visual Studio you… | …which runs |
|---|---|
| **File → Open → Folder** on a starter | `cmake -B build` (configure) |
| Watch *"CMake generation finished"* | configure succeeded |
| Press **Ctrl+F5** | `cmake --build build`, then runs the `.exe` |
| Pick from the **startup item** dropdown | chooses which `.exe` to run |
| **Project → Delete Cache and Reconfigure** | `rm -rf build` then `cmake -B build` |
| **Build → Rebuild All** | `cmake --build build --clean-first` |

Same operations, same order, same names. Only the interface differs.

## Never commit `build/`

This is the one that bites.

The `build/` folder (Visual Studio calls it `out/build/`) holds object files, cached paths,
generated project files, and your compiled `.exe`. It is:

- **Enormous** — often far larger than all your source code combined.
- **Machine-specific** — full of absolute paths like `C:\Users\yourname\...` that mean
  nothing on anyone else's computer.
- **Completely disposable** — delete it and `cmake -B build` rebuilds it from scratch.

Commit it and you push hundreds of files nobody can use, your repository bloats, and every
future `git pull` fights with it.

The course `.gitignore` already excludes `build/`, `*.exe`, and `*.obj`, so under normal
circumstances git will simply refuse to notice them and you never have to think about it.
Just don't go out of your way to override that — if git ever offers to commit something
inside `build/`, say no.

<div class="callout check" markdown="1">
<p class="callout-title">The one-sentence version</p>

**`CMakeLists.txt` is the blueprint you commit; `build/` is the building you throw away and
regenerate.** If you remember only that, you'll be fine.
</div>

## The bit you'll actually be graded on

Nothing, directly. There is no CMake question on any Keeper's Check.

But `CMakeLists.txt` is how each floor's starter tells your compiler which files to build,
and from Floor 3 onward the starters have several source files. When you eventually add a
file of your own and it mysteriously doesn't get compiled, the answer is always the same:
**you added it to the folder but not to the blueprint.** Open `CMakeLists.txt`, find the
`add_executable` line, add your filename to the list.

That's the whole of what you need from CMake this semester.

---

*Next: [The Chronicle]({{ site.baseurl }}/setup/git/) — git, GitHub, and getting your work
where I can see it.*
