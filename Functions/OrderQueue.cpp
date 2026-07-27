#include "../include/OrderQueue.hpp"
#include <iostream>

// ============================================================
// Implementation skeleton only - NO working logic provided.
// Follow the pseudocode comments in OrderQueue.hpp to implement
// each function yourself so you can explain it during Q&A.
// ============================================================

OrderQueue::OrderQueue(int capacity) {
    // TODO: initialise frontPtr = nullptr, rearPtr = nullptr,
    // count = 0, maxCapacity = capacity
}

OrderQueue::~OrderQueue() {
    // TODO: walk the linked list and delete every remaining node
    // to avoid memory leaks (loop calling dequeue() until empty,
    // or manually delete node by node).
}

bool OrderQueue::enqueue(const Order& newOrder) {
    // TODO: implement using the pseudocode in the header.
    return false;
}

Order OrderQueue::dequeue() {
    // TODO: implement using the pseudocode in the header.
    return Order();
}

Order OrderQueue::peekFront() const {
    // TODO: return frontPtr->data if not empty; otherwise handle
    // the empty-queue case (e.g. print message, return sentinel Order).
    return Order();
}

bool OrderQueue::isEmpty() const {
    // TODO: return true if count == 0 (or frontPtr == nullptr)
    return true;
}

bool OrderQueue::isFull() const {
    // TODO: return true if count == maxCapacity
    return false;
}

int OrderQueue::size() const {
    // TODO: return count
    return 0;
}

void OrderQueue::displayPending() const {
    // TODO: traverse from frontPtr to nullptr, print each order.
    // Example loop shape:
    //   OrderNode* current = frontPtr;
    //   while (current != nullptr) {
    //       // print current->data fields
    //       current = current->next;
    //   }
}
