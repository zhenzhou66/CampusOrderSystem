# Campus Food Court Self-Order Kiosk System — Lab Work #2

A C++ prototype for APU's smart self-order kiosk system. Students scan
their ID, browse the menu, and place orders that are queued fairly,
routed to a stall on a rotating basis, and tracked through to
fulfilment. All four required data structures are self-implemented —
no STL containers (`<queue>`, `<list>`, `<stack>`, etc.) are used
anywhere in the project.

## Team & module ownership

| Module                               | Data Structure | Owner          |
|---------------------------------------|-----------------|----------------|
| Order Queue Management                | Queue           | Tai Zhen Zhou |
| Stall Assignment                      | Circular Queue  | Tan Zheng Da   |
| Kiosk Session History and Navigation  | Stack           | Lew Li Jun     |
| Menu Item Search and Management       | BST             | Gubaeva Liliia |

All four modules are complete, tested, and wired together end-to-end
in `main.cpp`.

## Folder structure

Classes/
  Order.hpp                 shared struct used by all modules
  OrderQueue.hpp             Module 1 — Queue
  StallCircularQueue.hpp     Module 2 — Circular Queue
  SessionHistoryStack.hpp    Module 3 — Stack
  MenuBST.hpp                Module 4 — Binary Search Tree

Functions/
  OrderQueue.cpp              Module 1 — enqueue, dequeue, completed history
  StallCircularQueue.cpp      Module 2 — rotating stall assignment, skip closed/busy stalls
  SessionHistoryStack.cpp     Module 3 — record/undo kiosk steps
  MenuBST.cpp                 Module 4 — insert, search, remove, sorted listing
  main.cpp                    Wires all four modules together in one workflow

## How to build and run

bash
g++ -std=c++17 main.cpp Functions/*.cpp -o food_kiosk.exe
./food_kiosk.exe

This compiles `main.cpp` together with all four module `.cpp` files in
one command — all of them must be passed to the compiler at once,
since the headers in `Classes/` only declare the functions and the
`.cpp` files in `Functions/` define them.

## Module summaries (for Q&A / justification)

**Order Queue Management — Queue (linked list, FIFO)**
Self-implemented as a singly linked list of `OrderNode`. `enqueue`
adds to the rear, `dequeue` removes from the front — this hands an
order off to Stall Assignment in the exact order it arrived, which is
what fairness requires. A second, separate linked list inside the
same class logs fulfilled orders (`markCompleted`) so pending and
completed orders can be displayed separately; this is internal
bookkeeping, not a second graded data structure. Edge cases handled:
empty queue (on `dequeue`/`peekFront`) and a capacity cap to simulate
peak-hour overload (on `enqueue`).

**Stall Assignment — Circular Queue (fixed-size array, rotation
pointer)**
A fixed-size array of `Stall` with a `currentIndex` rotation pointer
that wraps around (`% capacity`) after each assignment, so workload
is distributed evenly rather than always starting from stall 0. Skips
stalls marked `BUSY` or `CLOSED`, checking at most `capacity` stalls
per call so it can never spin forever if every stall is unavailable.
A separate fixed-size circular log (`assignmentHistory`) records every
order → stall assignment, overwriting the oldest entry once full.

**Kiosk Session History and Navigation — Stack (linked list, LIFO)**
Implemented as a singly linked list where `topPtr` always points to
the most recent step. `recordStep` pushes; `undoLastStep` pops and
returns the removed step, mirroring a browser's back button. Edge
cases handled: undoing on an empty history, and an optional step
limit (`maxSteps`) to bound memory use during a very long session.

**Menu Item Search and Management — Binary Search Tree**
Items are keyed by `itemId`. `insertItem`/`searchById`/`removeItem`
all use standard recursive BST logic, giving average O(log n) lookup
instead of a linear scan. `removeItem` handles all three removal
cases (leaf, one child, two children via in-order successor).
`displaySorted` does an in-order traversal, which naturally returns
items sorted by ID with no extra sorting step. Duplicate inserts
update the existing record in place rather than creating a second
node with the same key.
