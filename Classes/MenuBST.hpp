#ifndef MENU_BST_HPP
#define MENU_BST_HPP

#include <string>

// MODULE 4: Menu Item Search and Management
// Owner: Gubaeva Liliia
// Data Structure: Binary Search Tree - self-implemented
// Functional requirements:
//   - Store item details: Item ID, name, stall, price
//   - Insert new menu items
//   - Search by Item ID (BST) and by name/category
//   - Update or remove item records
//   - Display items in sorted (in-order) format
//
// Suggested key: itemId (keeps BST ordering simple and unambiguous).


struct MenuItem {
    int itemId;
    std::string name;
    std::string stall;
    double price;

    MenuItem() : itemId(0), name(""), stall(""), price(0.0) {}
    MenuItem(int id, const std::string& n, const std::string& s, double p)
        : itemId(id), name(n), stall(s), price(p) {}
};

struct BSTNode {
    MenuItem data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const MenuItem& item) : data(item), left(nullptr), right(nullptr) {}
};

class MenuBST {
private:
    BSTNode* root;
    int itemCount;

    // --- Recursive helpers (typical BST pattern: public wrapper +
    //     private recursive worker that takes/returns BSTNode*) ---

    // Pseudocode:
    //   if node == nullptr: return new BSTNode(item)
    //   if item.itemId < node->data.itemId: node->left = insertHelper(node->left, item)
    //   else if item.itemId > node->data.itemId: node->right = insertHelper(node->right, item)
    //   else: handle duplicate ID case (update? reject?)
    //   return node
    BSTNode* insertHelper(BSTNode* node, const MenuItem& item);

    // Pseudocode: classic BST search by itemId, recurse left/right,
    // return nullptr if not found.
    BSTNode* searchHelper(BSTNode* node, int itemId) const;

    // Pseudocode: classic BST delete with 3 cases (leaf, one child,
    // two children -> find in-order successor).
    BSTNode* removeHelper(BSTNode* node, int itemId);

    // Pseudocode: in-order traversal (left, visit, right) -> ascending
    // order by itemId; print or collect each node's data.
    void inOrderHelper(BSTNode* node) const;

    // TODO: free all nodes recursively (post-order: left, right, delete)
    void destroyHelper(BSTNode* node);

public:
    MenuBST();
    ~MenuBST();

    void insertItem(const MenuItem& item);
    bool removeItem(int itemId);

    // Returns pointer to found item's data, or nullptr if not present.
    // (Caller should check for nullptr - handles "item not found" case.)
    MenuItem* searchById(int itemId);

    // Prints all items sorted by itemId (in-order traversal).
    void displaySorted() const;

    int size() const;
};

#endif // MENU_BST_HPP
