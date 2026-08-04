#include "Classes/Order.hpp"
#include "Classes/OrderQueue.hpp"
#include "Classes/StallCircularQueue.hpp"
#include "Classes/SessionHistoryStack.hpp"
#include "Classes/MenuBST.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <cctype>

using namespace std;

// Clears the terminal screen so old output doesn't pile up.
// Windows uses "cls", Mac/Linux use "clear" - picked at compile time.
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Wait for the user to press 'F' before returning to the main menu.
// Self-healing: if cin is already in a fail state (e.g. an earlier
// unvalidated numeric read choked on non-numeric input), a plain
// cin >> here would fail instantly forever, printing this prompt in
// an infinite loop without ever waiting for real input. Clearing the
// stream and dropping any leftover garbage before each read prevents that.
void waitForMenu() {
    char input = ' ';
    do {
        if (cin.fail()) {
            // Recovering from a broken stream: clear the error flag and
            // discard the garbage token that caused it, otherwise every
            // read below would keep failing instantly with no way out.
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "\nEnter 'F' to go back to main menu: ";
        cin >> input;
    } while (input != 'F' && input != 'f');
}

// Flushes the rest of the current input line so a getline() call
// right after a cin >> (int/word) doesn't just pick up a leftover '\n'.
void flushInputLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Reads a full line of text (names/stalls can contain spaces), reprompting
// on an empty entry.
string readNonEmptyLine(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);
    } while (value.empty());
    return value;
}

// Reads an int, reprompting on non-numeric input (e.g. letters/symbols)
// instead of leaving cin in a broken fail state.
int readValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "-> Invalid input - please enter a whole number.\n";
        cin.clear();
        flushInputLine();
    }
}

// Reads a Menu Item ID, reprompting unless it's a 3-digit whole number
// (100-999), e.g. 101, 250, 999. Rejects letters, negatives, and IDs that
// are too short (< 100) or too long (> 999).
int readValidatedItemId(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 100 && value <= 999) {
            return value;
        }
        if (cin.fail()) {
            cin.clear();
            flushInputLine();
        }
        cout << "-> Invalid input - Item ID must be a 3-digit number (100-999), e.g. 101.\n";
    }
}

// Validates a pre-read token as a 3-digit Item ID (100-999). Used inside
// flows that need to check for a "b" (back) keypress before committing to
// a numeric read, so it can't just use cin >> int directly.
bool parseItemId(const string& token, int& outId) {
    if (token.empty()) {
        return false;
    }
    for (char c : token) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    long value = atol(token.c_str());
    if (value < 100 || value > 999) {
        return false;
    }
    outId = static_cast<int>(value);
    return true;
}

// Reads a non-negative price, reprompting on non-numeric input or a
// negative value.
double readValidatedPrice(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0.0) {
            return value;
        }
        if (cin.fail()) {
            cin.clear();
            flushInputLine();
        }
        cout << "-> Invalid input - please enter a price of 0 or higher (e.g. 6.50).\n";
    }
}

// Place an Order sub-page
// Step IDs for the wizard. Each SessionStep pushed here carries its stepId
// so goBack() can tell main() which page to redraw.
const int STEP_MENU = 0;        // shows the menu AND asks for the food choice
const int STEP_STUDENT_ID = 1;  // asks for the TP number, then places the order

//  Menu Management sub-page step IDs (separate range so they never
// collide with the Place Order flow's step IDs above - both flows push
// onto the same shared SessionHistoryStack). ---
const int STEP_MGMT_HOME = 10;    // the Add/Remove/Update/Display submenu
const int STEP_MGMT_ADD = 11;
const int STEP_MGMT_REMOVE = 12;
const int STEP_MGMT_UPDATE = 13;

void runPlaceOrderFlow(MenuBST& menu, OrderQueue& orderQueue,
                        SessionHistoryStack& sessionHistory, int& orderIdCounter) {
    string studentId = "";
    int chosenItemId = -1;
    int currentStep = STEP_MENU;
    bool inFlow = true;
    int ordersPlacedThisSession = 0;

    sessionHistory.recordStep(SessionStep("Order Flow: Viewing menu", STEP_MENU));

    while (inFlow) {
        clearScreen();
        cout << "\n========================================\n";
        cout << " Place an Order\n";
        cout << "========================================\n";
        cout << "(Type 'b' to go back)\n\n";

        if (currentStep == STEP_MENU) {
            menu.displaySorted();
            if (ordersPlacedThisSession > 0) {
                cout << "\n" << ordersPlacedThisSession << " order(s) placed so far for "
                     << studentId << ".\n";
            }
            cout << "\nEnter Menu Item ID to order, or 'b' to finish and return to main menu: ";
            string input;
            cin >> input;

            if (input == "b" || input == "B") {
                if (ordersPlacedThisSession > 0) {
                    cout << "-> Finished placing " << ordersPlacedThisSession
                         << " order(s) for " << studentId << ".\n";
                    waitForMenu();
                }
                inFlow = false; // finish/cancel - return to main menu
            } else {
                int itemId = atoi(input.c_str());
                MenuItem* found = menu.searchById(itemId);
                if (found == nullptr) {
                    cout << "-> Item ID " << itemId << " not found. Try again.\n";
                    waitForMenu();
                } else if (!studentId.empty()) {
                    // Already have studentID can place multiple orders in one session
                    Order newOrder(orderIdCounter++, studentId, itemId);
                    if (orderQueue.enqueue(newOrder)) {
                        ordersPlacedThisSession++;
                        cout << "-> Order placed! Order ID #" << newOrder.orderId
                             << " for " << found->name << "\n";
                        sessionHistory.recordStep(SessionStep("Placed order for " + found->name));
                    }
                    waitForMenu();
                } else {
                    // First order this session - need the student ID first.
                    chosenItemId = itemId;
                    currentStep = STEP_STUDENT_ID;
                    sessionHistory.recordStep(
                        SessionStep("Order Flow: Chose item " + found->name, STEP_STUDENT_ID));
                }
            }
        }
        else if (currentStep == STEP_STUDENT_ID) {
            cout << "Enter Student ID (e.g., TP012345): ";
            string input;
            cin >> input;

            if (input == "b" || input == "B") {
                SessionStep s = sessionHistory.goBack();
                currentStep = s.stepId;
            } else {
                studentId = input;
                MenuItem* found = menu.searchById(chosenItemId);
                if (found != nullptr) {
                    Order newOrder(orderIdCounter++, studentId, chosenItemId);
                    if (orderQueue.enqueue(newOrder)) {
                        ordersPlacedThisSession++;
                        cout << "-> Order placed successfully! Order ID #" << newOrder.orderId
                             << " for " << found->name << "\n";
                        sessionHistory.recordStep(SessionStep("Placed order for " + found->name));
                    }
                } else {
                    cout << "-> Something went wrong locating the chosen item. Order cannot be placed.\n";
                }
                waitForMenu();
                currentStep = STEP_MENU; // loop back so more items can be added
            }
        }
    }
}

// Order Queue management sub-page
// Shows pending & completed orders, then lets the user process the next
// order ('P') or return to the main menu ('F').
void runOrderQueueFlow(OrderQueue& orderQueue, StallCircularQueue& stallQueue) {
    char input = ' ';

    do {
        clearScreen();
        cout << "\n========================================\n";
        cout << " Order Queue\n";
        cout << "========================================\n\n";

        orderQueue.displayPending();
        cout << "\n";
        orderQueue.displayCompleted();

        cout << "\nEnter 'P' to process the next order, or 'F' to go back to main menu: ";
        cin >> input;

        if (input == 'P' || input == 'p') {
            Order nextOrder = orderQueue.dequeue();
            if (nextOrder.orderId != 0) {
                // Assign stall via circular queue
                if (stallQueue.assignNext(nextOrder)) {
                    nextOrder.status = OrderStatus::FULFILLED;
                    orderQueue.markCompleted(nextOrder);
                    cout << "-> Order #" << nextOrder.orderId << " has been fulfilled and moved to history.\n";
                }
            } else {
                cout << "-> No pending orders to process.\n";
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        } else if (input != 'F' && input != 'f') {
            cout << "Invalid choice.\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while (input != 'F' && input != 'f');
}

// Menu Management sub-page (Menu BST + Session History Stack)
void runMenuManagementFlow(MenuBST& menu, SessionHistoryStack& sessionHistory) {
    int currentStep = STEP_MGMT_HOME;
    bool inFlow = true;

    sessionHistory.recordStep(SessionStep("Menu Management: Home", STEP_MGMT_HOME));

    while (inFlow) {
        clearScreen();
        cout << "\n========================================\n";
        cout << " Menu Management\n";
        cout << "========================================\n";

        if (currentStep == STEP_MGMT_HOME) {
            cout << "(Type 'b' to return to main menu, 'u' to undo last operation)\n\n";
            cout << "1. Add Menu Item\n";
            cout << "2. Remove Menu Item\n";
            cout << "3. Update Menu Item\n";
            cout << "4. Display Full Menu (sorted by Item ID)\n";
            cout << "Enter your choice: ";
            string input;
            cin >> input;

            if (input == "b" || input == "B") {
                inFlow = false; // leave the flow, back to main menu
            } else if (input == "u" || input == "U") {
                if (sessionHistory.isEmpty()) {
                    cout << "-> No operation to undo.\n";
                } else {
                    SessionStep last = sessionHistory.peekTop();
                    if (last.undoAction == UndoAction::ADDED) {
                        menu.removeItem(last.itemId);
                        cout << "-> Undo: removed item ID " << last.itemId
                             << " (" << last.itemName << ").\n";
                        sessionHistory.goBack(); // pop the undone step
                    } else if (last.undoAction == UndoAction::REMOVED) {
                        menu.insertItem(MenuItem(last.itemId, last.itemName, last.itemStall, last.itemPrice));
                        cout << "-> Undo: restored item ID " << last.itemId
                             << " (" << last.itemName << ").\n";
                        sessionHistory.goBack();
                    } else if (last.undoAction == UndoAction::UPDATED) {
                        menu.insertItem(MenuItem(last.itemId, last.itemName, last.itemStall, last.itemPrice));
                        cout << "-> Undo: reverted item ID " << last.itemId << " back to "
                             << last.itemName << " (" << last.itemStall << ") - RM " << last.itemPrice << "\n";
                        sessionHistory.goBack();
                    } else {
                        cout << "-> Nothing to undo (last recorded step wasn't a menu edit).\n";
                    }
                }
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                // stays at STEP_MGMT_HOME
            } else if (input == "1") {
                currentStep = STEP_MGMT_ADD;
                sessionHistory.recordStep(SessionStep("Menu Management: Add Item", STEP_MGMT_ADD));
            } else if (input == "2") {
                currentStep = STEP_MGMT_REMOVE;
                sessionHistory.recordStep(SessionStep("Menu Management: Remove Item", STEP_MGMT_REMOVE));
            } else if (input == "3") {
                currentStep = STEP_MGMT_UPDATE;
                sessionHistory.recordStep(SessionStep("Menu Management: Update Item", STEP_MGMT_UPDATE));
            } else if (input == "4") {
                menu.displaySorted();
                sessionHistory.recordStep(SessionStep("Menu Management: Viewed full menu (sorted)"));
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                // stays at STEP_MGMT_HOME
            } else {
                cout << "Invalid choice.\n";
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
        else if (currentStep == STEP_MGMT_ADD) {
            cout << "(Type 'b' to cancel and go back)\n\n";
            string idToken;
            cout << "Enter new Item ID (100-999), or 'b' to cancel: ";
            cin >> idToken;

            if (idToken == "b" || idToken == "B") {
                SessionStep s = sessionHistory.goBack();
                currentStep = s.stepId;
            } else {
                int id;
                if (!parseItemId(idToken, id)) {
                    cout << "-> Invalid Item ID - must be a 3-digit number (100-999), e.g. 101.\n";
                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                    // stay at STEP_MGMT_ADD to retry
                } else if (menu.searchById(id) != nullptr) {
                    cout << "-> Item ID " << id << " already exists - use 'Update Menu Item' instead.\n";
                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                    currentStep = STEP_MGMT_HOME;
                } else {
                    flushInputLine();
                    string name = readNonEmptyLine("Enter item name: ");
                    string stall = readNonEmptyLine("Enter stall name: ");
                    double price = readValidatedPrice("Enter price (RM): ");

                    menu.insertItem(MenuItem(id, name, stall, price));
                    cout << "-> Added: " << name << " (" << stall << ") - RM " << price
                         << " [ID " << id << "]\n";
                    // Undo of an ADD just removes the item that was inserted.
                    sessionHistory.recordStep(SessionStep("Added menu item: " + name,
                        UndoAction::ADDED, id, name, stall, price));

                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                    currentStep = STEP_MGMT_HOME;
                }
            }
        }
        else if (currentStep == STEP_MGMT_REMOVE) {
            cout << "(Type 'b' to cancel and go back)\n\n";
            string idToken;
            cout << "Enter Item ID to remove (100-999), or 'b' to cancel: ";
            cin >> idToken;

            if (idToken == "b" || idToken == "B") {
                SessionStep s = sessionHistory.goBack();
                currentStep = s.stepId;
            } else {
                int id;
                if (!parseItemId(idToken, id)) {
                    cout << "-> Invalid Item ID - must be a 3-digit number (100-999), e.g. 101.\n";
                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                    // stay at STEP_MGMT_REMOVE to retry
                } else {
                    MenuItem* found = menu.searchById(id);
                    // Snapshot the full item BEFORE removal, so undo can
                    // re-insert it exactly as it was.
                    MenuItem removedSnapshot = (found != nullptr) ? *found : MenuItem();
                    string removedName = (found != nullptr) ? found->name : "";
                    bool removed = menu.removeItem(id);

                    if (removed) {
                        cout << "-> Removed item ID " << id
                             << (removedName.empty() ? "" : (" (" + removedName + ")")) << " from the menu.\n";
                        // Undo of a REMOVE re-inserts the snapshot taken above.
                        sessionHistory.recordStep(SessionStep("Removed menu item: " + removedName,
                            UndoAction::REMOVED, removedSnapshot.itemId, removedSnapshot.name,
                            removedSnapshot.stall, removedSnapshot.price));
                    } else {
                        cout << "-> Item ID " << id << " not found - nothing removed.\n";
                    }

                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                    currentStep = STEP_MGMT_HOME;
                }
            }
        }
        else if (currentStep == STEP_MGMT_UPDATE) {
            cout << "(Type 'b' to cancel and go back)\n\n";
            string idToken;
            cout << "Enter Item ID to update (100-999), or 'b' to cancel: ";
            cin >> idToken;

            if (idToken == "b" || idToken == "B") {
                SessionStep s = sessionHistory.goBack();
                currentStep = s.stepId;
            } else {
                int id;
                if (!parseItemId(idToken, id)) {
                    cout << "-> Invalid Item ID - must be a 3-digit number (100-999), e.g. 101.\n";
                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                    // stay at STEP_MGMT_UPDATE to retry
                } else {
                    MenuItem* existing = menu.searchById(id);
                    if (existing == nullptr) {
                        cout << "-> Item ID " << id << " not found - nothing to update. Use 'Add Menu Item' instead.\n";
                        cout << "\nPress Enter to continue...";
                        cin.ignore();
                        cin.get();
                        currentStep = STEP_MGMT_HOME;
                    } else {
                        cout << "-> Currently: " << existing->name << " (" << existing->stall
                             << ") - RM " << existing->price << "\n";
                        // Snapshot the OLD values BEFORE overwriting, so undo
                        // can restore exactly what was there before.
                        MenuItem oldSnapshot = *existing;
                        flushInputLine();
                        string name = readNonEmptyLine("Enter new item name: ");
                        string stall = readNonEmptyLine("Enter new stall name: ");
                        double price = readValidatedPrice("Enter new price (RM): ");

                        menu.insertItem(MenuItem(id, name, stall, price)); // overwrites in place
                        cout << "-> Updated ID " << id << " to " << name << " (" << stall
                             << ") - RM " << price << "\n";
                        // Undo of an UPDATE restores the pre-edit snapshot.
                        sessionHistory.recordStep(SessionStep("Updated menu item: " + name,
                            UndoAction::UPDATED, oldSnapshot.itemId, oldSnapshot.name,
                            oldSnapshot.stall, oldSnapshot.price));

                        cout << "\nPress Enter to continue...";
                        cin.ignore();
                        cin.get();
                        currentStep = STEP_MGMT_HOME;
                    }
                }
            }
        }
    }
}

int main() {
    // Setup Phase
    OrderQueue orderQueue(100);
    SessionHistoryStack sessionHistory(50);
    MenuBST menu;
    StallCircularQueue stallQueue(4, 50); // 4 stalls, max 50 assignment records

    // Register stalls (Stall Circular Queue Module)
    stallQueue.addStall(0, "Stall A");
    stallQueue.addStall(1, "Stall B");
    stallQueue.addStall(2, "Stall C");
    stallQueue.addStall(3, "Stall D");

    // Populate menu items (Menu BST Module)
    menu.insertItem(MenuItem(101, "Nasi Lemak", "Stall A", 6.50));
    menu.insertItem(MenuItem(102, "Char Kway Teow", "Stall B", 7.00));
    menu.insertItem(MenuItem(103, "Grilled Chicken Chop", "Stall C", 9.50));
    menu.insertItem(MenuItem(104, "Iced Lemon Tea", "Stall D", 3.00));

    int choice = 0;
    int orderIdCounter = 1;

    // Interactive Kiosk Loop
    do {
        clearScreen();
        cout << "\n========================================\n";
        cout << " Campus Food Court Self-Order Kiosk System\n";
        cout << "========================================\n";
        cout << "\n";
        cout << "1. Search Menu Item by ID\n";
        cout << "2. Menu Management (Add / Remove / Update / Display)\n";
        cout << "3. Place an Order (Order Queue)\n";
        cout << "4. Order Queue (View & Process Orders)\n";
        cout << "5. View Stall Status History\n";
        cout << "6. Exit System\n";
        choice = readValidatedInt("Enter your choice: ");

        if (choice == 1) {
            int id = readValidatedInt("Enter Item ID to search: ");
            MenuItem* found = menu.searchById(id);
            if (found != nullptr) {
                cout << "-> Found: " << found->name << " (" << found->stall << ") - RM " << found->price << "\n";
                sessionHistory.recordStep(SessionStep("Searched/Viewed item: " + found->name));
            } else {
                cout << "-> Item ID " << id << " not found.\n";
            }
            waitForMenu();
        }
        else if (choice == 2) {
            runMenuManagementFlow(menu, sessionHistory);
        }
        else if (choice == 3) {
            runPlaceOrderFlow(menu, orderQueue, sessionHistory, orderIdCounter);
        }
        else if (choice == 4) {
            runOrderQueueFlow(orderQueue, stallQueue);
        }
        else if (choice == 5) {
            stallQueue.displayStallStatus();
            stallQueue.displayAssignmentHistory();
            waitForMenu();
        }
        else if (choice == 6) {
            cout << "\nExiting kiosk system. Thank you!\n";
        }
        else {
            cout << "Invalid choice. Please enter a number between 1 and 6.\n";
        }

    } while (choice != 6);

    return 0;
}
