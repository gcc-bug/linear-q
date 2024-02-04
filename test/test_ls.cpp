#include "alg/linear-tf.hpp"
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
    LabelIndexBiMap biMap(std::set<label>{1, 2, 3, 4, 5, 6});
    biMap.message();
    LFMatrix A_ = LFMatrix(A,biMap);
    auto Cs = linearSynth(A_,&g);
    std::cout << "use "<<Cs.size() <<" gates" << std::endl;
    std::cout << std::boolalpha << "good ? " << A_.isGood() << std::endl;
    std::cout << std::boolalpha << "finish ? " << A_.isEye() << std::endl;
    LFMatrix CheckA = LFMatrix(xt::eye(biMap.getSize()),biMap);
    for(auto c: Cs){
        auto [ctrl,targ] = c.getLabel();
        CheckA.CNOT_(ctrl,targ);
    }
    bool res = (CheckA.getData() == A);
    std::cout << std::boolalpha << res << std::endl;
    return 0;
}