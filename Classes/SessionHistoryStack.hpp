#ifndef SESSION_HISTORY_STACK_HPP
#define SESSION_HISTORY_STACK_HPP

#include <string>

// ============================================================
// MODULE 3: Kiosk Session History and Navigation
// Owner: ____________________ (fill in team member name)
// Data Structure: Stack (LIFO) - self-implemented, linked-list based
//
// Functional requirements this class must satisfy:
//   - Record each step taken in a session (menu browsed, item selected,
//     quantity updated, etc.)
//   - Store the full session history from login to current state
//   - Allow navigating BACK through steps one at a time (like a
//     browser back button) - undo / backtrack
//   - Support re-visiting a previously reached state
//   - Handle "back on empty history" and "exceeds session limit" cases
// ============================================================

struct SessionStep {
    std::string description;   // e.g. "Selected Item #12", "Viewed Menu"
    // TODO (team): add any extra state you need to restore on undo,
    // e.g. a snapshot of the current Order being built.

    SessionStep() : description("") {}
    SessionStep(const std::string& desc) : description(desc) {}
};

struct StepNode {
    SessionStep data;
    StepNode* next;   // points to the PREVIOUS step (toward the bottom of stack)

    StepNode(const SessionStep& step) : data(step), next(nullptr) {}
};

class SessionHistoryStack {
private:
    StepNode* topPtr;      // most recent step
    int count;
    int maxSteps;          // optional cap ("exceeding session limits")

    // TODO (team): consider a second stack to hold "undone" steps so
    // you can support redo/forward navigation as well as back.

public:
    SessionHistoryStack(int limit = 50);
    ~SessionHistoryStack();

    // Pushes a new step onto the history when the student does something.
    // Pseudocode:
    //   1. If count == maxSteps -> handle "exceeds session limit" case
    //   2. Create new StepNode, link next = topPtr, topPtr = newNode
    //   3. count++
    void recordStep(const SessionStep& step);

    // Pops and returns the most recent step (the "undo"/"back" action).
    // Pseudocode:
    //   1. If isEmpty() -> handle "back on empty history" case
    //   2. Save topPtr->data, advance topPtr = topPtr->next, delete old node
    //   3. count--; return saved step
    SessionStep undoLastStep();

    // Looks at the current (most recent) step without removing it.
    SessionStep currentState() const;

    bool isEmpty() const;
    int size() const;

    // Prints the full history from most recent to oldest.
    void displayHistory() const;
};

#endif // SESSION_HISTORY_STACK_HPP
