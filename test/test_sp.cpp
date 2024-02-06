#include "alg/seprate.hpp"
#include <set>
#include <string>
using namespace lq;
int main(){
    std::shared_ptr<Sttree> node1 = std::make_shared<Sttree>(1);
    std::shared_ptr<Sttree> node2 = std::make_shared<Sttree>(2);
    std::shared_ptr<Sttree> node3 = std::make_shared<Sttree>(3);
    std::shared_ptr<Sttree> node4 = std::make_shared<Sttree>(4);
    std::shared_ptr<Sttree> node5 = std::make_shared<Sttree>(5);
    std::shared_ptr<Sttree> node6 = std::make_shared<Sttree>(6);

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