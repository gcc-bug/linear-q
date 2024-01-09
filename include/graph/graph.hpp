#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <optional>
#include "../Config.hpp"

#ifndef GRAPH_CLASS
#define GRAPH_CLASS
namespace lq{
class Graph {
    int numVertices;
    std::map<label,std::list<label>> adjLists;

public:
    // Function to find the minimum spanning tree using Prim's algorithm
    
    Graph(int numvertices) : numVertices(numvertices) {}

    void addEdge(label src, label dest) {
        if(this->adjLists.size() <= this->numVertices){
            this->adjLists[src].push_back(dest);
            // For undirected graph, add edge from dest to src as well
            this->adjLists[dest].push_back(src);
        }
        else{
            throw std::invalid_argument("Erreur de paramètre");
        }
        
    }

    void BFS(const label startVertex) {
        if(this->adjLists.find(startVertex)== this->adjLists.end()){
            throw std::invalid_argument("Erruer");
        }

        std::set<label> visited;
        std::queue<label> queue;

        visited.insert(startVertex);
        queue.push(startVertex);

        while (!queue.empty()) {
            label currentVertex = queue.front();
            queue.pop();
            std::cout << "Visited " << currentVertex << std::endl;

            for (auto& adjPair : this->adjLists[currentVertex]) {
                label adjVertex = adjPair;
                if (visited.find(adjVertex)==visited.end()) {
                    visited.insert(adjVertex);
                    queue.push(adjVertex);
                }
            }
        }
    }

    std::vector<std::vector<label>> BFS(const label startVertex, const std::unordered_set<label> &terminals)
    {
        std::set<label> visited;

        std::map<label, label> predecessor;
        std::queue<label> queue;
        std::vector<std::vector<label>> paths;
        paths.reserve(terminals.size());

        visited.insert(startVertex);
        queue.push(startVertex);
        // predecessor[startVertex] = std::nullopt;
        // Start vertex has no predecessor

        while (!queue.empty()) {
            int vertex = queue.front();
            queue.pop();

            if (terminals.find(vertex)!=terminals.end()) {
                std::vector<label> path;
                label v=vertex;
                while(predecessor.find(v)!= predecessor.end()){
                    v = predecessor[v];
                    path.push_back(v);
                }
                std::reverse(path.begin(), path.end());
                paths.push_back(std::move(path));
            }

            for (int adjVertex : adjLists[vertex]) {
                if (visited.find(adjVertex) == visited.end()) {
                    visited.insert(adjVertex);
                    queue.push(adjVertex);
                    predecessor[adjVertex] = vertex;
                }
            }
        }
        return paths;
    }


    void exportToDot(const std::string& filename) {
        std::ofstream outfile(filename+".dot");
        outfile << "graph G {\n"; // Use "digraph G {" for directed graphs
        for (auto pair: this->adjLists) {
            label cur = pair.first;
            for (auto neighbor : pair.second) {
                outfile << "    " << cur << " -- " << neighbor <<";\n";
            }
        }
        outfile << "}\n";
        outfile.close();

        std::string command = "dot -Tpng " + filename+".dot" + " -o" + filename + ".png";
    system(command.c_str());
    }

    // Additional functions like removeEdge, printGraph, etc., can be added here.
};

}
#endif