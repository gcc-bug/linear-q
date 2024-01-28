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


    xt::xarray<bool> A = {{1, 1, 0, 1, 1, 0},
                         {0, 0, 1, 1, 0, 1},
                         {1, 0, 1, 0, 1, 0},
                         {1, 1, 0, 1, 0, 0},
                         {1, 1, 1, 1, 0, 0},
                         {0, 1, 0, 1, 0, 1}};
    label2qubit label2qubit(std::set<label>{1, 2, 3, 4, 5, 6});
    label2qubit.message();
    linearSynth(A,g,label2qubit);
    std::cout << std::endl;
    std::cout << A << std::endl;
    return 0;
}