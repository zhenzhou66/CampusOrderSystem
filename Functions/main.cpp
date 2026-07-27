#include "../Classes/Order.hpp"
#include "../Classes/OrderQueue.hpp"
#include "../Classes/StallCircularQueue.hpp"
#include "../Classes/SessionHistoryStack.hpp"
#include "../Classes/MenuBST.hpp"
#include <iostream>

// ============================================================
// Driver program - wires the four modules together following
// the FULL SYSTEM WORKFLOW from the assignment brief:
//
//   1. A student scans their ID at the kiosk
//   2. A new order is created and queued for processing   -> OrderQueue (done)
//   3. A stall is assigned via rotating allocation         -> StallCircularQueue (TODO - teammate)
//   4. The system locates the requested menu item          -> MenuBST (TODO - teammate)
//   5. Session steps are recorded as the student navigates  -> SessionHistoryStack (TODO - teammate)
//   6. The student can navigate backwards through history   -> SessionHistoryStack (TODO - teammate)
//   7. The order is marked fulfilled and removed from queue -> OrderQueue (done)
//
// Only the OrderQueue module (Order Queue Management) has been
// implemented here. The other three modules are left as skeletons
// for teammates to complete. This file should NOT contain
// module-specific logic itself - it only calls the public methods
// each module exposes. Build a menu-driven loop once every module
// is implemented.
// ============================================================

int main() {

    // --- Setup phase (run once at program start) ---
    OrderQueue orderQueue(100);
    StallCircularQueue stallQueue(4);       // e.g. Malay, Chinese, Western, +1
    SessionHistoryStack sessionHistory(50);
    MenuBST menu;

    // TODO (teammate, Stall Assignment): register real stalls, e.g.
    //   stallQueue.addStall(0, "Malay Stall");
    //   stallQueue.addStall(1, "Chinese Stall");
    //   stallQueue.addStall(2, "Western Stall");
    //   stallQueue.addStall(3, "Beverage Stall");

    // TODO (teammate, Menu Search): seed the menu with sample items, e.g.
    //   menu.insertItem(MenuItem(101, "Nasi Lemak", "Malay Stall", 6.50));
    //   menu.insertItem(MenuItem(102, "Char Kway Teow", "Chinese Stall", 7.00));

    std::cout << "=== Campus Food Court Self-Order Kiosk (Prototype) ===\n";

    // --- Demo of the completed Order Queue module ---
    Order orderA(1, "TP012345", 101);
    Order orderB(2, "TP099999", 103);

    orderQueue.enqueue(orderA);
    orderQueue.enqueue(orderB);
    orderQueue.displayPending();

    Order processed = orderQueue.dequeue();
    std::cout << "Dequeued order #" << processed.orderId
               << " for processing.\n";
    orderQueue.displayPending();

    // Normally the Stall Assignment module would assign a stall here,
    // e.g.:
    //   stallQueue.assignNext(processed);
    // and once fulfilled:
    //   processed.status = OrderStatus::FULFILLED;
    processed.status = OrderStatus::FULFILLED;
    orderQueue.markCompleted(processed);
    orderQueue.displayCompleted();

    // TODO (team): once Stall Assignment, Session History, and Menu Search
    // are implemented, replace the demo above with a real menu-driven loop
    // that walks through the full workflow:
    //   1-2. scan ID -> orderQueue.enqueue()
    //   3.   stallQueue.assignNext()
    //   4.   menu.searchById()
    //   5-6. sessionHistory.recordStep() / undoLastStep()
    //   7.   orderQueue.markCompleted()

    std::cout << "\nOrder Queue module demo complete. Other modules "
                 "still need to be implemented by the team.\n";
    return 0;
}
