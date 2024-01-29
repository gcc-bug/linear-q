#ifndef ALG_ROW_OP
#define ALG_ROW_OP

#include <vector>
#include <set>
#include "xtensor/xarray.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xbuilder.hpp" 
#include <iostream>
#include <string>
#include "seprate.hpp"
#include "../Config.hpp"
#include "Bimap.hpp"

namespace lq {
    inline void CNOT(label control, label target){
        std::cout << "CNOT " << control <<" " << target << std::endl;
    }
    std::vector<std::pair<label,label>> topDown1(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->children) {
            gates.push_back(std::make_pair(root->data,child->data));
            if(leaves.find(child->data) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<label,label>> bottomUp1(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }


    std::vector<std::pair<label,label>> bottomUp2(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                gates.push_back(std::make_pair(root->data,child->data));
                temp = bottomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<label,label>> topDown2(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                temp = bottomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    void mod2add(xt::xarray<bool>& A, int row_i, int row_j){
        if (row_i < A.shape()[0] && row_j < A.shape()[0]) {
            xt::view(A, row_i, xt::all()) = xt::view(A, row_i, xt::all()) ^ xt::view(A, row_j, xt::all());
            return ;
        }
        else{
            throw std::invalid_argument("errur");
        }
    }

    void rowOp(xt::xarray<bool>& A, std::set<label>& terminals, Sttree* root, const size_t alg, const label2qubit label2qubit) {
                auto Ts = separate(root,root->data,terminals,alg);

        std::vector<std::pair<label,label>> paths;
        for(label i = Ts.size()-1; i>=0; --i){
            if(alg!=1){
                std::cout << "Bottom Up 1" << std::endl;
                paths = bottomUp1(Ts.at(i).root,Ts.at(i).leaves);
                std::reverse(paths.begin(),paths.end());
                for(auto labels: paths){
                    CNOT(labels.first,labels.second);
                    if(alg != 4){
                        mod2add(A,label2qubit.getqubit(labels.second),label2qubit.getqubit(labels.first));
                    }
                }   
            }
            // Top-Down-1
            std::cout << "Top Down 1" << std::endl;
            paths = topDown1(Ts.at(i).root,Ts.at(i).leaves);
            for(auto labels: paths){
                CNOT(labels.first,labels.second);
                if(alg != 4){
                    mod2add(A,label2qubit.getqubit(labels.second),label2qubit.getqubit(labels.first));
                }
            }
            // Bottom-Up-2
            std::cout << "Bottom up 2" << std::endl;
            paths = bottomUp2(Ts.at(i).root,Ts.at(i).leaves);
            std::reverse(paths.begin(),paths.end());
            for(auto labels: paths){
                CNOT(labels.first,labels.second);
                if(alg != 4){
                    mod2add(A,label2qubit.getqubit(labels.second),label2qubit.getqubit(labels.first));
                }
            }
            if(alg!=1){
                // Top-Down-2
                std::cout << "Top Down 2" << std::endl;
                paths = topDown2(Ts.at(i).root,Ts.at(i).leaves);
                for(auto labels: paths){
                    CNOT(labels.first,labels.second);
                    if(alg != 4){
                        mod2add(A,label2qubit.getqubit(labels.second),label2qubit.getqubit(labels.first));
                    }
                }   
            }
            if(alg==4){
                for(auto leaf: terminals){
                    mod2add(A,label2qubit.getqubit(root->data),label2qubit.getqubit(leaf));
                }
            }
        }
    }
}

#endif /* ALG_ROW_OP */
