#include "Classes/Order.hpp"
#include "Classes/OrderQueue.hpp"
#include "Classes/StallCircularQueue.hpp"
#include "Classes/SessionHistoryStack.hpp"
#include "Classes/MenuBST.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>

// Clears the terminal screen so old output doesn't pile up.
// Windows uses "cls", Mac/Linux use "clear" - picked at compile time.
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Wait for the user to press 'F' before returning to the main menu
void waitForMenu() {
    char input = ' ';
    do {
        std::cout << "\nEnter 'F' to go back to main menu: ";
        std::cin >> input;
    } while (input != 'F' && input != 'f');
}

// Flushes the rest of the current input line so a std::getline() call
// right after a std::cin >> (int/word) doesn't just pick up a leftover '\n'.
void flushInputLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Reads a full line of text (names/stalls can contain spaces), reprompting
// on an empty entry.
std::string readNonEmptyLine(const std::string& prompt) {
    std::string value;
    do {
        std::cout << prompt;
        std::getline(std::cin, value);
    } while (value.empty());
    return value;
}

// Reads an int, reprompting on non-numeric input (e.g. letters/symbols)
// instead of leaving std::cin in a broken fail state.
int readValidatedInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cout << "-> Invalid input - please enter a whole number.\n";
        std::cin.clear();
        flushInputLine();
    }
}

// Reads a Menu Item ID, reprompting unless it's a 3-digit whole number
// (100-999), e.g. 101, 250, 999. Rejects letters, negatives, and IDs that
// are too short (< 100) or too long (> 999).
int readValidatedItemId(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 100 && value <= 999) {
            return value;
        }
        if (std::cin.fail()) {
            std::cin.clear();
            flushInputLine();
        }
        std::cout << "-> Invalid input - Item ID must be a 3-digit number (100-999), e.g. 101.\n";
    }
}

// Reads a non-negative price, reprompting on non-numeric input or a
// negative value.
double readValidatedPrice(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0.0) {
            return value;
        }
        if (std::cin.fail()) {
            std::cin.clear();
            flushInputLine();
        }
        std::cout << "-> Invalid input - please enter a price of 0 or higher (e.g. 6.50).\n";
    }
}

// --- Place an Order sub-page ---
// Step IDs for the wizard. Each SessionStep pushed here carries its stepId
// so goBack()/goForward() can tell main() which page to redraw.
const int STEP_MENU = 0;        // shows the menu AND asks for the food choice
const int STEP_STUDENT_ID = 1;  // asks for the TP number, then places the order

void runPlaceOrderFlow(MenuBST& menu, OrderQueue& orderQueue,
                        SessionHistoryStack& sessionHistory, int& orderIdCounter) {
    std::string studentId = "";
    int chosenItemId = -1;
    int currentStep = STEP_MENU;
    bool inFlow = true;
    int ordersPlacedThisSession = 0;

    sessionHistory.recordStep(SessionStep("Order Flow: Viewing menu", STEP_MENU));

    while (inFlow) {
        clearScreen();
        std::cout << "\n========================================\n";
        std::cout << " Place an Order\n";
        std::cout << "========================================\n";
        std::cout << "(Type 'b' to go back, 'f' to go forward)\n\n";

        if (currentStep == STEP_MENU) {
            menu.displaySorted();
            if (ordersPlacedThisSession > 0) {
                std::cout << "\n" << ordersPlacedThisSession << " order(s) placed so far for "
                           << studentId << ".\n";
            }
            std::cout << "\nEnter Menu Item ID to order, or 'b' to finish and return to main menu: ";
            std::string input;
            std::cin >> input;

            if (input == "b" || input == "B") {
                if (ordersPlacedThisSession > 0) {
                    std::cout << "-> Finished placing " << ordersPlacedThisSession
                               << " order(s) for " << studentId << ".\n";
                    waitForMenu();
                }
                inFlow = false; // finish/cancel - return to main menu
            } else if (input == "f" || input == "F") {
                if (sessionHistory.canGoForward()) {
                    SessionStep s = sessionHistory.goForward();
                    currentStep = s.stepId;
                } else {
                    std::cout << "Nothing to go forward to.\n";
                    waitForMenu();
                }
            } else {
                int itemId = std::atoi(input.c_str());
                MenuItem* found = menu.searchById(itemId);
                if (found == nullptr) {
                    std::cout << "-> Item ID " << itemId << " not found. Try again.\n";
                    waitForMenu();
                } else if (!studentId.empty()) {
                    // Student ID already captured this session - place this
                    // additional order immediately, no need to ask again.
                    Order newOrder(orderIdCounter++, studentId, itemId);
                    if (orderQueue.enqueue(newOrder)) {
                        ordersPlacedThisSession++;
                        std::cout << "-> Order placed! Order ID #" << newOrder.orderId
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
            std::cout << "Enter Student ID (e.g., TP012345): ";
            std::string input;
            std::cin >> input;

            if (input == "b" || input == "B") {
                SessionStep s = sessionHistory.goBack();
                currentStep = s.stepId;
            } else if (input == "f" || input == "F") {
                std::cout << "You're already at the last step.\n";
                waitForMenu();
            } else {
                studentId = input;
                MenuItem* found = menu.searchById(chosenItemId);
                if (found != nullptr) {
                    Order newOrder(orderIdCounter++, studentId, chosenItemId);
                    if (orderQueue.enqueue(newOrder)) {
                        ordersPlacedThisSession++;
                        std::cout << "-> Order placed successfully! Order ID #" << newOrder.orderId
                                   << " for " << found->name << "\n";
                        sessionHistory.recordStep(SessionStep("Placed order for " + found->name));
                    }
                } else {
                    std::cout << "-> Something went wrong locating the chosen item. Order cannot be placed.\n";
                }
                waitForMenu();
                currentStep = STEP_MENU; // loop back so more items can be added
            }
        }
    }
}

// --- Order Queue management sub-page ---
// Shows pending & completed orders, then lets the user process the next
// order ('P') or return to the main menu ('F').
void runOrderQueueFlow(OrderQueue& orderQueue, StallCircularQueue& stallQueue) {
    char input = ' ';

    do {
        clearScreen();
        std::cout << "\n========================================\n";
        std::cout << " Order Queue\n";
        std::cout << "========================================\n\n";

        orderQueue.displayPending();
        std::cout << "\n";
        orderQueue.displayCompleted();

        std::cout << "\nEnter 'P' to process the next order, or 'F' to go back to main menu: ";
        std::cin >> input;

        if (input == 'P' || input == 'p') {
            Order nextOrder = orderQueue.dequeue();
            if (nextOrder.orderId != 0) {
                // Assign stall via circular queue
                if (stallQueue.assignNext(nextOrder)) {
                    nextOrder.status = OrderStatus::FULFILLED;
                    orderQueue.markCompleted(nextOrder);
                    std::cout << "-> Order #" << nextOrder.orderId << " has been fulfilled and moved to history.\n";
                }
            } else {
                std::cout << "-> No pending orders to process.\n";
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        } else if (input != 'F' && input != 'f') {
            std::cout << "Invalid choice.\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }

    } while (input != 'F' && input != 'f');
}

int main() {
    // --- Setup Phase ---
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
        std::cout << "\n========================================\n";
        std::cout << " Campus Food Court Self-Order Kiosk System\n";
        std::cout << "========================================\n";
        std::cout << "\n";
        std::cout << "1. Search Menu Item by ID\n";
        std::cout << "2. Add Menu Item\n";
        std::cout << "3. Remove Menu Item\n";
        std::cout << "4. Display Full Menu (sorted by Item ID)\n";
        std::cout << "5. Place an Order (Order Queue)\n";
        std::cout << "6. Order Queue (View & Process Orders)\n";
        std::cout << "7. View Stall Status History\n";
        std::cout << "8. Exit System\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            int id;
            std::cout << "Enter Item ID to search: ";
            std::cin >> id;
            MenuItem* found = menu.searchById(id);
            if (found != nullptr) {
                std::cout << "-> Found: " << found->name << " (" << found->stall << ") - RM " << found->price << "\n";
                sessionHistory.recordStep(SessionStep("Searched/Viewed item: " + found->name));
            } else {
                std::cout << "-> Item ID " << id << " not found.\n";
            }
            waitForMenu();
        }
        else if (choice == 2) {
            // Add Menu Item (BST insert)
            int id = readValidatedItemId("Enter new Item ID (3-digit, 100-999): ");
            flushInputLine(); // drop the leftover '\n' before getline()

            std::string name = readNonEmptyLine("Enter item name: ");
            std::string stall = readNonEmptyLine("Enter stall name: ");

            double price = readValidatedPrice("Enter price (RM): ");

            bool existedBefore = (menu.searchById(id) != nullptr);
            menu.insertItem(MenuItem(id, name, stall, price));

            if (existedBefore) {
                std::cout << "-> Item ID " << id << " already existed - record updated to "
                           << name << " (" << stall << ") - RM " << price << "\n";
            } else {
                std::cout << "-> Added: " << name << " (" << stall << ") - RM " << price
                           << " [ID " << id << "]\n";
            }
            sessionHistory.recordStep(SessionStep("Added/updated menu item: " + name));
            waitForMenu();
        }
        else if (choice == 3) {
            // Remove Menu Item (BST delete) 
            int id;
            std::cout << "Enter Item ID to remove: ";
            std::cin >> id;

            MenuItem* found = menu.searchById(id);
            std::string removedName = (found != nullptr) ? found->name : "";

            bool removed = menu.removeItem(id);
            if (removed) {
                std::cout << "-> Removed item ID " << id
                           << (removedName.empty() ? "" : (" (" + removedName + ")")) << " from the menu.\n";
                sessionHistory.recordStep(SessionStep("Removed menu item ID " + std::to_string(id)));
            } else {
                std::cout << "-> Item ID " << id << " not found - nothing removed.\n";
            }
            waitForMenu();
        }
        else if (choice == 4) {
            // Display Full Menu (BST in-order traversal) 
            menu.displaySorted();
            sessionHistory.recordStep(SessionStep("Viewed full menu (sorted)"));
            waitForMenu();
        }
        else if (choice == 5) {
            runPlaceOrderFlow(menu, orderQueue, sessionHistory, orderIdCounter);
        }
        else if (choice == 6) {
            runOrderQueueFlow(orderQueue, stallQueue);
        }
        else if (choice == 7) {
            stallQueue.displayStallStatus();
            stallQueue.displayAssignmentHistory();
            waitForMenu();
        }
        else if (choice == 8) {
            std::cout << "\nExiting kiosk system. Thank you!\n";
        }
        else {
            std::cout << "Invalid choice. Please enter a number between 1 and 8.\n";
        }

    } while (choice != 8);

    return 0;
}
