/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-01-27 22:26:28
 * @Description  : this file is about algorithm row operation
 * @ToDo         : reduce the four tree function to one
 */
#ifndef ALG_ROW_OP
#define ALG_ROW_OP

#include <vector>
#include <set>
#include <iostream>
#include <string>
#include "../Config.hpp"
#include "typedef.hpp"

namespace lq {
    std::vector<std::pair<label,label>> topDown1(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->get_children()) {
            gates.push_back(std::make_pair(root->get_data(),child->get_data()));
            if(leaves.find(child->get_data()) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<label,label>> bottomUp1(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->get_children()) {
            if(leaves.find(child->get_data()) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }


    std::vector<std::pair<label,label>> bottomUp2(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->get_children()) {
            if(leaves.find(child->get_data()) == leaves.end()){
                gates.push_back(std::make_pair(root->get_data(),child->get_data()));
                temp = bottomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<label,label>> topDown2(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->get_children()) {
            if(leaves.find(child->get_data()) == leaves.end()){
                temp = bottomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    void rowOp(LFMatrix& A, std::vector<SubTree>& Ts, AlgSignal alg) {
        std::vector<std::pair<label,label>> paths;
        for(label i = Ts.size()-1; i>=0; --i){
            if(alg != AlgSignal::diag){
                std::cout << "Bottom Up 1" << std::endl;
                paths = bottomUp1(Ts.at(i).root,Ts.at(i).leaves);
                std::reverse(paths.begin(),paths.end());
                for(auto labels: paths){
                    CNOT(labels.first,labels.second);
                    if(alg != AlgSignal::phase){
                        A.mod2add(labels.second,labels.first);
                    }
                }   
            }
            // Top-Down-1
            std::cout << "Top Down 1" << std::endl;
            paths = topDown1(Ts.at(i).root,Ts.at(i).leaves);
            for(auto labels: paths){
                CNOT(labels.first,labels.second);
                if(alg != AlgSignal::phase){
                    A.mod2add(labels.second,labels.first);
                }
            }
            // Bottom-Up-2
            std::cout << "Bottom up 2" << std::endl;
            paths = bottomUp2(Ts.at(i).root,Ts.at(i).leaves);
            std::reverse(paths.begin(),paths.end());
            for(auto labels: paths){
                CNOT(labels.first,labels.second);
                if(alg != AlgSignal::phase){
                    A.mod2add(labels.second,labels.first);
                }
            }
            if(alg != AlgSignal::diag){
                // Top-Down-2
                std::cout << "Top Down 2" << std::endl;
                paths = topDown2(Ts.at(i).root,Ts.at(i).leaves);
                for(auto labels: paths){
                    CNOT(labels.first,labels.second);
                    if(alg != AlgSignal::phase){
                        A.mod2add(labels.second,labels.first);
                    }
                }   
            }
            // if(alg==4){
            //     for(auto leaf: terminals){
            //         mod2add(A,label2qubit.get_qubit(root->get_data()),label2qubit.get_qubit(leaf));
            //     }
            // }
        }
    }
}

#endif /* ALG_ROW_OP */
