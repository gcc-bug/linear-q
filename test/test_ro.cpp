#include "graph/tree.hpp"
#include "alg/row-op.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
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

    traverse(node1);
    exportTreeToDot(node1,"tree");

    std::set<label> terminals = {3,4,5};

    xt::xarray<bool> A = {{1, 1, 0, 1, 1, 0},
                         {0, 0, 1, 1, 0, 1},
                         {1, 0, 1, 0, 1, 0},
                         {1, 1, 0, 1, 0, 0},
                         {1, 1, 1, 1, 0, 0},
                         {0, 1, 0, 1, 0, 1}};
    std::map<lq::label, int> label2qubit = {{1,0},{2,1},{3,2},{4,3},{5,4},{6,5}};
    rowOp(A,terminals,node1,1,label2qubit);
    std::cout << A << std::endl;
    return 0;
}