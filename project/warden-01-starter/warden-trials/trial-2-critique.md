# Trial II — *The Critique*

*Warden of the Foundations · Midterm 1 · 15 %*

This trial **requires** you to use an LLM. You will paste the prompt and the reply, then critique what you got. Reading and judging AI output is part of the job now; we are practicing it on purpose.

---

## The prompt

Open Claude (or your LLM of choice) and paste **this exact prompt**:

> *Write a C++ function template that returns the largest element of a `std::vector<T>`. Handle the case where the vector is empty. Briefly explain your design choices.*

Tell us which tool you used and the date:

- **Tool: Claude
- **Date of the conversation: 2026-09-15

---

## The LLM's reply

Paste the **full reply**, code and prose. Use a fenced code block for the code. Do not edit the reply — we want to see what you actually got.

```cpp
cpp
#include <vector>
#include <optional>
#include <algorithm>

template <typename T>
std::optional<T> maxElement(const std::vector<T>& vec) {
    if (vec.empty()) {
        return std::nullopt;
    }
    return *std::max_element(vec.begin(), vec.end());
}
cpp
// Usage
std::vector<int> nums{3, 7, 2, 9, 4};
if (auto result = maxElement(nums)) {
    std::cout << "Max: " << *result << '\n';
} else {
    std::cout << "Vector is empty\n";
}
Alternative version if you'd rather signal the empty case as a hard error instead of a normal outcome:
cpp
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename T>
T maxElement(const std::vector<T>& vec) {
    if (vec.empty()) {
        throw std::invalid_argument("maxElement: vector is empty");
    }
    return *std::max_element(vec.begin(), vec.end());
}
```

(paste any prose explanation here, verbatim)
Explanation
Return type. An empty vector has no largest element — that's a legitimate state, not necessarily an error. std::optional<T> makes this explicit in the signature: callers can't silently ignore the possibility of emptiness the way they could with a sentinel value (like -1 or T{}), and it avoids exception overhead for something that's often just a normal branch in control flow. It also composes cleanly with if (auto result = ...).
The throwing version is a reasonable alternative if an empty vector represents a genuine bug in your context rather than a case you expect callers to handle.
std::max_element over a hand-rolled loop. It's the standard, well-tested approach, works with any type supporting operator< (or a custom comparator), and communicates intent clearly. It only requires a ForwardIterator, so the signature generalizes easily to iterators instead of std::vector specifically if you want that later.
Const reference parameter. const std::vector<T>& avoids copying the vector just to inspect it and signals the function doesn't modify the input.
Caveat. T must support operator< for std::max_element to work. For types without a natural ordering, pass a custom comparator as a third argument.

---

## My critique  *( ≥ 150 words )*

Your critique must read like an engineer reviewing a colleague's pull request — specific, technical, and naming exactly what the LLM did. A vague critique ("looks fine to me, but it could be better") is *not* full credit even if the LLM's code was great.

Address each of these:

- **Compile.** Did you actually try to compile and run the code? On what types? What happened?
- **Empty case.** What does the empty-vector case actually do in this code? Is the LLM's choice reasonable? Is it different from what *you* would have chosen?
- **Requirements on `T`.** What does the code require of `T`? What happens if you instantiate it with a type that doesn't support `<` (e.g., a custom struct)?
- **Const-correctness.** Pass by value or by reference? Return by value or by reference? Would you change either?
- **`std::max_element`?** Did the LLM cheat by using the algorithm header, or did it write the loop? Which would *you* have shipped, and why?
- **One concrete improvement.** Name one specific change you would make before merging this into a real project. Not "make it better" — a concrete code change.

(your critique — at least 150 words)

I tested the code that Claude gave me with a vector of int values and it compiled and ran. It returned the correct value and when I test an empty vector, it returned std::nullopt. Claude gave me two different ways to handle an empty vector. One uses std::optional and returns nullopt if the the vector is empty. The other throws an exception instead. In my opinion I think the first option is better because having an empty vector doesn't always mean there is a problem. The code requires 'T' to support < because std::max_element uses it to compare values. If I instantiate it with a type that doesn't support '<' then the code would not compile. The vector is passed by const reference, meaning it cannot change the original. The result is returned by value, which I would keep. Claude used std::max_element insead of writing a loop. I would use the same approach because it is already built into C++. One change I would make is adding a custom comparator so that it can work with types that do not have a < comparison
