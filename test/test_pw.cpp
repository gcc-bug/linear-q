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

    ParityTerm p1 = ParityTerm({1,0,0,1,1,0},1,1);
    ParityTerm p2 = ParityTerm({0,1,1,0,1,1},0,2);
    ParityTerm p3 = ParityTerm({0,0,0,1,1,1},1,4);
    ParityTerm p4 = ParityTerm({1,1,0,0,0,1},1,4);
    ParityTerm p5 = ParityTerm({1,1,1,0,0,0},1,6);
    ParityTerm p6 = ParityTerm({1,1,0,1,0,1},1,7);
    ParityTerm p7 = ParityTerm({0,1,0,1,1,0},0,1);

    auto j = findOptimal({p1,p2,p3,p4,p5,p6,p7},{0,1,2,3,4,5});
    std::cout << "j: " << j.first <<std::endl;
    return 0;
}