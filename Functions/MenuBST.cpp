#include "../Classes/MenuBST.hpp"
#include <iostream>
#include <iomanip>

// Menu Item Search and Management - Binary Search Tree

MenuBST::MenuBST() {
    root = nullptr;
    itemCount = 0;
}

MenuBST::~MenuBST() {
    destroyHelper(root);
    root = nullptr;
}

BSTNode* MenuBST::insertHelper(BSTNode* node, const MenuItem& item) {
    if (node == nullptr) {
        return new BSTNode(item);
    }

    if (item.itemId < node->data.itemId) {
        node->left = insertHelper(node->left, item);
    } else if (item.itemId > node->data.itemId) {
        node->right = insertHelper(node->right, item);
    } else {
        // Duplicate ID: reject the insert by overwriting the same node's
        // data with the new details (treat it as an update-in-place).
        std::cout << "[MenuBST] Item ID " << item.itemId
                   << " already exists - updating existing record.\n";
        node->data = item;
    }

    return node;
}

BSTNode* MenuBST::searchHelper(BSTNode* node, int itemId) const {
    if (node == nullptr || node->data.itemId == itemId) {
        return node;
    }

    if (itemId < node->data.itemId) {
        return searchHelper(node->left, itemId);
    }
    return searchHelper(node->right, itemId);
}

BSTNode* MenuBST::removeHelper(BSTNode* node, int itemId) {
    if (node == nullptr) {
        return nullptr; // item not found along this path
    }

    if (itemId < node->data.itemId) {
        node->left = removeHelper(node->left, itemId);
    } else if (itemId > node->data.itemId) {
        node->right = removeHelper(node->right, itemId);
    } else {
        // Found the node to remove.

        // Case 1: leaf node:
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Case 2: one child - bypass this node:
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        }
        if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: two children - replace with in-order successor
        // (smallest value in the right subtree), then remove that
        // successor from the right subtree.
        BSTNode* successor = node->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }
        node->data = successor->data;
        node->right = removeHelper(node->right, successor->data.itemId);
    }

    return node;
}

void MenuBST::inOrderHelper(BSTNode* node) const {
    if (node == nullptr) {
        return;
    }

    inOrderHelper(node->left);
    std::cout << "  ID " << node->data.itemId
               << " | " << std::left << std::setw(20) << node->data.name
               << " | Stall: " << std::setw(15) << node->data.stall
               << " | RM " << std::fixed << std::setprecision(2) << node->data.price
               << "\n";
    inOrderHelper(node->right);
}

void MenuBST::destroyHelper(BSTNode* node) {
    if (node == nullptr) {
        return;
    }
    destroyHelper(node->left);
    destroyHelper(node->right);
    delete node;
}

void MenuBST::insertItem(const MenuItem& item) {
    bool existedBefore = (searchHelper(root, item.itemId) != nullptr);
    root = insertHelper(root, item);
    if (!existedBefore) {
        itemCount++;
    }
}

bool MenuBST::removeItem(int itemId) {
    if (searchHelper(root, itemId) == nullptr) {
        std::cout << "[MenuBST] Item ID " << itemId << " not found - nothing to remove.\n";
        return false;
    }
    root = removeHelper(root, itemId);
    itemCount--;
    return true;
}

MenuItem* MenuBST::searchById(int itemId) {
    BSTNode* found = searchHelper(root, itemId);
    return found ? &(found->data) : nullptr;
}

void MenuBST::displaySorted() const {
    std::cout << "--- Menu Items (" << itemCount << ") sorted by Item ID ---\n";
    if (root == nullptr) {
        std::cout << "  (no menu items yet)\n";
        return;
    }
    inOrderHelper(root);
}

int MenuBST::size() const {
    return itemCount;
}
