#ifndef MENU_BST_HPP
#define MENU_BST_HPP

#include <string>

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

    BSTNode* insertHelper(BSTNode* node, const MenuItem& item);

    BSTNode* searchHelper(BSTNode* node, int itemId) const;

    BSTNode* removeHelper(BSTNode* node, int itemId);

    void inOrderHelper(BSTNode* node) const;

    void destroyHelper(BSTNode* node);

public:
    MenuBST();
    ~MenuBST();

    void insertItem(const MenuItem& item);
    bool removeItem(int itemId);

    MenuItem* searchById(int itemId);

    // Prints all items sorted by itemId (in-order traversal).
    void displaySorted() const;

    int size() const;
};

#endif // MENU_BST_HPP
