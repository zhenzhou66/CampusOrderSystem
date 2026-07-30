#ifndef SESSION_HISTORY_STACK_HPP
#define SESSION_HISTORY_STACK_HPP

#include <iostream>
#include <string>

struct SessionStep {
    std::string description;   // e.g. "Selected Item #12", "Viewed Menu"
    int stepId;                 // which wizard "page" this step represents
                                 // (-1 = not tied to a specific page, just a log entry)

    SessionStep() : description(""), stepId(-1) {}
    SessionStep(const std::string& desc, int id = -1) : description(desc), stepId(id) {}
};

struct StepNode {
    SessionStep data;
    StepNode* next;   // points to the PREVIOUS step (toward the bottom of stack)

    StepNode(const SessionStep& step) : data(step), next(nullptr) {}
};

class SessionHistoryStack {
private:
    StepNode* topPtr;      // most recent step (the "back" stack)
    int count;
    int maxSteps;          // optional cap ("exceeding session limits")

    // Second stack holding steps that were undone via goBack(), so goForward()
    // can redo them - same idea as a browser's forward button.
    StepNode* forwardTopPtr;
    int forwardCount;

public:
    explicit SessionHistoryStack(int limit = 50);
    ~SessionHistoryStack();

    // Prevent shallow copying
    SessionHistoryStack(const SessionHistoryStack&) = delete;
    SessionHistoryStack& operator=(const SessionHistoryStack&) = delete;

    // Pushes a new step. Also clears the forward stack, since taking a new
    // action invalidates whatever "forward" branch existed before (same as
    // a browser: visiting a new page clears the forward history).
    void recordStep(const SessionStep& step);

    // Moves one step back: pops the current step onto the forward stack and
    // returns the step that is now current (the new top of the back stack).
    SessionStep goBack();

    // Moves one step forward again: pops from the forward stack back onto
    // the main stack and returns it as the new current step.
    SessionStep goForward();

    bool canGoForward() const;

    bool isEmpty() const;
};

#endif // SESSION_HISTORY_STACK_HPP
