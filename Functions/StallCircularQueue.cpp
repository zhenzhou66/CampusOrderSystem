#include "../Classes/StallCircularQueue.hpp"
#include <iostream>

// ============================================================
// Implementation skeleton only - NO working logic provided.
// Follow the pseudocode comments in StallCircularQueue.hpp.
// This is teammate territory - fill in the TODOs below.
// ============================================================

StallCircularQueue::StallCircularQueue(int numStalls) {
    // TODO: capacity = numStalls; currentIndex = 0;
    // stalls = new Stall[capacity];
    stalls = nullptr;
    capacity = 0;
    currentIndex = 0;
}

StallCircularQueue::~StallCircularQueue() {
    // TODO: delete[] stalls;
}

void StallCircularQueue::addStall(int index, const std::string& name) {
    // TODO: bounds-check index, then stalls[index] = Stall(name);
}

bool StallCircularQueue::assignNext(Order& order) {
    // TODO: implement the rotation-scan pseudocode from the header.
    return false;
}

void StallCircularQueue::setStallStatus(int index, StallStatus newStatus) {
    // TODO: bounds-check index, then stalls[index].status = newStatus;
}

void StallCircularQueue::displayStallStatus() const {
    // TODO: loop i = 0 to capacity-1, print stalls[i] name/status/ordersHandled
}
