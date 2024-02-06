#include"alg/phase-nw.hpp"
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

    xt::xarray<bool> B ={{1, 0, 0, 1, 1, 1, 0},
                        {0, 1, 0, 1, 1, 1, 1},
                        {0, 1, 0, 0, 1, 0, 0},
                        {1, 0, 1, 0, 0, 1, 1},
                        {1, 1, 1, 0, 0, 0, 1},
                        {0, 1, 1, 1, 0, 1, 0}};
    std::vector<bool> flips = {1, 0, 1, 1, 1, 1, 0};
    std::vector<int> cs = {1, 2, 4, 4, 6, 7, 1};
    LabelIndexBiMap biMap(std::set<label>{1, 2, 3, 4, 5, 6});

    ParityTerm pt = ParityTerm(B,flips,cs,biMap);
    auto j = pt.findOptimal({0,1,2,3,4,5,6},{0,1,2,3,4,5}).first;
    std::cout << "j: " << j <<std::endl;
    return 0;
}