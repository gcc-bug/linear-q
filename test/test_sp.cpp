#include "alg/seprate.hpp"
#include <set>
#include <string>
using namespace lq;
int main(){
    Sttree* node1 = new Sttree(1);
    Sttree* node2 = new Sttree(2);
    Sttree* node3 = new Sttree(3);
    Sttree* node4 = new Sttree(4);
    Sttree* node5 = new Sttree(5);
    Sttree* node6 = new Sttree(6);

    insertChild(node1,node2);
    insertChild(node2,node3);
    insertChild(node3,node4);
    insertChild(node4,node5);

    node1->traverse();
    node1->exportTreeToDot("tree");

    std::set<label> terminals = {3,4,5};

    std::cout << "phase:" <<std::endl;
    auto Ts = separate(node1,terminals,AlgSignal::phase);
    for(auto T:Ts){
        std::cout << "root: ";
        std::cout << T.getRoot()->getData() << std::endl;
        std::cout << "leave: ";
        for(auto leaf:T.getLeaves()){
            std::cout << leaf << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "diag:" <<std::endl;
    Ts = separate(node1,terminals,AlgSignal::diag);
    for(auto T:Ts){
        std::cout << "root: ";
        std::cout << T.getRoot()->getData() << std::endl;
        std::cout << "leave: ";
        for(auto leaf:T.getLeaves()){
            std::cout << leaf << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

}