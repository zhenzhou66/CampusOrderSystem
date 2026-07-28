#include "SessionHistoryStack.hpp"

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

SessionStep SessionHistoryStack::undoLastStep() {
    if (isEmpty()) {
        cout << "Session history is empty — cannot undo further." << endl;
        return SessionStep("No previous step");
    }

    SessionStep saved = topPtr->data;
    StepNode* temp = topPtr;
    topPtr = topPtr->next;
    delete temp;
    count--;

    return saved;
}

SessionStep SessionHistoryStack::currentState() const {
    if (isEmpty()) {
        return SessionStep("No active session");
    }
    return topPtr->data;
}

bool SessionHistoryStack::isEmpty() const {
    return count == 0;
}

int SessionHistoryStack::size() const {
    return count;
}

void SessionHistoryStack::displayHistory() const {
    cout << "\n--- Session History (most recent -> oldest) ---" << endl;
    StepNode* temp = topPtr;
    while (temp != nullptr) {
        cout << "  " << temp->data.description << endl;
        temp = temp->next;
    }
    cout << "List ended here!" << endl;
}