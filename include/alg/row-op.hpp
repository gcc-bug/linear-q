/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-01-27 22:26:28
 * @Description  : this file is about algorithm row operation
 * @TODO         : reduce the four tree function to one
 */
#ifndef ALG_ROW_OP
#define ALG_ROW_OP

#include "typedef.hpp"

namespace lq {
    std::vector<std::pair<label,label>> topDown1(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->getChildren()) {
            gates.push_back(std::make_pair(root->getData(),child->getData()));
            if(leaves.find(child->getData()) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<label,label>> bottomUp1(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->getChildren()) {
            if(leaves.find(child->getData()) == leaves.end()){
                temp = topDown1(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }


    std::vector<std::pair<label,label>> bottomUp2(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->getChildren()) {
            if(leaves.find(child->getData()) == leaves.end()){
                gates.push_back(std::make_pair(root->getData(),child->getData()));
                temp = bottomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<std::pair<label,label>> topDown2(Sttree* root,const std::set<label> leaves){
        if (root == nullptr) return {};
        std::vector<std::pair<label,label>> gates, temp;

        for (auto child : root->getChildren()) {
            if(leaves.find(child->getData()) == leaves.end()){
                temp = bottomUp2(child, leaves);
                gates.insert(gates.end(),temp.begin(),temp.end());
            }
        }
        
        return gates;
    }

    std::vector<CNOTGate> rowOp(LFMatrix& A, std::vector<SubTree>& Ts, AlgSignal alg) {
        std::vector<std::pair<label,label>> paths;
        std::vector<CNOTGate> res;
        for(label i = Ts.size()-1; i>=0; --i){
            if(alg != AlgSignal::diag){
                // Bottom Up 1
                paths = bottomUp1(Ts.at(i).root,Ts.at(i).leaves);
                std::reverse(paths.begin(),paths.end());
                for(auto labels: paths){
                    res.push_back(CNOTGate(labels.first,labels.second));
                    if(alg != AlgSignal::phase){
                        A.mod2add(labels.second,labels.first);
                    }
                }   
            }
            // Top-Down-1
            paths = topDown1(Ts.at(i).root,Ts.at(i).leaves);
            for(auto labels: paths){
                res.push_back(CNOTGate(labels.first,labels.second));
                if(alg != AlgSignal::phase){
                    A.mod2add(labels.second,labels.first);
                }
            }
            // Bottom-Up-2
            paths = bottomUp2(Ts.at(i).root,Ts.at(i).leaves);
            std::reverse(paths.begin(),paths.end());
            for(auto labels: paths){
                res.push_back(CNOTGate(labels.first,labels.second));
                if(alg != AlgSignal::phase){
                    A.mod2add(labels.second,labels.first);
                }
            }
            if(alg != AlgSignal::diag){
                // Top-Down-2
                paths = topDown2(Ts.at(i).root,Ts.at(i).leaves);
                for(auto labels: paths){
                    res.push_back(CNOTGate(labels.first,labels.second));
                    if(alg != AlgSignal::phase){
                        A.mod2add(labels.second,labels.first);
                    }
                }   
            }
            // if(alg==4){
            //     for(auto leaf: terminals){
            //         mod2add(A,LabelIndexBiMap.getIndex(root->getData()),LabelIndexBiMap.getIndex(leaf));
            //     }
            // }
        }
        return res;
    }
}

#endif /* ALG_ROW_OP */
