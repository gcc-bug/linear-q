#include <vector>
#include <iostream>

#ifndef ST_TREE_CLASS
#define ST_TREE_CLASS

namespace lq
{
    struct Sttree {
        int data;
        std::vector<Sttree*> children;

        Sttree(int value) : data(value) {}
    };

    void insertChild(Sttree* parent, int value) {
        Sttree* newNode = new Sttree(value);
        parent->children.push_back(newNode);
    }
    void insertChild(Sttree* parent, Sttree* child) {
        parent->children.push_back(child);
    }
    void traverse(Sttree* root, int depth = 0) {
        if (root == nullptr) return;
        std::cout << std::string(depth, ' ') << root->data << std::endl;
        for (auto child : root->children) {
            traverse(child, depth + 1);
        }
}

} // namespace lq

#endif