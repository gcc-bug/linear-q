#ifndef GRAPH_CLASS
#define GRAPH_CLASS

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <optional>
#include "../Config.hpp"

namespace lq{
class Graph {
private:
    std::set<label> vertices;
    std::map<label,std::list<label>> adjLists;

public:
    // Function to find the minimum spanning tree using Prim's algorithm
    
    Graph() {}

    inline bool inGraph(label vertice){
        return this->vertices.find(vertice) != this->vertices.end();
    }

    void addEdge(label src, label dest) {
        if(!inGraph(src)){
            std::cout << "new edge: "<< src << std::endl;
            this->vertices.insert(src);
        }
        if(!inGraph(dest)){
            std::cout << "new edge: "<< dest << std::endl;
            this->vertices.insert(dest);
        }
        this->adjLists[src].push_back(dest);
        this->adjLists[dest].push_back(src);
    }

    std::set<std::pair<label, label>> primMST() {
        std::set<std::pair<label, label>> mstEdges;
        std::map<label,int> key;
        std::map<label,bool> inMST;
        for(auto vertice:this->vertices){
            key[vertice] = std::numeric_limits<int>::max();
            inMST[vertice] = false;
        }
        
        std::priority_queue<std::pair<label, label>, std::vector<std::pair<label, label>>, std::greater<std::pair<label, label>>> minHeap;

        // Start with the first vertex
        label start = *this->vertices.begin();
        minHeap.push({0, start}); // (key, vertex)
        key[start] = 0;

        while (!minHeap.empty()) {
            // Extract the vertex with minimum key value
            label u = minHeap.top().second;
            minHeap.pop();

            if (inMST[u]) continue; // Skip if the vertex is already in MST
            inMST[u] = true; // Include vertex in MST

            // Iterate over the adjacent vertices
            for (auto &i : adjLists[u]) {
                label v = i;
                int weight = 1; // Assuming equal weights for simplicity

                // If v is not in MST and weight of (u,v) is smaller than current key of v
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    minHeap.push({key[v], v});
                    mstEdges.insert({u, v});
                }
            }
        }

        return mstEdges;
    }
    std::set<std::pair<label, label>> reduceMSTToSteinerTree(const std::set<label>& terminals) {
        std::set<std::pair<label, label>> mstEdges = this->primMST();
        std::set<std::pair<label, label>> steinerTree;
        std::map<label, int> degree;

        // Count the degree of each node in the MST
        for (const auto& edge : mstEdges) {
            degree[edge.first]++;
            degree[edge.second]++;
        }

        // Find non-terminal leaf nodes and remove them iteratively
        bool removed;
        do {
            removed = false;
            for (auto it = degree.begin(); it != degree.end();) {
                if (it->second == 1 && terminals.find(it->first) == terminals.end()) { // Non-terminal leaf node
                    // Remove this leaf node
                    label nodeToRemove = it->first;
                    it = degree.erase(it);
                    removed = true;

                    // Find and remove the corresponding edge
                    for (auto edgeIt = mstEdges.begin(); edgeIt != mstEdges.end(); ) {
                        if (edgeIt->first == nodeToRemove || edgeIt->second == nodeToRemove) {
                            degree[edgeIt->first == nodeToRemove ? edgeIt->second : edgeIt->first]--;
                            edgeIt = mstEdges.erase(edgeIt);
                        } else {
                            ++edgeIt;
                        }
                    }
                } else {
                    ++it;
                }
            }
        } while (removed);

        return mstEdges; 
    }
    std::vector<int> findPath(const std::set<label>& start_vertices, const std::set<label>& target) {
        std::queue<std::pair<label, std::vector<label>>> queue;
        std::set<std::pair<label, std::vector<label>>> visited;

        // Initialize queue with start vertices
        for (const auto& vertice : start_vertices) {
            std::vector<label> initialPath = {vertice};
            queue.push({vertice, initialPath});
            visited.insert({vertice, initialPath});
        }

        while (!queue.empty()) {
            auto front = queue.front();
            int current_vertex = front.first;
            std::vector<int> path_to_current = front.second;
            queue.pop();

            if (target.find(current_vertex) != target.end()) {
                // Target found, return the path
                return path_to_current;
            }

            for (const auto& adjacent_vertex : adjLists[current_vertex]) {
                std::vector<int> path_to_adjacent = path_to_current;
                path_to_adjacent.push_back(adjacent_vertex);

                if (visited.find({adjacent_vertex, path_to_adjacent}) == visited.end()) {
                    queue.push({adjacent_vertex, path_to_adjacent});
                    visited.insert({adjacent_vertex, path_to_adjacent});
                }
            }
        }

        throw std::runtime_error("No path found");
    }

    std::vector<std::vector<label>> findPaths(const label privot, std::set<label> termianls){
        std::vector<std::vector<label>> Paths;
        std::vector<label> path;
        std::set<label> startvertices= {privot};
        try{
            while(!termianls.empty()){
                path = findPath(startvertices,termianls);
                for(auto u:path){
                    startvertices.insert(u);
                }
                termianls.erase(path.back());
                Paths.push_back(path);
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << "still have terminal " << termianls.size(); 
            return Paths;
        }
        return Paths;
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
