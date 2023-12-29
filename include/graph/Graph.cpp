#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <unordered_set>


class Graph {
    int numVertices;
    std::vector<std::list<int>> adjLists;

public:
    // Function to find the minimum spanning tree using Prim's algorithm
    
    Graph(int vertices) : numVertices(vertices), adjLists(vertices) {}

    void addEdge(int src, int dest) {
        if(src < this->numVertices && dest < this->numVertices ){
            this->adjLists[src].push_back(dest);
            // For undirected graph, add edge from dest to src as well
            this->adjLists[dest].push_back(src);
        }
        else{
            throw std::invalid_argument("Erreur de paramètre");
        }
        
    }

    void BFS(const int startVertex) {
        if(startVertex > this->numVertices || startVertex < 0){
            throw std::invalid_argument("Erruer");
        }

        std::vector<bool> visited(this->numVertices, false);
        std::queue<int> queue;

        visited[startVertex] = true;
        queue.push(startVertex);

        while (!queue.empty()) {
            int currentVertex = queue.front();
            queue.pop();
            std::cout << "Visited " << currentVertex << std::endl;

            for (auto& adjPair : this->adjLists[currentVertex]) {
                int adjVertex = adjPair;
                if (!visited[adjVertex]) {
                    visited[adjVertex] = true;
                    queue.push(adjVertex);
                }
            }
        }
    }

    std::vector<std::vector<int>> BFS(const int startVertex, const std::unordered_set<int> &terminals)
    {
        std::vector<bool> visited(numVertices, false), isTerminal(numVertices, false);
        for (int terminal : terminals) {
            isTerminal[terminal] = true;
        }

        std::map<int, int> predecessor;
        std::queue<int> queue;
        std::vector<std::vector<int>> paths;
        paths.reserve(terminals.size());

        visited[startVertex] = true;
        queue.push(startVertex);
        predecessor[startVertex] = -1; // Start vertex has no predecessor

        while (!queue.empty()) {
            int vertex = queue.front();
            queue.pop();

            if (isTerminal[vertex]) {
                std::vector<int> path;
                for (int v = vertex; v != -1; v = predecessor[v]) {
                    path.push_back(v);
                }
                std::reverse(path.begin(), path.end());
                paths.push_back(std::move(path));
            }

            for (int adjVertex : adjLists[vertex]) {
                if (!visited[adjVertex]) {
                    visited[adjVertex] = true;
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
        for (int i = 0; i < this->numVertices; ++i) {
            for (auto neighbor : this->adjLists[i]) {
                if(i > neighbor){
                    outfile << "    " << i << " -- " << neighbor <<";\n";

                }
            }
        }
        outfile << "}\n";
        outfile.close();

        std::string command = "dot -Tpng " + filename+".dot" + " -o" + filename + ".png";
    system(command.c_str());
    }

    // Additional functions like removeEdge, printGraph, etc., can be added here.
};
