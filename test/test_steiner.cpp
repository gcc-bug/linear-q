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
    
    label c = 1;
    std::unordered_set<label> terminals = {6,7,11};
    std::map<label,std::unordered_set<label>> path_set ;
    std::unordered_set<label> determined_path_set ;

    while(!terminals.empty()){
        std::cout << "---------------" <<std::endl;
        std::cout << "c:" << c <<std::endl;
        
         
        auto paths = gridGraph.BFS(c,terminals);
        for(auto path: paths){
            std::unordered_set<int> temp_set;
            std::cout << path.at(0) ;
            for(int i = 1; i < path.size()-1;++i){
                std::cout << "->" << path.at(i);
                temp_set.insert(path.at(i));
            }
            std::cout << "->" << path.at(path.size()-1) << std::endl;
            path_set[path.at(path.size()-1)] = temp_set; 
        }
        
        bool iter = false;
        for(auto path:paths){
            if(determined_path_set.empty()){
                c = path.at(path.size()-1);
                determined_path_set.insert(path_set[c].begin(),path_set[c].end());
                terminals.erase(c);
                iter = true;
                break;
            }
            else if(isSubset(path_set[path.at(path.size()-1)],determined_path_set)){
                c = path.at(path.size()-1);
                terminals.erase(c);
                iter = true;
                break;
            }
        }
        if(!iter){
            c = paths[0].at(paths[0].size()-1);
            determined_path_set.insert(path_set[c].begin(),path_set[c].end());
            terminals.erase(c);
        }
    }
}