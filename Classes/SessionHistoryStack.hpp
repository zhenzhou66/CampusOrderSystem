#ifndef SESSION_HISTORY_STACK_HPP
#define SESSION_HISTORY_STACK_HPP

#include <iostream>
#include <string>

// What kind of menu-editing action a step represents, so it can be
// reversed later. NONE means the step is just a navigation/log entry
// (e.g. a wizard page or a search) with nothing to undo.
enum class UndoAction { NONE, ADDED, REMOVED, UPDATED };

struct SessionStep {
    std::string description;   // e.g. "Selected Item #12", "Viewed Menu"
    int stepId;                 
    UndoAction undoAction;
    int itemId;
    std::string itemName;
    std::string itemStall;
    double itemPrice;

    SessionStep()
        : description(""), stepId(-1), undoAction(UndoAction::NONE),
          itemId(0), itemName(""), itemStall(""), itemPrice(0.0) {}

    SessionStep(const std::string& desc, int id = -1)
        : description(desc), stepId(id), undoAction(UndoAction::NONE),
          itemId(0), itemName(""), itemStall(""), itemPrice(0.0) {}

    // Constructor for undo-capable steps (menu management edits).
    SessionStep(const std::string& desc, UndoAction action, int id,
                const std::string& name, const std::string& stall, double price)
        : description(desc), stepId(-1), undoAction(action),
          itemId(id), itemName(name), itemStall(stall), itemPrice(price) {}
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

public:
    explicit SessionHistoryStack(int limit = 50);
    ~SessionHistoryStack();

    // Prevent shallow copying
    SessionHistoryStack(const SessionHistoryStack&) = delete;
    SessionHistoryStack& operator=(const SessionHistoryStack&) = delete;

    // Pushes a new step onto the stack.
    void recordStep(const SessionStep& step);

    // Pops the current step and returns the step that is now current
    SessionStep goBack();

    SessionStep peekTop() const;

    bool isEmpty() const;
};

#endif // SESSION_HISTORY_STACK_HPP
