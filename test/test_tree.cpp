#include "graph/tree.hpp"
#include <iterator>
using namespace lq;
int main() {
    auto root = std::make_shared<Sttree>(1);

    // Inserting children for root
    insertChild(root, 2);
    auto node3 = std::make_shared<Sttree>(3);
    insertChild(root, node3);
    auto node4 = std::make_shared<Sttree>(4);
    root->insertChild(node4);
    auto it = root->getChildren().begin();

    // Inserting children for the first child of root
    auto first_child = root->getChildren().front();
    first_child->insertChild(5);
    first_child->insertChild(6);

    auto second_child = root->getChildren()[1];
    second_child->insertChild(7);


    std::cout << "Tree Traversal:" << std::endl;
    root->traverse();
    root->exportTreeToDot("tree");

    // Remember to free memory, implement and call a tree destructor if necessary
    // For simplicity, it's not included here

    return 0;
}
