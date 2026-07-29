#ifndef ORDER_QUEUE_HPP
#define ORDER_QUEUE_HPP

#include "Order.hpp"


struct OrderNode {
    Order data;
    OrderNode* next;

    OrderNode(const Order& order) : data(order), next(nullptr) {}
};

class OrderQueue {
private:
    OrderNode* frontPtr;   // front of the queue (next order to be served)
    OrderNode* rearPtr;    // rear of the queue (last order enqueued)
    int count;             // current number of pending orders
    int maxCapacity;       // cap used to simulate peak-hour "system overload"

    OrderNode* completedFront;
    OrderNode* completedRear;
    int completedCount;

public:
    OrderQueue(int capacity = 100);
    ~OrderQueue();

    // Adds a new order to the rear of the queue (FIFO enqueue).
    bool enqueue(const Order& newOrder);

    // Removes and returns the order at the front of the queue -
    // this order is then handed off to the Stall Assignment module.
    Order dequeue();

    // Looks at the front order without removing it.
    Order peekFront() const;

    bool isEmpty() const;
    bool isFull() const;
    int size() const;

    // Appends a fulfilled order to the completed-order history log.
    void markCompleted(const Order& finishedOrder);

    // Prints all pending orders from front to rear, in arrival order.
    void displayPending() const;

    // Prints every order that has been marked completed so far.
    void displayCompleted() const;
};

#endif // ORDER_QUEUE_HPP
