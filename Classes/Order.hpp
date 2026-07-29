#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>


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

    Order()
        : orderId(0), studentId(""), itemId(-1),
          stallName(""), status(OrderStatus::PENDING) {}

    Order(int id, const std::string& student, int item)
        : orderId(id), studentId(student), itemId(item),
          stallName(""), status(OrderStatus::PENDING) {}
};

#endif // ORDER_HPP
