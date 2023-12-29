#include "graph/tree.hpp"
using namespace lq;
int main() {
    Sttree* root = new Sttree(1);

    // Inserting children for root
    insertChild(root, 2);
    insertChild(root, 3);
    insertChild(root, 4);

    // Inserting children for the first child of root
    insertChild(root->children[0], 5);
    insertChild(root->children[0], 6);

    // Inserting a child for the second child of root
    insertChild(root->children[1], 7);

    std::cout << "Tree Traversal:" << std::endl;
    traverse(root);

    // Remember to free memory, implement and call a tree destructor if necessary
    // For simplicity, it's not included here

    return 0;
}
