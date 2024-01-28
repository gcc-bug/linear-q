#ifndef Alg_SEP
#define Alg_SEP

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <queue>
#include <map>
#include <utility>
#include "graph/tree.hpp"
#include "../Config.hpp"

namespace lq{
    struct SubTree {
        Sttree* root;
        std::set<label> leaves;
    };

    Sttree* findNode(Sttree* root, const label data) {
        if (root == nullptr) return nullptr;
        if (root->data == data) return root;

        for (auto child : root->children) {
            Sttree* result = findNode(child, data);
            if (result != nullptr) return result;
        }

        return nullptr;
    }
    
    std::vector<SubTree> seprate(Sttree* TcS, label pivot, std::set<label>& terminals, size_t alg) {
        // alg tag ={1,2,3} for different stitution of TF, while tag = 4 only for NW
        if(TcS->data != pivot || TcS->children.empty()){
            throw std::invalid_argument("Erruer");
        }

        std::vector<SubTree> T;
        std::queue<Sttree*> R;
        R.push(TcS);
        terminals.erase(pivot);

        Sttree* root;
        // Traverse the Steiner tree in breadth-first search order
        while (!R.empty()) {
            root = R.front();
            R.pop();
            SubTree TrootSroot = {root, {} };

            // Traverse TcS in BFS order and store vertices and edges
            std::queue<Sttree*> queue;
            queue.push(root);
            while(!queue.empty()){
                Sttree* currentNode = queue.front();
                queue.pop();
                for(auto u: currentNode->children){
                    if (terminals.find(u->data)!=terminals.end()) {
                        TrootSroot.leaves.insert(u->data);
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
