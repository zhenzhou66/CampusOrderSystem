#include "../Classes/SessionHistoryStack.hpp"
#include <iostream>
using namespace std;

int main() {
    SessionHistoryStack sessionHistory(50);

    cout << "=== Session History Module Test ===\n";

    sessionHistory.recordStep(SessionStep("LOGIN: Student TP087450 logged in"));
    sessionHistory.recordStep(SessionStep("MENU_BROWSED: Chinese Stall Menu"));
    sessionHistory.recordStep(SessionStep("ITEM_SELECTED: Char Kway Teow"));
    sessionHistory.recordStep(SessionStep("QUANTITY_UPDATED: Char Kway Teow x2"));

    cout << "\nCurrent state: " << sessionHistory.currentState().description << "\n";

    cout << "\n>> Student presses BACK to fix quantity\n";
    SessionStep undone = sessionHistory.undoLastStep();
    cout << "Undone step was: " << undone.description << "\n";
    cout << "Current state: " << sessionHistory.currentState().description << "\n";

    cout << "\n>> Student re-selects the corrected quantity\n";
    sessionHistory.recordStep(SessionStep("QUANTITY_UPDATED: Char Kway Teow x1"));
    cout << "Current state: " << sessionHistory.currentState().description << "\n";

    cout << "\n>> Student presses BACK repeatedly, including past empty history\n";
    while (!sessionHistory.isEmpty()) {
        SessionStep step = sessionHistory.undoLastStep();
        cout << "Undone: " << step.description << "\n";
    }
    sessionHistory.undoLastStep(); // empty-history edge case

    cout << "\n>> Testing session limit (small cap)\n";
    SessionHistoryStack tinySession(2);
    tinySession.recordStep(SessionStep("LOGIN"));
    tinySession.recordStep(SessionStep("MENU_BROWSED"));
    tinySession.recordStep(SessionStep("ITEM_SELECTED")); // should be rejected — limit is 2

    // Re-populate main session to show displayHistory with content
    sessionHistory.recordStep(SessionStep("LOGIN: Student TP087450 logged in"));
    sessionHistory.recordStep(SessionStep("MENU_BROWSED: Chinese Stall Menu"));
    sessionHistory.displayHistory();

    return 0;
}