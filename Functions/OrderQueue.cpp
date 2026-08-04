#include "../Classes/OrderQueue.hpp"
#include <iostream>

using namespace std;

// ============================================================
// Order Queue Management - implementation
// ============================================================

OrderQueue::OrderQueue(int capacity)
    : frontPtr(nullptr), rearPtr(nullptr), count(0), maxCapacity(capacity),
      completedFront(nullptr), completedRear(nullptr), completedCount(0) {}

OrderQueue::~OrderQueue() {
    // Drain the pending queue node by node.
    while (!isEmpty()) {
        dequeue();
    }
    // Free the completed-order history list too.
    OrderNode* current = completedFront;
    while (current != nullptr) {
        OrderNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

bool OrderQueue::enqueue(const Order& newOrder) {
    if (isFull()) {
        cout << "[OrderQueue] Cannot accept order #" << newOrder.orderId
                   << " - system at peak capacity (" << maxCapacity << ").\n";
        return false;
    }

    OrderNode* newNode = new OrderNode(newOrder);
    if (isEmpty()) {
        frontPtr = newNode;
        rearPtr = newNode;
    } else {
        rearPtr->next = newNode;
        rearPtr = newNode;
    }
    count++;
    return true;
}

Order OrderQueue::dequeue() {
    if (isEmpty()) {
        cout << "[OrderQueue] No pending orders to process.\n";
        return Order(); // sentinel order (orderId 0)
    }

    OrderNode* oldFront = frontPtr;
    Order frontData = oldFront->data;

    frontPtr = frontPtr->next;
    if (frontPtr == nullptr) {
        rearPtr = nullptr; // queue just became empty
    }

    delete oldFront;
    count--;
    return frontData;
}

Order OrderQueue::peekFront() const {
    if (isEmpty()) {
        cout << "[OrderQueue] Queue is empty.\n";
        return Order();
    }
    return frontPtr->data;
}

bool OrderQueue::isEmpty() const {
    return count == 0;
}

bool OrderQueue::isFull() const {
    return count >= maxCapacity;
}

int OrderQueue::size() const {
    return count;
}

void OrderQueue::markCompleted(const Order& finishedOrder) {
    OrderNode* newNode = new OrderNode(finishedOrder);
    if (completedFront == nullptr) {
        completedFront = newNode;
        completedRear = newNode;
    } else {
        completedRear->next = newNode;
        completedRear = newNode;
    }
    completedCount++;
}

void OrderQueue::displayPending() const {
    cout << "--- Pending Orders (" << count << ") ---\n";
    if (isEmpty()) {
        cout << "  (no pending orders)\n";
        return;
    }
    OrderNode* current = frontPtr;
    while (current != nullptr) {
        cout << "  Order #" << current->data.orderId
             << " | Student: " << current->data.studentId
             << " | Item ID: " << current->data.itemId << "\n";
        current = current->next;
    }
}

void OrderQueue::displayCompleted() const {
    cout << "--- Completed Orders (" << completedCount << ") ---\n";
    if (completedFront == nullptr) {
        cout << "  (no completed orders yet)\n";
        return;
    }
    OrderNode* current = completedFront;
    while (current != nullptr) {
        cout << "  Order #" << current->data.orderId
             << " | Student: " << current->data.studentId
             << " | Stall: " << current->data.stallName << "\n";
        current = current->next;
    }
}
