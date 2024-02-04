#include "alg/row-op.hpp"
#include "alg/seprate.hpp"
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