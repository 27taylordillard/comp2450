# Trial II — *The Critique*

*Warden of the Middle Gates · Midterm 2 · 15 %*

This trial **requires** you to use an LLM. You will paste the prompt and the reply, then critique what you got. Reading and judging AI output is part of the job now; we are practicing it on purpose.

---

## The prompt

Open Claude (or your LLM of choice) and paste **this exact prompt**:

> *Write a C++ class template `Deque<T>` that wraps a doubly-linked list and supports `push_front`, `push_back`, `pop_front`, `pop_back`, `front`, `back`, `size`, and `empty`. Each method should be O(1). Briefly explain your design choices.*

Tell us which tool you used and the date:

- **Tool:** (e.g., Claude / ChatGPT / Copilot Chat / …)
- **Date of the conversation:** YYYY-MM-DD

---

## The LLM's reply

Paste the **full reply**, code and prose. Use a fenced code block for the code. Do not edit the reply — we want to see what you actually got.

```cpp
// (paste the LLM's code here)
```

(paste any prose explanation here, verbatim)

---

## My critique  *( ≥ 150 words )*

Your critique must read like an engineer reviewing a colleague's pull request — specific, technical, and naming exactly what the LLM did. A vague critique ("looks fine to me, but it could be better") is *not* full credit even if the LLM's code was great.

Address each of these:

- **Compile.** Did you actually try to compile and run the code? On what types? What happened?
- **Is it really O(1)?** The prompt asks for all eight operations to be O(1). Audit the implementation. Did the LLM rely on `std::list` (whose `back()` and `pop_back()` are O(1) thanks to a tail pointer), or did it hand-roll storage that walks from `head` for `back()` (silently O(n))? The prose claim is not the same as the code.
- **Empty case.** What does `pop_front()` or `pop_back()` do on an empty deque? Is the LLM's choice reasonable? Compare to what your own `Queue<T>::dequeue` does in your Floor 7 code (it's a safe no-op via `Chain<T>::pop_front`).
- **Composition.** Did the LLM compose over your `Chain<T>` (the right move in your codebase), over `std::list` (also fine), or reimplement doubly-linked storage from scratch (duplication — mark down)?
- **Const-correctness.** Did the LLM give you both `T& front()` and `const T& front() const`? Both `T& back()` and `const T& back() const`? Or only the mutable overloads?
- **One concrete improvement.** Name one specific change you would make before merging this into a real project. Not "make it better" — a concrete code change.

(your critique — at least 150 words)
