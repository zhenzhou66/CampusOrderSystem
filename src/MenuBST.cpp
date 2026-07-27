#include "../include/MenuBST.hpp"
#include <iostream>

// ============================================================
// Implementation skeleton only - NO working logic provided.
// Follow the pseudocode comments in MenuBST.hpp for each function.
// ============================================================

MenuBST::MenuBST() {
    // TODO: root = nullptr; itemCount = 0;
    root = nullptr;
    itemCount = 0;
}

MenuBST::~MenuBST() {
    // TODO: destroyHelper(root);
}

BSTNode* MenuBST::insertHelper(BSTNode* node, const MenuItem& item) {
    // TODO: implement using the pseudocode in the header.
    return node;
}

BSTNode* MenuBST::searchHelper(BSTNode* node, int itemId) const {
    // TODO: implement using the pseudocode in the header.
    return nullptr;
}

BSTNode* MenuBST::removeHelper(BSTNode* node, int itemId) {
    // TODO: implement the 3-case BST delete described in the header.
    return node;
}

void MenuBST::inOrderHelper(BSTNode* node) const {
    // TODO: if node == nullptr return;
    //       inOrderHelper(node->left);
    //       print node->data;
    //       inOrderHelper(node->right);
}

void MenuBST::destroyHelper(BSTNode* node) {
    // TODO: if node == nullptr return;
    //       destroyHelper(node->left);
    //       destroyHelper(node->right);
    //       delete node;
}

void MenuBST::insertItem(const MenuItem& item) {
    // TODO: root = insertHelper(root, item); itemCount++;
}

bool MenuBST::removeItem(int itemId) {
    // TODO: check existence via searchHelper first, then
    // root = removeHelper(root, itemId); itemCount--; return true/false
    return false;
}

MenuItem* MenuBST::searchById(int itemId) {
    // TODO: BSTNode* found = searchHelper(root, itemId);
    //       return found ? &(found->data) : nullptr;
    return nullptr;
}

void MenuBST::displaySorted() const {
    // TODO: inOrderHelper(root);
}

int MenuBST::size() const {
    // TODO: return itemCount;
    return 0;
}
