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
    std::vector<std::string> topDown1(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::string> gates, temp;

        for (auto child : root->children) {
            gates.push_back("cx q["+std::to_string(root->data)+"] "+" q["+std::to_string(child->data)+"];\n");
            if(leaves.find(child->data) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::string> bootomUp1(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::string> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }


    std::vector<std::string> bootomUp2(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::string> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                gates.push_back("cx q["+std::to_string(root->data)+"] "+" q["+std::to_string(child->data)+"];\n");
                temp = bootomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::string> topDown2(Sttree* root,const std::set<int> leaves){
        if (root == nullptr) return {};
        std::vector<std::string> gates, temp;

        for (auto child : root->children) {
            if(leaves.find(child->data) == leaves.end()){
                temp = bootomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    void rowOp(xt::xarray<int>& A, std::set<int>& terminals, Sttree* root, size_t alg) {
        auto Ts = seprate(root,root->data,terminals,alg);

        std::vector<std::string> temp;
        for(int i = Ts.size()-1; i>=0; --i){
            if(alg!=1){
                std::cout << "Bootom Up 1" << std::endl;
                temp = bootomUp1(Ts.at(i).root,Ts.at(i).leaves);
                std::reverse(temp.begin(),temp.end());
                for(auto item: temp){
                    std::cout << item;
                }
                if(alg != 4){

                }   
            }
            // Top-Down-1
            std::cout << "Top Down 1" << std::endl;
            temp = topDown1(Ts.at(i).root,Ts.at(i).leaves);
            for(auto item: temp){
                std::cout << item;
            }
            if(alg != 4){

            }
            // Bootom-Up-2
            std::cout << "Bootom up 2" << std::endl;
            temp = bootomUp2(Ts.at(i).root,Ts.at(i).leaves);
            std::reverse(temp.begin(),temp.end());
            for(auto item: temp){
                std::cout << item;
            }
            if(alg != 4){

            }
            if(alg!=1){
                // Top-Down-2
                std::cout << "Top Down 2" << std::endl;
                temp = topDown2(Ts.at(i).root,Ts.at(i).leaves);
                for(auto item: temp){
                    std::cout << item;
                }
                if(alg != 4){

                }   
            }
            if(alg==4){

            }
        }

    }
}

#endif /* ROW_OP */
