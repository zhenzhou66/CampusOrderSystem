#ifndef SESSION_HISTORY_STACK_HPP
#define SESSION_HISTORY_STACK_HPP

#include <iostream>
#include <string>

struct SessionStep {
    std::string description;   // e.g. "Selected Item #12", "Viewed Menu"

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

public:
    explicit SessionHistoryStack(int limit = 50);
    ~SessionHistoryStack();

    // Prevent shallow copying
    SessionHistoryStack(const SessionHistoryStack&) = delete;
    SessionHistoryStack& operator=(const SessionHistoryStack&) = delete;

    void recordStep(const SessionStep& step);

    SessionStep undoLastStep();
    
    SessionStep currentState() const;
    
    bool isEmpty() const;
    
    int size() const;
    
    void displayHistory() const;
};

#endif // SESSION_HISTORY_STACK_HPP