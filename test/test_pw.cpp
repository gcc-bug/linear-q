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

    phase p1 = phase({1,0,0,1,1,0},1,1);
    phase p2 = phase({0,1,1,0,1,1},0,2);
    phase p3 = phase({0,0,0,1,1,1},1,4);
    phase p4 = phase({1,1,0,0,0,1},1,4);
    phase p5 = phase({1,1,1,0,0,0},1,6);
    phase p6 = phase({1,1,0,1,0,1},1,7);
    phase p7 = phase({0,1,0,1,1,0},0,1);

    auto j = choose_j({p1,p2,p3,p4,p5,p6,p7},{0,1,2,3,4,5});
    std::cout << "---------------\n";
    std::cout << "j: " << j <<std::endl;
    return 0;
}