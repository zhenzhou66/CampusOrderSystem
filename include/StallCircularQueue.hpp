#ifndef STALL_CIRCULAR_QUEUE_HPP
#define STALL_CIRCULAR_QUEUE_HPP

#include "Order.hpp"
#include <string>

// ============================================================
// MODULE 2: Stall Assignment
// Owner: ____________________ (fill in team member name)
// Data Structure: Circular Queue - self-implemented, fixed-size array
//
// Functional requirements this class must satisfy:
//   - Maintain a list of all stalls and their status (available/busy/closed)
//   - Assign incoming orders to stalls in continuous rotation
//   - Skip stalls that are currently closed or at capacity
//   - Track order assignments per stall
//   - Rotate without restarting the cycle (classic circular queue index wrap)
// ============================================================

enum class StallStatus {
    AVAILABLE,
    BUSY,
    CLOSED
};

struct Stall {
    std::string name;
    StallStatus status;
    int ordersHandled;   // running count, for "track assignments per stall"

    Stall() : name(""), status(StallStatus::AVAILABLE), ordersHandled(0) {}
    Stall(const std::string& n) : name(n), status(StallStatus::AVAILABLE), ordersHandled(0) {}
};

class StallCircularQueue {
private:
    Stall* stalls;        // fixed-size array of stalls (the "circle")
    int capacity;          // total number of stalls
    int currentIndex;      // rotation pointer - where we left off last time

    // TODO (team): decide if you need a separate array/queue to store
    // "order -> stall" assignment history for the "track assignments" requirement.

public:
    StallCircularQueue(int numStalls);
    ~StallCircularQueue();

    // Register a stall name at a given slot when the system starts up.
    void addStall(int index, const std::string& name);

    // Finds the next AVAILABLE stall starting from currentIndex, wrapping
    // around the array, and assigns the given order to it.
    // Pseudocode:
    //   1. Start scanning from currentIndex
    //   2. Loop at most `capacity` times (to avoid infinite loop if all closed):
    //        a. If stalls[currentIndex].status == AVAILABLE:
    //             - assign order.stallName = stalls[currentIndex].name
    //             - stalls[currentIndex].ordersHandled++
    //             - advance currentIndex = (currentIndex + 1) % capacity
    //             - return true (assigned)
    //        b. Else: currentIndex = (currentIndex + 1) % capacity; continue
    //   3. If loop completes with no available stall -> handle "all stalls
    //      busy/closed" edge case, return false
    bool assignNext(Order& order);

    // Manually mark a stall open/closed/busy (e.g. lunch rush, staff break).
    void setStallStatus(int index, StallStatus newStatus);

    // Prints every stall's name, status, and orders handled so far.
    void displayStallStatus() const;
};

#endif // STALL_CIRCULAR_QUEUE_HPP
