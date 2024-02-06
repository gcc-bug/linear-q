#include "alg/row-op.hpp"
#include "alg/seprate.hpp"
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
    insertChild(node2,node5);

    node1->traverse();
    node1->exportTreeToDot("tree");

    std::set<label> terminals = {3,4,5};

    xt::xarray<bool> A = {{1, 1, 0, 1, 1, 0},
                         {0, 0, 1, 1, 0, 1},
                         {1, 0, 1, 0, 1, 0},
                         {1, 1, 0, 1, 0, 0},
                         {1, 1, 1, 1, 0, 0},
                         {0, 1, 0, 1, 0, 1}};
    LabelIndexBiMap biMap(std::set<label>{1, 2, 3, 4, 5, 6});
    biMap.message();
    auto Ts = separate(node1,terminals,AlgSignal::diag);
    LFMatrix A_ = LFMatrix(A,biMap);
    rowOp(A_,Ts,AlgSignal::diag);
    std::cout << A << std::endl;
    return 0;
}