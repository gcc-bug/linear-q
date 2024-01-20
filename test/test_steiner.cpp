#include"graph/graph.hpp"
#include <iostream>
#include <map>
#include <unordered_set>

template<typename T>
bool isSubset(const std::unordered_set<T>& setA, const std::unordered_set<T>& SetB) {
    for (const T& element : setA) {
        if (SetB.find(element) == SetB.end()) {
            return false; // Element not found in orderedSet
        }
    }
    return true;
}
using namespace lq;
int main() {
    Graph gridGraph = Graph(); // 12 vertices for the 3x4 grid

    gridGraph.addEdge(1, 2);
    gridGraph.addEdge(2, 3);
    gridGraph.addEdge(4, 5);
    gridGraph.addEdge(5, 6);
    gridGraph.addEdge(7, 8);
    gridGraph.addEdge(8, 9);
    gridGraph.addEdge(10, 11);
    gridGraph.addEdge(11, 12);

    gridGraph.addEdge(1, 4);
    gridGraph.addEdge(2, 5);
    gridGraph.addEdge(3, 6);
    gridGraph.addEdge(4, 7);
    gridGraph.addEdge(5, 8);
    gridGraph.addEdge(6, 9);
    gridGraph.addEdge(7, 10);
    gridGraph.addEdge(8, 11);
    gridGraph.addEdge(9, 12);

    gridGraph.exportToDot("graph");
    
    std::set<label> terminals ={6,9,12};
    // auto tree = gridGraph.reduceMSTToSteinerTree(terminals);
    // for(auto edge:tree){
    //     std::cout << edge.first << " "<< edge.second << std::endl;
    // }
    auto paths = gridGraph.findPaths(1,terminals);
    for(auto path:paths){
        std::cout << "path to "<< path.back() <<std::endl;
        for(auto v:path){
            std::cout << v << "->";
        }
        std::cout << std::endl;
    }
}