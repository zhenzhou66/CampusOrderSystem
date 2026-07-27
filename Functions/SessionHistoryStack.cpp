#include "../Classes/SessionHistoryStack.hpp"
#include <iostream>

// ============================================================
// Implementation skeleton only - NO working logic provided.
// Follow the pseudocode comments in SessionHistoryStack.hpp.
// This is teammate territory - fill in the TODOs below.
// ============================================================

SessionHistoryStack::SessionHistoryStack(int limit) {
    // TODO: topPtr = nullptr; count = 0; maxSteps = limit;
    topPtr = nullptr;
    count = 0;
    maxSteps = limit;
}

SessionHistoryStack::~SessionHistoryStack() {
    // TODO: pop until empty, deleting each node (avoid memory leaks)
}

void SessionHistoryStack::recordStep(const SessionStep& step) {
    // TODO: implement using the pseudocode in the header.
}

SessionStep SessionHistoryStack::undoLastStep() {
    // TODO: implement using the pseudocode in the header.
    return SessionStep();
}

SessionStep SessionHistoryStack::currentState() const {
    // TODO: return topPtr->data if not empty; else handle empty case
    return SessionStep();
}

bool SessionHistoryStack::isEmpty() const {
    // TODO: return count == 0
    return true;
}

int SessionHistoryStack::size() const {
    // TODO: return count
    return 0;
}

void SessionHistoryStack::displayHistory() const {
    // TODO: traverse from topPtr to nullptr, printing each step's description
}
