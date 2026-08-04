#include "../Classes/SessionHistoryStack.hpp"

using namespace std;

SessionHistoryStack::SessionHistoryStack(int limit) {
    topPtr = nullptr;
    count = 0;
    maxSteps = limit;
}

SessionHistoryStack::~SessionHistoryStack() {
    while (topPtr != nullptr) {
        StepNode* temp = topPtr;
        topPtr = topPtr->next;
        delete temp;
    }
}

void SessionHistoryStack::recordStep(const SessionStep& step) {
    if (count == maxSteps) {
        cout << "Session step limit (" << maxSteps
             << ") reached — cannot record further steps." << endl;
        return;
    }

    StepNode* newNode = new StepNode(step);
    newNode->next = topPtr;
    topPtr = newNode;
    count++;
}

SessionStep SessionHistoryStack::goBack() {
    if (isEmpty()) {
        cout << "No history to go back to." << endl;
        return SessionStep("No previous step");
    }

    // Pop and discard the current step off the stack.
    StepNode* temp = topPtr;
    topPtr = topPtr->next;
    delete temp;
    count--;

    // Whatever is now on top of the stack becomes the current step.
    if (topPtr != nullptr) {
        return topPtr->data;
    }
    return SessionStep("Start of session", 0);
}

SessionStep SessionHistoryStack::peekTop() const {
    if (isEmpty()) {
        return SessionStep("No history");
    }
    return topPtr->data;
}

bool SessionHistoryStack::isEmpty() const {
    return count == 0;
}
