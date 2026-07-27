#include "../include/Order.hpp"
#include "../include/OrderQueue.hpp"
#include "../include/StallCircularQueue.hpp"
#include "../include/SessionHistoryStack.hpp"
#include "../include/MenuBST.hpp"
#include <iostream>

// ============================================================
// Driver program - wires the four modules together following
// the FULL SYSTEM WORKFLOW from the assignment brief:
//
//   1. A student scans their ID at the kiosk
//   2. A new order is created and queued for processing   -> OrderQueue
//   3. A stall is assigned via rotating allocation         -> StallCircularQueue
//   4. The system locates the requested menu item          -> MenuBST
//   5. Session steps are recorded as the student navigates  -> SessionHistoryStack
//   6. The student can navigate backwards through history   -> SessionHistoryStack
//   7. The order is marked fulfilled and removed from queue -> OrderQueue
//
// This file should NOT contain module-specific logic itself -
// it only calls the public methods each module exposes. Build a
// simple menu-driven loop around these calls once your modules
// are implemented.
// ============================================================

int main() {

    // --- Setup phase (run once at program start) ---
    OrderQueue orderQueue(100);
    StallCircularQueue stallQueue(4);       // e.g. Malay, Chinese, Western, +1
    SessionHistoryStack sessionHistory(50);
    MenuBST menu;

    // TODO (team, setup): register real stalls, e.g.
    //   stallQueue.addStall(0, "Malay Stall");
    //   stallQueue.addStall(1, "Chinese Stall");
    //   stallQueue.addStall(2, "Western Stall");
    //   stallQueue.addStall(3, "Beverage Stall");

    // TODO (team, setup): seed the menu with sample items, e.g.
    //   menu.insertItem(MenuItem(101, "Nasi Lemak", "Malay Stall", 6.50));
    //   menu.insertItem(MenuItem(102, "Char Kway Teow", "Chinese Stall", 7.00));

    std::cout << "=== Campus Food Court Self-Order Kiosk (Prototype) ===\n";

    // TODO (team): replace this single hard-coded pass with a real
    // menu-driven loop (e.g. switch/case on user input) that lets a
    // "student" repeatedly: browse menu, place order, undo, view status, exit.

    // --- Example single workflow pass (fill in with real calls) ---

    // Step 1-2: student scans ID -> create + enqueue order
    // Order newOrder(1, "TP0123456", /*itemId*/ 101);
    // orderQueue.enqueue(newOrder);
    // sessionHistory.recordStep(SessionStep("Order created, queued"));

    // Step 3: assign a stall via rotation
    // Order current = orderQueue.dequeue();
    // stallQueue.assignNext(current);
    // sessionHistory.recordStep(SessionStep("Assigned to stall: " + current.stallName));

    // Step 4: look up the requested item
    // MenuItem* item = menu.searchById(current.itemId);
    // if (item != nullptr) { /* display item details */ }

    // Step 5-6: record further session steps / demonstrate undo
    // sessionHistory.recordStep(SessionStep("Reviewed order summary"));
    // SessionStep lastStep = sessionHistory.undoLastStep();  // demo back navigation

    // Step 7: mark fulfilled (update status, order already left the active queue)
    // current.status = OrderStatus::FULFILLED;

    // TODO (team): print final state of each module for the demo, e.g.
    //   orderQueue.displayPending();
    //   stallQueue.displayStallStatus();
    //   menu.displaySorted();
    //   sessionHistory.displayHistory();

    std::cout << "Prototype skeleton ready. Implement the TODOs above.\n";
    return 0;
}
