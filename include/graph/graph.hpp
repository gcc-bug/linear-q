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
#include "tree.hpp"

namespace lq{
class Graph {
private:
    std::set<label> vertices;
    std::map<label,std::set<label>> neigh;

public:
    // Function to find the minimum spanning tree using Prim's algorithm
    
    Graph() {}

    inline bool inGraph(label verite) const{
        return this->vertices.find(verite) != this->vertices.end();
    }

    std::set<label> get_neigh(label v) const{
        if(inGraph(v)){
            return this->neigh.at(v);
        }
        throw std::runtime_error("errur");
    }

    void addEdge(label src, label dest) {
        if(!inGraph(src)){
            this->vertices.insert(src);
        }
        if(!inGraph(dest)){
            this->vertices.insert(dest);
        }
        this->neigh[src].insert(dest);
        this->neigh[dest].insert(src);
    }
    
    Graph* clone() const {
        Graph* newGraph = new Graph();

        // Copy vertices
        newGraph->vertices = this->vertices;

        // Copy adjacency lists
        for (const auto& pair : this->neigh) {
            newGraph->neigh[pair.first] = pair.second;
        }

        return newGraph;
    }

    // Function to delete a vertex

    void deleteVertex(label vertex) {
        // Check if the vertex exists
        if (vertices.find(vertex) == vertices.end()) {
            return; // Vertex not found
        }

        // Remove the vertex from the adjacency lists of other vertices
        for (label neighbor : neigh[vertex]) {
            neigh[neighbor].erase(vertex);
        }
        // Remove all edges associated with this vertex
        neigh.erase(vertex);
        
        // Remove the vertex from the set of vertices
        vertices.erase(vertex);
    }


    std::set<std::pair<label, label>> primMST() {
        std::set<std::pair<label, label>> mstEdges;
        std::map<label,int> key;
        std::map<label,bool> inMST;
        for(auto verite:this->vertices){
            key[verite] = std::numeric_limits<int>::max();
            inMST[verite] = false;
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
            for (auto &i : neigh[u]) {
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
    std::vector<label> findPath(const std::set<label>& start_vertices, const std::set<label>& targets) {
        std::queue<std::pair<label, std::vector<label>>> queue;
        std::set<std::pair<label, std::vector<label>>> visited;

        // Initialize queue with start vertices
        for (const auto& verite : start_vertices) {
            std::vector<label> initialPath = {verite};
            queue.push({verite, initialPath});
            visited.insert({verite, initialPath});
        }

        while (!queue.empty()) {
            auto front = queue.front();
            label current_vertex = front.first;
            auto path_to_current = front.second;
            queue.pop();

            if (targets.find(current_vertex) != targets.end()) {
                // Target found, return the path
                return path_to_current;
            }

            for (const auto& adjacent_vertex : neigh[current_vertex]) {
                auto path_to_adjacent = path_to_current;
                path_to_adjacent.push_back(adjacent_vertex);

                if (visited.find({adjacent_vertex, path_to_adjacent}) == visited.end()) {
                    queue.push({adjacent_vertex, path_to_adjacent});
                    visited.insert({adjacent_vertex, path_to_adjacent});
                }
            }
        }

        throw std::runtime_error("No path found");
    }

    Sttree* SteinerTree(const label pivot, std::set<label> terminals){
        std::vector<label> path;
        std::set<label> start_vertices= {pivot};
        Sttree* root = new Sttree(pivot);
        Sttree* current_node;
        Sttree* next_node;
        try{
            while(!terminals.empty()){
                path = findPath(start_vertices,terminals);
                current_node = root->findNode(path.front());
                for(auto it = std::next(path.begin()); it != path.end(); ++it) {
                    auto u = *it;
                    start_vertices.insert(u);

                    next_node = new Sttree(u);
                    insertChild(current_node, next_node);
                    current_node = next_node;
                }

                terminals.erase(path.back());
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << "still have terminal " << terminals.size();
        }
        return root;
    }

    void exportToDot(const std::string& filename) {
        std::ofstream outfile(filename+".dot");
        outfile << "graph G {\n"; // Use "digraph G {" for directed graphs
        for (auto pair: this->neigh) {
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
