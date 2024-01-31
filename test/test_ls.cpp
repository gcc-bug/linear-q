#include "alg/linear-tf.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"

using namespace lq;
int main(){
    Graph g = Graph();
    g.addEdge(1,2);
    g.addEdge(1,6);
    g.addEdge(2,3);
    g.addEdge(2,5);
    g.addEdge(3,4);
    g.addEdge(4,5);
    g.addEdge(5,6);

    g.exportToDot("graph");
    xt::xarray<bool> A = {{1, 1, 0, 1, 1, 0},
                         {0, 0, 1, 1, 0, 1},
                         {1, 0, 1, 0, 1, 0},
                         {1, 1, 0, 1, 0, 0},
                         {1, 1, 1, 1, 0, 0},
                         {0, 1, 0, 1, 0, 1}};
    LabelIndexBiMap biMap(std::set<label>{1, 2, 3, 4, 5, 6});
    biMap.message();
    LFMatrix A_ = LFMatrix(A,biMap);
    linearSynth(A_,&g);
    std::cout << std::endl;
    std::cout << A_.getData() << std::endl;
    std::cout << std::boolalpha << "good ? " << A_.isGood() << std::endl;
    std::cout << std::boolalpha << "finish ? " << A_.isEye() << std::endl;
    return 0;
}