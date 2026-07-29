#ifndef STALL_CIRCULAR_QUEUE_HPP
#define STALL_CIRCULAR_QUEUE_HPP

#include "Order.hpp"
#include <string>

// ============================================================
// TASK 2: Stall Assignment
// Data Structure: Circular Queue - self-implemented, fixed-size array
// ============================================================

enum class StallStatus {
    AVAILABLE,
    BUSY,
    CLOSED
};

struct Stall {
    std::string name;
    StallStatus status;
    int ordersHandled;   // Running count, for "Track Assignments per Stall"

    Stall() : name(""), status(StallStatus::AVAILABLE), ordersHandled(0) {}
    Stall(const std::string& n) : name(n), status(StallStatus::AVAILABLE), ordersHandled(0) {}
};

// Records which order went to which stall. A simple self-built log, stored in its own fixed-size circular buffer.
struct AssignmentRecord {
    int orderId;
    std::string stallName;

    AssignmentRecord() : orderId(-1), stallName("") {}
    AssignmentRecord(int id, const std::string& s) : orderId(id), stallName(s) {}
};

class StallCircularQueue {
    private:
        Stall* stalls;          // Fixed-size array of stalls (Loop)
        int capacity;           // Total number of stalls
        int currentIndex;       // Rotation pointer - where its left off last time

        AssignmentRecord* assignmentHistory; // Log of order-> Stall assignments
        int assignmentCapacity;              // Max records the log can hold
        int assignmentCount;                 // Total assignments made so far (Wraps around assignmentCapacity)

        std::string statusToString(StallStatus s) const;

    public:
        // maxAssignments defaults to 500 so existing calls like StallCircularQueue stallQueue(4); still compile unchanged
        StallCircularQueue(int numStalls, int maxAssignments = 500);
        ~StallCircularQueue();

        // Register a stall name at a given slot when the system starts up
        void addStall(int index, const std::string& name);

        // Finds the next AVAILABLE stall starting from currentIndex, wrapping around the array, and assigns the given order to it
        bool assignNext(Order& order);

        // Manually mark a stall as open/closed/busy (e.g. lunch rush, staff break)
        void setStallStatus(int index, StallStatus newStatus);

        // Prints every stall's name, status, and orders handled so far
        void displayStallStatus() const;

        // Prints the full order -> Stall assignment log
        void displayAssignmentHistory() const;
};

#endif // STALL_CIRCULAR_QUEUE_HPP