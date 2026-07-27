#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

// ============================================================
// Shared Order type
// ------------------------------------------------------------
// Every module (Queue, Circular Queue, Stack, BST, Tree) passes
// this SAME struct around so the four/five modules can plug into
// one prototype without needing to know each other's internals.
//
// Agree on this struct as a TEAM before anyone starts coding their
// own module - do not let each member invent their own version.
// ============================================================

enum class OrderStatus {
    PENDING,     // waiting in the Order Queue
    ASSIGNED,    // stall has been allocated (Circular Queue)
    PREPARING,   // stall is working on it
    FULFILLED    // completed and removed from active queue
};

struct Order {
    int orderId;
    std::string studentId;
    int itemId;          // links to a MenuItem in the BST
    std::string stallName;    // filled in once StallCircularQueue assigns it
    OrderStatus status;
    // TODO (team): add any extra fields your modules need,
    // e.g. timestamp, quantity, counter/zone location, etc.

    Order()
        : orderId(0), studentId(""), itemId(-1),
          stallName(""), status(OrderStatus::PENDING) {}

    Order(int id, const std::string& student, int item)
        : orderId(id), studentId(student), itemId(item),
          stallName(""), status(OrderStatus::PENDING) {}
};

#endif // ORDER_HPP
