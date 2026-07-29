# Campus Food Court Self-Order Kiosk System — Lab Work #2

Only the **Order Queue Management** module is complete (my part). The other
three modules are left as skeletons — pseudocode comments only, no working
logic — for teammates to implement themselves.

## Folder structure

```
Classes/            header files (class declarations)
  Order.hpp                shared struct used by all modules
  OrderQueue.hpp            Module 1 — Queue                
  StallCircularQueue.hpp    Module 2 — Circular Queue        
  SessionHistoryStack.hpp   Module 3 — Stack                 
  MenuBST.hpp               Module 4 — Binary Search Tree    

Functions/          implementation files
  OrderQueue.cpp            Fully implemented — enqueue, dequeue, completed history
  StallCircularQueue.cpp    TODO stub — see pseudocode in the header
  SessionHistoryStack.cpp   TODO stub — see pseudocode in the header
  MenuBST.cpp               TODO stub — see pseudocode in the header
  main.cpp                  Demonstrates the OrderQueue module only; the rest
                             of the workflow is commented out until the other
                             modules are implemented
```

## Suggested role assignment

--------------------------------------------------------------------------
| Module                               | Data Structure | Owner          |
|--------------------------------------|----------------|----------------|
| Order Queue Management               | Queue          | Tan Zheng Zhou |
|--------------------------------------|----------------|----------------|
| Stall Assignment                     | Circular Queue | Tan Zheng Da   |
|--------------------------------------|----------------|----------------|
| Kiosk Session History and Navigation | Stack          | Lew Li Jun     |
|--------------------------------------|----------------|----------------|
| Menu Item Search and Management      | BST            | Gubaeva Liliia |
--------------------------------------------------------------------------

## How to build and run (Order Queue demo)

```bash
g++ -std=c++17 -Wall -Wextra -o kiosk Functions/*.cpp
./kiosk
```

This currently just demonstrates the Order Queue module: enqueue two
orders, display pending, dequeue one, mark it completed, display completed.
Once teammates implement their modules, `main.cpp` should be replaced with a
real menu-driven loop wiring all four together (see the TODO comments at the
bottom of `main.cpp` for the intended workflow).

## How my module (OrderQueue) works

Queue (FIFO), self-implemented as a singly linked list of `OrderNode`.
`enqueue` adds to the rear, `dequeue` removes from the front — this is what
hands an order off to Stall Assignment. A second, separate linked list
inside the same class logs fulfilled orders (`markCompleted`) so pending and
completed orders can be displayed separately; this is internal bookkeeping,
not a second graded data structure. Edge cases handled: empty queue (on
dequeue/peek) and a capacity cap to simulate peak-hour overload (on
enqueue).

No STL containers (`<queue>`, `<list>`, etc.) are used — everything is built
from raw pointers/nodes, per the assignment's requirement.

