#include"graph/Graph.cpp"
#include <iostream>
#include <unordered_set>
int main() {
    Graph gridGraph(13); // 12 vertices for the 3x4 grid

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
    std::unordered_set<int> nonS;

    std::cout << 1 <<":"<< std::endl;
    auto paths = gridGraph.BFS(1,{6,7,11});
    std::unordered_set<int> nons;
    for(auto path: paths){
        std::cout << path.at(0) ;
        for(int i = 1; i < path.size()-1;++i){
            std::cout << "->" << path.at(i);
            nons.insert(path.at(i));
        }
        std::cout << "->" << path.at(path.size()-1) << std::endl;
    }
    std::cout << "non termianl node: " << nons.size() << std::endl ;

    for(int i = 1; i < paths[0].size()-1;++i){
            nonS.insert(paths[0].at(i));
    }

    
    nons = nonS;
    for (int element : nons) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    std::cout << 7 <<":"<< std::endl;
    paths = gridGraph.BFS(7,{6,11});
    for(auto path: paths){
        std::cout << path.at(0) ;
        for(int i = 1; i < path.size();++i){
            std::cout << "->" << path.at(i);
            nons.insert(path.at(i));
        }
        std::cout<< std::endl;
    }
    std::cout << "non termianl node: " << nons.size();
}