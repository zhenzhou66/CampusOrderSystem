#include "../Classes/StallCircularQueue.hpp"
#include <iostream>

// Stall variables (Constructor)
StallCircularQueue::StallCircularQueue(int numStalls, int maxAssignments) {
    capacity = (numStalls > 0) ? numStalls : 0;
    currentIndex = 0;
    stalls = (capacity > 0) ? new Stall[capacity] : nullptr;

    assignmentCapacity = (maxAssignments > 0) ? maxAssignments : 0;
    assignmentCount = 0;
    assignmentHistory = (assignmentCapacity > 0) ? new AssignmentRecord[assignmentCapacity] : nullptr;
}

// Stall variables (Destructor)
StallCircularQueue::~StallCircularQueue() {
    delete[] stalls;
    delete[] assignmentHistory;
}

// Stall addition with error handling
void StallCircularQueue::addStall(int index, const std::string& name) {
    if (index < 0 || index >= capacity) {
        std::cout << "Error: stall index " << index << " is out of bounds.\n";
        return;
    }
    stalls[index] = Stall(name);
}

// Stall assignments with error handling
bool StallCircularQueue::assignNext(Order& order) {
    if (capacity == 0 || stalls == nullptr) {
        std::cout << "Error: no stalls registered.\n";
        return false;
    }

    // Loop at most `Capacity` times so we never spin forever if every stall is closed/busy.
    for (int i = 0; i < capacity; i++) {
        if (stalls[currentIndex].status == StallStatus::AVAILABLE) {

            // Assign the order to this stall
            order.stallName = stalls[currentIndex].name;
            order.status = OrderStatus::ASSIGNED;
            stalls[currentIndex].ordersHandled++;

            // Log the assignment (Circular overwrite once the log is full, same wrap-around idea as the stall rotation itself)
            if (assignmentCapacity > 0) {
                int slot = assignmentCount % assignmentCapacity;
                assignmentHistory[slot] = AssignmentRecord(order.orderId, stalls[currentIndex].name);
                assignmentCount++;
            }

            std::cout << "Order #" << order.orderId << " assigned to " << stalls[currentIndex].name << ".\n";

            // Advance the pointer so the NEXT call starts one slot further on
            currentIndex = (currentIndex + 1) % capacity;
            return true;
        }

        // Not available = Keep rotating
        currentIndex = (currentIndex + 1) % capacity;
    }

    // Went all the way around and found nothing available
    std::cout << "All stalls are currently busy or closed. Order #" << order.orderId << " could not be assigned.\n";
    return false;
}

// Out of bounds/range index handling
void StallCircularQueue::setStallStatus(int index, StallStatus newStatus) {
    if (index < 0 || index >= capacity) {
        std::cout << "Error: stall index " << index << " is out of bounds.\n";
        return;
    }
    stalls[index].status = newStatus;
}

// Stall status assigning, defaults to UNKNOWN
std::string StallCircularQueue::statusToString(StallStatus s) const {
    switch (s) {
        case StallStatus::AVAILABLE: return "AVAILABLE";
        case StallStatus::BUSY:      return "BUSY";
        case StallStatus::CLOSED:    return "CLOSED";
        default:                     return "UNKNOWN";
    }
}

// Displays stall's status
void StallCircularQueue::displayStallStatus() const {
    std::cout << "\n--- Stall Status Overview ---\n";
    for (int i = 0; i < capacity; i++) {
        std::cout << "[" << i << "] " << stalls[i].name << " | Status: " << statusToString(stalls[i].status) << " | Orders handled: " << stalls[i].ordersHandled << "\n";
    }
}

// Assignment history of stalls
void StallCircularQueue::displayAssignmentHistory() const {
    std::cout << "\n--- Order -> Stall Assignment Log ---\n";
    if (assignmentCount == 0) {
        std::cout << "(no assignments yet)\n";
        return;
    }

    int recordsToShow = (assignmentCount < assignmentCapacity) ? assignmentCount : assignmentCapacity;
    for (int i = 0; i < recordsToShow; i++) {
        std::cout << "Order #" << assignmentHistory[i].orderId << " -> " << assignmentHistory[i].stallName << "\n";
    }
}