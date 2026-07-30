#include "../Classes/SessionHistoryStack.hpp"

using namespace std;

SessionHistoryStack::SessionHistoryStack(int limit) {
    topPtr = nullptr;
    count = 0;
    maxSteps = limit;
    forwardTopPtr = nullptr;
    forwardCount = 0;
}

SessionHistoryStack::~SessionHistoryStack() {
    while (topPtr != nullptr) {
        StepNode* temp = topPtr;
        topPtr = topPtr->next;
        delete temp;
    }
    while (forwardTopPtr != nullptr) {
        StepNode* temp = forwardTopPtr;
        forwardTopPtr = forwardTopPtr->next;
        delete temp;
    }
}

void SessionHistoryStack::recordStep(const SessionStep& step) {
    if (count == maxSteps) {
        cout << "Session step limit (" << maxSteps
             << ") reached — cannot record further steps." << endl;
        return;
    }

    // Taking a fresh action invalidates whatever "forward" branch existed,
    // same as a browser: visiting a new page clears the forward history.
    while (forwardTopPtr != nullptr) {
        StepNode* temp = forwardTopPtr;
        forwardTopPtr = forwardTopPtr->next;
        delete temp;
    }
    forwardCount = 0;

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

    // Pop the current step off the main stack and push it onto the
    // forward stack, so goForward() can bring it back later.
    StepNode* temp = topPtr;
    topPtr = topPtr->next;
    count--;

    temp->next = forwardTopPtr;
    forwardTopPtr = temp;
    forwardCount++;

    // Whatever is now on top of the main stack becomes the current step.
    if (topPtr != nullptr) {
        return topPtr->data;
    }
    return SessionStep("Start of session", 0);
}

SessionStep SessionHistoryStack::goForward() {
    if (forwardTopPtr == nullptr) {
        cout << "No forward history available." << endl;
        return SessionStep("No forward step");
    }

    // Pop from the forward stack and push it back onto the main stack.
    StepNode* temp = forwardTopPtr;
    forwardTopPtr = forwardTopPtr->next;
    forwardCount--;

    temp->next = topPtr;
    topPtr = temp;
    count++;

    return topPtr->data;
}

bool SessionHistoryStack::canGoForward() const {
    return forwardTopPtr != nullptr;
}

bool SessionHistoryStack::isEmpty() const {
    return count == 0;
}
