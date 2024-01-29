#include "graph/tree.hpp"
#include <iterator>
using namespace lq;
int main() {
    Sttree* root = new Sttree(1);

    // Inserting children for root
    insertChild(root, 2);
    Sttree* node3 = new Sttree(3);
    insertChild(root, node3);
    Sttree* node4 = new Sttree(4);
    root->insertChild(node4);
    auto it = root->get_children().begin();

    // Inserting children for the first child of root
    Sttree* first_child = *it;
    first_child->insertChild(5);
    first_child->insertChild(6);

    // Inserting a child for the second child of root
    std::advance(it,1);
    Sttree* second_child = *it;
    second_child->insertChild(7);


    std::cout << "Tree Traversal:" << std::endl;
    root->traverse();
    root->exportTreeToDot("tree");

    // Remember to free memory, implement and call a tree destructor if necessary
    // For simplicity, it's not included here

    return 0;
}
