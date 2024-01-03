#ifndef ROW_OP
#define ROW_OP

#include <vector>
#include <set>
#include "xtensor/xarray.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xbuilder.hpp" 
#include <iostream>
#include <string>
#include "seprate.hpp"

namespace lq {
    std::vector<std::pair<int,int>> topDown1(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<int,int>> gates, temp;

        for (auto child : root->children) {
            gates.push_back(std::make_pair(root->data,child->data));
            if(leaves.find(child->data) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<int,int>> bootomUp1(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<int,int>> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }


    std::vector<std::pair<int,int>> bootomUp2(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<int,int>> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                gates.push_back(std::make_pair(root->data,child->data));
                temp = bootomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<int,int>> topDown2(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<int,int>> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                temp = bootomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    xt::xarray<int> mod2add(xt::xarray<int>& A, int row_i, int row_j){
        row_i --;
        row_j --;
        if (row_i < A.shape()[0] && row_j < A.shape()[0]) {
            xt::view(A, row_i, xt::all()) = xt::eval(xt::view(A, row_i, xt::all()) + xt::view(A, row_j, xt::all())) % 2;
            return A;
        }
        else{
            throw std::invalid_argument("errur");
        }
    }

    void rowOp(xt::xarray<int>& A, std::set<int>& terminals, Sttree* root, size_t alg) {
        auto Ts = seprate(root,root->data,terminals,alg);

        std::vector<std::pair<int,int>> paths;
        for(int i = Ts.size()-1; i>=0; --i){
            if(alg!=1){
                std::cout << "Bootom Up 1" << std::endl;
                paths = bootomUp1(Ts.at(i).root,Ts.at(i).leaves);
                std::reverse(paths.begin(),paths.end());
                for(auto qubits: paths){
                    std::cout << qubits.first << " " << qubits.second << "\n";
                    if(alg != 4){
                        A = mod2add(A,qubits.second,qubits.first);
                    }
                }   
            }
            // Top-Down-1
            std::cout << "Top Down 1" << std::endl;
            paths = topDown1(Ts.at(i).root,Ts.at(i).leaves);
            for(auto qubits: paths){
                std::cout << qubits.first << " " << qubits.second << "\n";
                if(alg != 4){
                    A = mod2add(A,qubits.second,qubits.first);
                }
            }
            // Bootom-Up-2
            std::cout << "Bootom up 2" << std::endl;
            paths = bootomUp2(Ts.at(i).root,Ts.at(i).leaves);
            std::reverse(paths.begin(),paths.end());
            for(auto qubits: paths){
                std::cout << qubits.first << " " << qubits.second << "\n";
                if(alg != 4){
                    A = mod2add(A,qubits.second,qubits.first);
                }
            }
            if(alg!=1){
                // Top-Down-2
                std::cout << "Top Down 2" << std::endl;
                paths = topDown2(Ts.at(i).root,Ts.at(i).leaves);
                for(auto qubits: paths){
                    std::cout << qubits.first << " " << qubits.second << "\n";
                    if(alg != 4){
                        A = mod2add(A,qubits.second,qubits.first);
                    }
                }   
            }
            if(alg==4){
                for(auto leaf: terminals){
                    A = mod2add(A,root->data,leaf);
                }
            }
        }
    }
}

#endif /* ROW_OP */
