#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <queue>
#include <map>
#include <utility>
#include "../graph/tree.hpp"

#ifndef Alg_SEP
#define Alg_SEP

namespace lq{
    struct SubTree {
        Sttree* root;
        std::vector<Sttree*> leaves;
    };

    Sttree* findNode(Sttree* root, const int data) {
        if (root == nullptr) return nullptr;
        if (root->data == data) return root;

        for (auto child : root->children) {
            Sttree* result = findNode(child, data);
            if (result != nullptr) return result;
        }

        return nullptr;
    }

    std::map<int,std::vector<int>> findPath(Sttree* root , const std::vector<int> leaves){
        std::map<int,std::vector<int>> paths;
        std::queue<std::pair<Sttree*,std::vector<int>>> queue;

        std::vector<int> path = {root->data};
        queue.push(std::make_pair(root,path));
        while(!queue.empty()){
            std::pair<Sttree*, std::vector<int>>& frontElement = queue.front();
            Sttree* currentNode = frontElement.first;
            std::vector<int> path = frontElement.second;
            queue.pop();

            for(auto u: currentNode->children){
                std::vector<int> newPath = path;
                newPath.push_back(u->data);
                queue.push(std::make_pair(u,newPath));
            }
            if(std::find(leaves.begin(), leaves.end(), currentNode->data) != leaves.end()){
                paths[currentNode->data] = path ;
            }
        }
        return paths;
    }

    std::vector<SubTree> SEPARATE(Sttree* TcS, int pivot, std::set<int>& terminals, int alg) {
        std::cout << "----------" <<std::endl;
        traverse(TcS);
        std::cout << std::endl;
        
        std::vector<SubTree> T;
        std::set<int> R = { pivot };
        int rootData = pivot;
        terminals.erase(pivot);

        int item = 10;
        
        // Traverse the Steiner tree in breadth-first search order
        while (!terminals.empty()&&item) {
            item --;

            Sttree* root = findNode(TcS, rootData);
            if (root == nullptr) continue;

            std::set<Sttree*> Sroot = { root };
            SubTree TrootSroot = { root, {} };

            // Traverse TcS in BFS order and store vertices and edges
            std::queue<Sttree*> queue;
            queue.push(root);
            while(!queue.empty()){
                Sttree* currentNode = queue.front();
                queue.pop();
                for(auto u: currentNode->children){
                    queue.push(u);
                    if (terminals.find(u->data)!=terminals.end()) {
                        TrootSroot.leaves.push_back(u);
                        terminals.erase(u->data);
                        Sroot.insert(u);
                        if(u->children.size()>0){
                            R.insert(u->data);
                        }
                    }
                }
            }

            if (alg == 4) {
                T.push_back(TrootSroot);
            } else {
                for (Sttree* u : Sroot) {
                    if (u != root) {
                        SubTree TuSu = { u, { root } };
                        T.push_back(TuSu);
                    }
                }
            }
            R.erase(rootData);
            if(!R.empty()) rootData = *R.begin();
            else break; 
        }

        return T;
    }
}

#endif /* Alg_SEP */
