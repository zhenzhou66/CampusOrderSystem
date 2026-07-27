# Campus Food Court Self-Order Kiosk System — Lab Work #2 Skeleton

This is a **scaffold only**. Every function body is a `TODO` with pseudocode
comments — no working logic has been written. Per the assignment's AI usage
policy (Amber tier), your team must write the actual algorithms yourselves so
each member can explain and justify their code during the Q&A.

## Folder structure

```
include/            header files (class declarations, function signatures, pseudocode)
  Order.hpp                shared struct used by all modules — agree on this first
  OrderQueue.hpp            Module 1 — Queue
  StallCircularQueue.hpp    Module 2 — Circular Queue
  SessionHistoryStack.hpp   Module 3 — Stack
  MenuBST.hpp               Module 4 — Binary Search Tree
  LayoutTree.hpp            Not used (optional 5th-member module — team is 4 people)

src/                implementation files — fill in the TODOs here
  OrderQueue.cpp
  StallCircularQueue.cpp
  SessionHistoryStack.cpp
  MenuBST.cpp
  LayoutTree.cpp            Not used — safe to ignore/delete
  main.cpp                  driver that wires the four modules together
```

## Suggested role assignment

| Module | Data Structure | Owner |
|---|---|---|
| Order Queue Management | Queue | _______________ |
| Stall Assignment | Circular Queue | _______________ |
| Kiosk Session History and Navigation | Stack | _______________ |
| Menu Item Search and Management | BST | _______________ |

## How to build (once logic is implemented)

```bash
g++ -std=c++17 -Wall -Iinclude src/OrderQueue.cpp src/StallCircularQueue.cpp src/SessionHistoryStack.cpp src/MenuBST.cpp src/main.cpp -o kiosk
./kiosk
```

`LayoutTree.hpp`/`.cpp` are left in the project in case you pick up a 5th
member later, but they're not part of the build above and `main.cpp` doesn't
include them.

## Integration notes

- Everyone builds against the same `Order` struct in `Order.hpp` — don't
  redefine it per module.
- Each module only exposes its public class methods (`enqueue`, `assignNext`,
  `recordStep`, `searchById`, etc.) — `main.cpp` is the only place that calls
  across modules, following the workflow in the assignment brief.
- No STL containers are used anywhere (`<queue>`, `<stack>`, `<vector>`,
  etc.) — every structure is built from raw pointers/nodes or manually
  managed arrays, as required.
- Test each module standalone first (write a small throwaway test in your
  own `main` while developing), then integrate into the shared `main.cpp`.

## Reminder

You must be able to explain and justify every part of your code. Declare any
AI usage per the policy (e.g. "AI was used to understand Queue syntax in
C++") — do not submit AI-generated logic as your own.
