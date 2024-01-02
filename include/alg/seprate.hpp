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
        int root;
        std::vector<int> leaves;
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

    std::vector<SubTree> SEPARATE(Sttree* TcS, int pivot, std::set<int>& terminals, size_t alg) {
        // alg tag ={1,2,3} for different stitution of TF, while tag = 4 only for NW
        if(TcS->data != pivot || TcS->children.empty()){
            throw std::invalid_argument("Erruer");
        }

        std::vector<SubTree> T;
        std::queue<Sttree*> R;
        R.push(TcS);
        terminals.erase(pivot);

        int item = 10;
        Sttree* root;
        // Traverse the Steiner tree in breadth-first search order
        while (!R.empty()&&item) {
            item --;

            root = R.front();
            R.pop();
            SubTree TrootSroot = {root->data, {} };

            // Traverse TcS in BFS order and store vertices and edges
            std::queue<Sttree*> queue;
            queue.push(root);
            while(!queue.empty()){
                Sttree* currentNode = queue.front();
                queue.pop();
                for(auto u: currentNode->children){
                    if (terminals.find(u->data)!=terminals.end()) {
                        TrootSroot.leaves.push_back(u->data);
                        if(u->children.size()>0){
                            R.push(u);
                        }
                    }
                    else{
                        queue.push(u);
                    }
                }
            }

            if (alg == 4) {
                // TODO: need modify
                T.push_back(TrootSroot);
            } else {
                T.push_back(TrootSroot);
            }
        }

        return T;
    }
}

#endif /* Alg_SEP */
