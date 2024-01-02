#include "graph/tree.hpp"
#include "alg/row-op.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include <set>
#include <string>
using namespace lq;
int main(){
    Sttree* node1 = new Sttree(1);
    Sttree* node4 = new Sttree(4);
    Sttree* node5 = new Sttree(5);
    Sttree* node6 = new Sttree(6);
    Sttree* node7 = new Sttree(7);
    Sttree* node8 = new Sttree(8);
    Sttree* node11 = new Sttree(11);

    insertChild(node1,node4);
    insertChild(node4,node5);
    insertChild(node5,node6);
    insertChild(node4,node7);
    insertChild(node7,node8);
    insertChild(node8,node11);

    traverse(node1);
    exportTreeToDot(node1,"tree");

    std::set<int> terminals = {6,7,11};

    xt::xarray<int> A {1, 2, 3, 4, 5, 6, 7, 8, 9};
    rowOp(A,terminals,node1,1);
    std::cout << A ;
    return 0;
}