#include "Classes/Order.hpp"
#include "Classes/OrderQueue.hpp"
#include "Classes/StallCircularQueue.hpp"
#include "Classes/SessionHistoryStack.hpp"
#include "Classes/MenuBST.hpp"
#include <iostream>
#include <string>

// Wait for the user to press 'F' before returning to the main menu
void waitForMenu() {
    char input = ' ';
    do {
        std::cout << "\nEnter 'F' to go back to main menu: ";
        std::cin >> input;
    } while (input != 'F' && input != 'f');
}

int main() {
    // --- Setup Phase ---
    OrderQueue orderQueue(100);
    SessionHistoryStack sessionHistory(50);
    MenuBST menu;
    StallCircularQueue stallQueue(4, 50); // 4 stalls, max 50 assignment records

    // Register stalls (Stall Circular Queue Module)
    stallQueue.addStall(0, "Malay Stall");
    stallQueue.addStall(1, "Chinese Stall");
    stallQueue.addStall(2, "Western Stall");
    stallQueue.addStall(3, "Beverage Stall");

    // Populate menu items (Menu BST Module)
    menu.insertItem(MenuItem(101, "Nasi Lemak", "Malay Stall", 6.50));
    menu.insertItem(MenuItem(102, "Char Kway Teow", "Chinese Stall", 7.00));
    menu.insertItem(MenuItem(103, "Grilled Chicken Chop", "Western Stall", 9.50));
    menu.insertItem(MenuItem(104, "Iced Lemon Tea", "Beverage Stall", 3.00));

    int choice = 0;
    int orderIdCounter = 1;

    // --- Interactive Kiosk Loop ---
    do {
        std::cout << "\n========================================\n";
        std::cout << " Campus Food Court Self-Order Kiosk System\n";
        std::cout << "========================================\n";
        std::cout << "1. Browse Menu (BST Sorted Display)\n";
        std::cout << "2. Search Menu Item by ID\n";
        std::cout << "3. Place an Order (Order Queue)\n";
        std::cout << "4. Process Next Order & Assign Stall (Circular Queue)\n";
        std::cout << "5. Undo Last Kiosk Step (Session Stack)\n";
        std::cout << "6. View Pending & Completed Orders\n";
        std::cout << "7. View Stall Status History\n";
        std::cout << "8. Exit System\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "\n";
            menu.displaySorted();
            sessionHistory.recordStep(SessionStep("Browsed menu"));
            waitForMenu();
        } 
        else if (choice == 2) {
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
        else if (choice == 3) {
            std::string studentId;
            int itemId;
            std::cout << "Enter Student ID (e.g., TP012345): ";
            std::cin >> studentId;
            std::cout << "Enter Menu Item ID to order: ";
            std::cin >> itemId;

            MenuItem* found = menu.searchById(itemId);
            if (found != nullptr) {
                Order newOrder(orderIdCounter++, studentId, itemId);
                if (orderQueue.enqueue(newOrder)) {
                    std::cout << "-> Order placed successfully! Order ID #" << newOrder.orderId << " for " << found->name << "\n";
                    sessionHistory.recordStep(SessionStep("Placed order for " + found->name));
                }
            } else {
                std::cout << "-> Invalid Item ID. Order cannot be placed.\n";
            }
            waitForMenu();
        } 
        else if (choice == 4) {
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
            waitForMenu();
        } 
        else if (choice == 5) {
            SessionStep undone = sessionHistory.undoLastStep();
            std::cout << "-> Undo action: \"" << undone.description << "\"\n";
            std::cout << "-> Current active state: \"" << sessionHistory.currentState().description << "\"\n";
            waitForMenu();
        } 
        else if (choice == 6) {
            std::cout << "\n";
            orderQueue.displayPending();
            std::cout << "\n";
            orderQueue.displayCompleted();
            waitForMenu();
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