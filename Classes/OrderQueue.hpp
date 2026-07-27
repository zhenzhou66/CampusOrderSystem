#ifndef ORDER_QUEUE_HPP
#define ORDER_QUEUE_HPP

#include "Order.hpp"

// ============================================================
// MODULE 1: Order Queue Management
// Owner: ____________________ (fill in team member name)
// Data Structure: Queue (FIFO) - self-implemented, linked-list based
//
// Functional requirements this class must satisfy:
//   - Accept and record new student orders
//   - Maintain an ordered list of all incoming requests
//   - Process orders sequentially by arrival time
//   - Remove an order once it has been assigned to a stall
//   - Display pending orders / current order / completed history
//   - Handle empty-queue and overload edge cases
//
// NOTE: No STL containers (<queue>, <list>, etc.) allowed.
// Build the underlying storage yourself with a linked node struct
// (see OrderNode below) or a manually managed dynamic array.
// ============================================================

struct OrderNode {
    Order data;
    OrderNode* next;

    OrderNode(const Order& order) : data(order), next(nullptr) {}
};

class OrderQueue {
private:
    OrderNode* frontPtr;   // points to the front (next to be served)
    OrderNode* rearPtr;    // points to the rear (last enqueued)
    int count;             // current number of orders in queue
    int maxCapacity;       // optional cap to simulate "system overload"

    // TODO (team): add a second internal queue/array here if you
    // want to keep a separate "completed order history" list.

public:
    OrderQueue(int capacity = 100);
    ~OrderQueue();

    // Adds a new order to the rear of the queue.
    // Pseudocode:
    //   1. If count == maxCapacity -> handle overload case (reject / warn)
    //   2. Create new OrderNode from the given order
    //   3. If queue empty, frontPtr = rearPtr = newNode
    //      else rearPtr->next = newNode; rearPtr = newNode
    //   4. count++
    bool enqueue(const Order& newOrder);

    // Removes and returns the order at the front of the queue
    // (this is the order about to be handed to Stall Assignment).
    // Pseudocode:
    //   1. If isEmpty() -> handle empty-queue case (return sentinel / throw)
    //   2. Save frontPtr->data into a temp Order
    //   3. Advance frontPtr = frontPtr->next; delete old node
    //   4. If frontPtr == nullptr, rearPtr = nullptr too
    //   5. count--; return temp
    Order dequeue();

    // Looks at the front order without removing it.
    Order peekFront() const;

    // True if there are no pending orders.
    bool isEmpty() const;

    // True if maxCapacity reached (simulate peak-hour overload).
    bool isFull() const;

    // Current number of pending orders.
    int size() const;

    // Prints all pending orders from front to rear, in order.
    // Pseudocode: walk the linked list from frontPtr to nullptr,
    // printing each order's fields.
    void displayPending() const;
};

#endif // ORDER_QUEUE_HPP
