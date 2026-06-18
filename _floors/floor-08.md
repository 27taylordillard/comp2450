---
title: The Hall of Lineages
floor_number: Floor 8
flavor_quote: "A line remembers what came before it. A tree remembers where everything belongs."
week: 10
zybook: "Chapter 8 (Trees) — full chapter; class focus §8.1–8.2"
---

## The briefing

The marshalling yard behind the Warden of the Middle Gates empties into a stair that climbs instead of descends — the first time the Hold has asked you to go *up*. At the top is a long scriptorium, and one wall of it is a single chart that spreads across the stone like the roots of an overturned tree: one name at the top, lines branching down to names beneath it, those branching again. A grey-robed scholar stands at the foot of it with a stick of charcoal, adding a name to a twig near the bottom. He does not turn around. "You came up from the line," he says. "Good. Down there, order was a *sequence* — one thing after another. Up here, order is a *place*. Every creature in the Hold belongs somewhere on this chart, and somewhere is not a position in a line. It is a lineage."

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/mccown.svg' | relative_url }}" alt="Scrivener McCown, Keeper of the Bestiary — an aging scholar-monk in grey robes with ink-stained fingers and spectacles, a charcoal stick in one hand, standing before a vast branching classification chart drawn on the wall, an open ledger of monsters under his arm."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Scrivener McCown, Keeper of the Bestiary</span>

You met my Bestiary on the first floor — a *flat* list, every beast in a row, alphabetised so you could search it. A row was enough then. It is not enough now. A Skeleton is an undead thing; so is a Lich; so is a Wraith — but a Wraith has no *body*, and that matters. The list could not say so. A tree can. Here a thing's *parent* is its kind, and its parent's parent is its kind-of-kind, all the way up to the one name that has no parent at all. Give me a creature and I will read you its lineage from the root. Give me a category and I will read you everything filed beneath it. *Bring me your questions.*
</div>
</div>

For two floors you built containers by *adapting* a `Chain<T>`. A `Stack<T>` was a chain you only touched at one end; a `Queue<T>` was a chain you touched at both. The shape underneath never changed — it was always a *line*. This week the shape itself changes. A node no longer has *one* `next`. It has a whole `Bag` of children. The line bends into a branch, the branch into a fan, and the fan into a **tree**: the first non-linear structure you will build, and the one every structure after it is a variation on.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/tree-anatomy.svg' | relative_url }}" alt="A seven-node tree A through G. A is the root with no parent; B, C, D are its children on level 1; E and F hang under B and G hangs under D on level 2. The childless nodes C, E, F, G are leaves. Lines between nodes are edges. A right-hand axis marks level and depth per row; a bracket marks the tree's height as 2."/>
  <figcaption>The whole vocabulary of §8.1 on one picture. <strong>Root</strong>: the one node with no parent. <strong>Leaf</strong>: a node with no children. <strong>Edge</strong>: the link from a parent to a child. <strong>Depth</strong> of a node: edges from the root down to it. <strong>Height</strong> of the tree: the largest depth — here 2.</figcaption>
</figure>

You will learn three things this week, and the third is the quiet payoff to your last two floors:

1. **A tree is recursion made of pointers.** A tree *is* a root node plus a bag of subtrees — and each subtree is *itself* a root node plus a bag of subtrees. That self-similarity is why almost every tree operation is the same three-line shape: handle the empty case, do something with this node, then **recurse into each child**. You will write that shape four times this week — count, height, pre-order, search — and by the fourth it writes itself.
2. **The same tree has more than one order.** A line has exactly one order: front to back. A tree has several. *Depth-first* dives down a branch before moving sideways; *breadth-first* sweeps one whole level before the next. Same nodes, same edges — different answer to "what comes next?" Choosing the walk *is* choosing the order.
3. **Breadth-first is the promise Floor 7 made coming due.** On the Cisterns you were told a queue is half of a tree walk you had not written yet. This is that walk. To visit a tree closest-first, you enqueue the root, then loop: dequeue a node, record it, enqueue its children. The `Queue<T>` you built last week is *exactly* the bookkeeping that makes closest-first happen. You already own it; this week you spend it.

<figure class="diagram">
  <img src="{{ '/assets/diagrams/dfs-vs-bfs.svg' | relative_url }}" alt="Two side-by-side panels show the same seven-node tree. The left panel numbers the nodes in depth-first pre-order A, B, E, F, C, D, G and is tagged 'recursion (a hidden call stack)'. The right panel numbers them in breadth-first level-order A, B, C, D, E, F, G and is tagged 'an explicit Queue'. Caption: same tree, recursion gives depth-first, a queue gives breadth-first."/>
  <figcaption>One tree, two walks. The only difference in your code is <strong>recursion</strong> (depth-first — the call stack does the remembering for you) versus <strong>a queue</strong> (breadth-first — you do the remembering, in a <code>Queue&lt;T&gt;</code>). Node <code>E</code> is visited 3rd one way and 5th the other.</figcaption>
</figure>

## Objectives

By the end of Floor 8 you will be able to:

- Define *root*, *parent*, *child*, *leaf*, *edge*, *depth*, *level*, and *height* on a concrete tree, and say why exactly one node (the root) has no parent and why a leaf is just "a node with an empty bag of children."
- Implement `addChild` on a `Tree<T>` — allocate a node, set its parent back-pointer, push it onto the parent's children — and explain why the parent pointer is the tree's answer to `Chain<T>`'s `prev`.
- Write the recursive *count* and *height* of a tree and articulate the shared skeleton: base case for `nullptr`, act on the node, recurse into each child, combine.
- Implement a depth-first **pre-order** traversal recursively, and explain how moving one line (the `push_back`) from before the loop to after it turns pre-order into post-order.
- Implement a breadth-first **level-order** traversal *iteratively* using your Floor 7 `Queue<T>`, and explain — in terms of enqueue/dequeue order — why it visits the tree closest-first.
- Read both traversals' output on the same tree and predict which order a given node appears in for each.
- Implement a recursive *search* that returns a node by value, then use the parent pointers to walk that node's **lineage** back to the root — the tree's analogue of `Chain<T>`'s reverse walk.
- Explain why a general tree is *not yet* a fast search structure (you scan every child), and name what extra rule the next floor adds to fix that.

## Pre-class

### Reading (ZyBook Ch. 8, *Trees* — the whole chapter)

**Read all of Chapter 8.** That includes the binary-search-tree and trie sections we will not touch in class until later floors. The Hold spends three more floors inside this chapter; reading it once, whole, now is how you see where this week fits in the larger map. We map *class time* to specific sections below — but the **reading assignment is the entire chapter**, and the pre-class checks may draw from any of it.

The sections class leans on this week, and what to watch for in each:

- Before **Monday** — §8.1 *Binary trees*. Read it for the **vocabulary**, not the "binary" part. Root, leaf, internal node, parent, ancestors, edge, depth, level, height: every one of those terms applies to *any* tree, not just two-child ones. We build the general case first; the two-children-with-a-rule special case comes a floor or two on.
- Before **Wednesday** — §8.2 *Applications of trees*. Watch the animation "*a file system is a hierarchy that can be represented by a tree*." A folder containing files and other folders is exactly the structure you are building: a node with a bag of children. McCown's classification is a file system for monsters.
- Before **Friday** — re-read the §8.1 definitions of **depth** and **height** and the line *"a node's ancestors include the node's parent, the parent's parent, etc., up to the tree's root."* That sentence *is* the `lineage` algorithm — read it as pseudocode.

The rest of the chapter (§8.3 onward — BST search, insert, remove, inorder traversal, recursion, tries) is **not** wasted pre-reading: it is the next two floors. Reading it now means that when we get there, you are reviewing, not meeting it cold.

> **A note on "binary."** The ZyBook chapter is mostly about *binary* trees (≤ 2 children) and *binary search* trees. We are deliberately starting with the **general** tree — any number of children, no ordering rule — because the vocabulary and the recursion are clearest there. The next floors add the two constraints (at most two children; left < node < right) that turn this loose tree into a `BST`, and you will see exactly what each constraint buys.

There are no pre-class videos. Class time is for live coding and discussion together — your reading is the prep.

## In-class (MWF)

| Day | Focus | Activity |
|-----|-------|----------|
| **M** | Build the tree + count/height | Open `Tree.h`. Read the **provided** recursive destructor and copy ctor aloud — note the shape: *for each child, recurse; then handle this node.* Then write `addChild` together (three lines; the only catch is remembering to pass `parent` so the back-pointer is set). Then write `countFrom` and `heightFrom` — same recursion shape as the destructor you just read. Run `selftest tree` (phases 2–4 go green) and `taxonomy` — the whole classification outline prints, because the loader could finally hang children off the root. |
| **W** | Two traversals, two tools | Write `preOrderInto` — *push the node, then recurse into each child.* Run `taxonomy --dfs` and point out it lists names in the **same order** as the indented `taxonomy` outline — because that outline *is* a pre-order walk, just with indentation instead of a flat list. Then write `levelOrder` — the one method that is **not** recursive. Put `preOrder` and `levelOrder` on the projector side by side: one uses the call stack you can't see; the other uses a `Queue<Node*>` you can. Run `taxonomy --bfs` right after and let the class read the two orders aloud — `Undead` appears 2nd both times, but `Skeleton` is early in DFS and late in BFS. **30-second tweak:** move `out.push_back` in `preOrderInto` to *after* the loop — that's post-order — run it, then put it back. |
| **F** | Search + lineage (walking up) | Write `findFrom` — recursive search returning the matching node or `nullptr`, with the `if (const Node* hit = findFrom(...)) return hit;` early-return idiom. The public `lineage` is already written for you: it calls `findFrom`, then walks `parent` pointers up to the root and reverses. Run `lineage Lich`, `lineage Frostmaw`, `lineage Goblin` (the last one is *not classified* — discuss why that's the not-found path working, not a bug). Close by asking: how many nodes did `findFrom` have to check? Why is that the problem the next floor exists to solve? |

## The project — Floor 8

This week's increment is **a general `Tree<T>` and three things you do with it**: load McCown's classification from a file, walk it two different ways, and trace any creature's lineage back to the root.

You will receive (in your starter drop):

- Everything through Floor 7, fully working — `Bag<T>`, `Chain<T>` with iterators, `Stack<T>`, `Queue<T>`, `hotPotato`, `simulate`, `lint`, `undo`, every benchmark, the boss battle, every earlier self-test. Floor 7's stubs are now reference code.
- A new `hero/Tree.h` with **six method bodies stubbed**: `addChild`, `countFrom` (behind `size`), `heightFrom` (behind `height`), `preOrderInto` (behind `preOrder`), `levelOrder`, and `findFrom` (behind `lineage`). The `Node` struct, the root accessors, `makeRoot`, and the **recursive Rule of Three** are provided — read them as worked examples of the recursion you are about to write.
- A new `hero/TreeTests.cpp` harness — `selftest tree` exercises `Tree<int>` across seven phases against one fixed sample tree, with a diagnostic on each FAIL naming the likely-stubbed method. Phase 5 (pre-order) vs. Phase 6 (level-order) is the depth-first/breadth-first contrast encoded as two expected vectors.
- A new `hero/Taxonomy.h`/`.cpp` (provided framework): `loadTaxonomy` builds a `Tree<std::string>` from `data/taxonomy.txt`, and `printTaxonomy` prints it as an indented outline. Read `printTaxonomy`'s recursion before you write `preOrderInto` — same walk, minus the vector, plus indentation.
- A `data/taxonomy.txt` of one root-to-leaf path per line. Editable: the tree's whole shape is just the set of paths in that file.
- A modified `main.cpp` with `taxonomy`, `taxonomy --dfs`, `taxonomy --bfs`, `lineage <name>`, and `selftest tree` wired in. `Hero` is **unchanged** this week — the classification belongs to the catalogue, so it lives as a local `Tree<std::string>` in `main`, the way `bestiary` does.

You will write:

1. **Monday:** `addChild`, `countFrom`, `heightFrom` in `hero/Tree.h`. After this, `selftest tree` passes phases 2–4 and `taxonomy` prints the full outline with a correct "(N creatures and categories; height H)" footer.
2. **Wednesday:** `preOrderInto` and `levelOrder`. After this, `taxonomy --dfs` and `taxonomy --bfs` print real, *different* lists, and self-test phases 5–6 pass. `levelOrder` must use your `Queue<T>`.
3. **Friday:** `findFrom`. After this, `lineage <name>` prints a real root-to-creature path and phase 7 passes.

Demo target (Friday):

```
> selftest tree
  empty Tree<int>: empty() && size==0 && height==0: OK
  addChild: root==1 with 3 children: OK
  size()==7 over the sample tree: OK
  height()==2 (longest path 1->2->5): OK
  preOrder == {1,2,5,6,3,4,7} (depth-first): OK
  levelOrder == {1,2,3,4,5,6,7} (breadth-first): OK
  lineage(6)=={1,2,6} and lineage(404) empty: OK
  all phases OK

> taxonomy
Creatures of the Hold
    - Undead
        - Corporeal
            - Skeleton
            - Necrothrall
            - Lich
        - Incorporeal
            - Wraith
            - Wisp
    - Beasts
        - Mammals
            - Shadow Hound
            - Ironclaw Bear
            - Cinder Bat
        - Giants
            - Cave Troll
            - Frostmaw
    - Vermin
        - Bone Spider
        - Ratking
        - Marsh Lurker
    - Constructs
        - Stone Sentinel
  (23 creatures and categories; height 3)

> taxonomy --bfs
  breadth-first (level-order):
    Creatures of the Hold
    Undead
    Beasts
    Vermin
    Constructs
    Corporeal
    Incorporeal
    ...

> lineage Lich
  Creatures of the Hold > Undead > Corporeal > Lich
> lineage Goblin
  'Goblin' is not in McCown's catalogue.
> quit
McCown notes your departure on the chart. "You leave classified."
```

### Lab 8 — *Everything Has a Place* (folded into the project)

There is no separate lab handout. The work you do this week *is* the lab.

Commit `floor-08/lab-notes.md` to your project repo with:

1. A transcript of the demo above (with your own hero name; run `lineage` on at least three different creatures, one of them a category like `Beasts`).
2. **The recursion skeleton, in your words.** Write `countFrom`, `heightFrom`, and `preOrderInto` out and circle the three parts they share: the `nullptr` base case, the act-on-this-node line, the recurse-into-children loop. In two sentences: what is the *one* thing that differs between counting, measuring height, and listing pre-order?
3. **DFS vs BFS by hand.** On paper, take the sample tree from `selftest tree` and write out both the pre-order and the level-order sequences *before* you run them. Then run `taxonomy --dfs` and `--bfs` on the real taxonomy and confirm your hand-trace logic. One sentence: which node moved the *most* positions between the two orders, and why?
4. **The missing Goblin.** `lineage Goblin` reports "not classified" — McCown never finished filing Grix's kin. Add the Goblin to `data/taxonomy.txt` under a sensible branch (your call — argue for it in one sentence). Rebuild, run `lineage Goblin`, paste the result. Did you have to recompile any `.cpp`, or just edit the data? Why does that distinction matter?
5. **Post-order, then put it back.** Change `preOrderInto` to push the node *after* the loop instead of before. Run `taxonomy --dfs`, paste the new order, and explain in one sentence what changed and why this is now called *post*-order. Restore pre-order.
6. **Why this tree is slow to search.** Add a temporary counter to `findFrom` that counts how many nodes it visits, and call `lineage` on the *last* leaf in the file. How many nodes did it check? In two sentences: why does a general tree give you no search speedup over McCown's old flat list — and what single rule would the structure need before search could skip whole branches? (You are describing a BST. That's next floor.)
7. **One-paragraph reflection.** You have now built four containers on top of pointers — `Chain`, `Stack`, `Queue`, and `Tree`. The first three were lines; this one branches. What felt genuinely new about the tree, and what felt like the same lesson as before? (Heads up: the destructor you *read* this week, you will *write* next week, because a BST owns its nodes the same way.)

Your commit history this week should show at least three commits — Mon (`addChild`/`size`/`height` + `taxonomy`), Wed (both traversals + `--dfs`/`--bfs`), Fri (`findFrom` + `lineage` + lab notes).

<div class="character-card" markdown="1">
<div class="character-portrait">
  <img src="{{ '/assets/portraits/grix.svg' | relative_url }}" alt="Grix the Opportunist — green-skinned goblin with pointed ears, yellow eyes, gap-toothed grin, patched mismatched tunic, and a stolen brass key on a leather collar."/>
</div>
<div class="character-quote" markdown="1">
<span class="speaker">Grix the Opportunist objects to the chart</span>

McCown left Grix *off* the tree! No lineage for Grix! *Fine.* Grix made his own tree — Grix is the root, and *everything* is Grix's child. One level! Very flat! Grix can find anything in his tree instantly — it is all *right there* under Grix. ...Why are you laughing. A tree of height one is still a tree. It is also, Grix has been told, "a list wearing a hat." Grix does not see the problem.
</div>
</div>

<div class="callout bestiary" markdown="1">
<p class="callout-title">Bestiary · Floor 8</p>

**The Severed Branch** — *HP: zero. Damage: orphaned subtrees, leaked nodes, lineages that lie.*
Strikes when tree code forgets that a node *owns* the entire subtree beneath it. Delete a node without first deleting its children and the whole subtree is leaked — unreachable memory, gone until the program ends. *Reparent* a node without updating its `parent` pointer and `lineage` will happily walk up the wrong ancestors and report a confident, wrong answer. Both bugs share one cause: a tree has *two* directions of reference (down to children, up to the parent) and they must always agree.

Counter by:

- **Delete bottom-up.** The provided destructor recurses into every child *before* `delete`-ing the node. That order is not a style choice — delete the node first and you lose the only pointers to its children. Read it; it is the template for any subtree you ever free.
- **Set the parent when you set the child.** `addChild` does both in one breath: `new Node(value, parent)` *and* `parent->children.push_back(...)`. If you ever move a node, fix *both* directions or `lineage` lies.
- **The Severed Branch and Floor 4's leaks are the same monster, grown up.** A `Chain<T>` leaked a *line* of nodes if its destructor was wrong. A `Tree<T>` leaks a whole *subtree* — exponentially more, from the same one-line mistake. The deep-copy ctor has the same stakes: copy the data but not the children and you get a root pointing at someone else's subtrees.
- **A general tree does not protect you from itself.** Nothing stops you from making the same node a child of two parents, or a child of itself. The structure has no rule that forbids it; *you* are the rule. (Next floor's BST adds rules. This floor trusts you.)
</div>

<div class="callout check" markdown="1">
<p class="callout-title">Check for understanding</p>

You are now in the Trees stretch of the Hold (Floors 8–9½). Before you move on to the next floor, you should be able to answer these without looking:

1. Define root, leaf, edge, depth, and height in one sentence each, pointing at the tree-anatomy diagram. Why does the root have depth 0, and why is a one-node tree's height 0?
2. Write the shared three-line skeleton of `countFrom`, `heightFrom`, and `preOrderInto`. What is the base case they all share, and what is the *one* line that differs between them?
3. `preOrder` pushes the node *before* recursing into children; *post*-order pushes it after. On the sample tree `{1:(2:(5,6)),3,4:(7)}`, write both sequences. Which one lists a parent before its children, and which after?
4. `levelOrder` is the only traversal that is not recursive. Why does it need a `Queue<T>` specifically — what would go wrong if you used a `Stack<T>` instead? (What order would you get?)
5. Trace `lineage("Lich")` on McCown's tree. Which method finds the node, and which pointers does the walk *up* to the root follow? Why is a parent pointer required, and what `Chain<T>` feature is it the analogue of?
6. `findFrom` may have to visit *every* node before reporting "not found." Why does a general tree give no search advantage over a flat sorted list? What single ordering rule would let search skip half the tree at each step?
7. Grix's "tree of height one" really is a tree. Describe the tree that is *also* exactly a `Chain<T>` — every node has at most one child. What is its height in terms of N, and why is that the worst case for any tree algorithm?

Answers are discussed at the start of the next floor.
</div>

---

*McCown adds your name to a fresh twig at the bottom of the chart — initiate, descending, classification pending — and steps back to let you read the whole lineage above it: every adventurer who came before, branching back to the first. "You are on the tree now," he says, not unkindly. "Where you end up is still being written." Beyond the scriptorium the stair keeps climbing, toward a smaller, stranger room where the chart on the wall has only ever two branches from each node — and someone has sorted them. The Nameless Bard, somewhere, starts a verse about a tree that knows how to search itself.*
