#include"graph/graph.hpp"
using namespace lq;
int main() {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.exportToDot("edge");
    g.BFS(0);
    return 0;
}