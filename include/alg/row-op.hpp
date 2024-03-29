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
    std::vector<std::pair<label,label>> topDown1(const std::shared_ptr<Sttree> root,const std::set<label>& leaves){
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

    std::vector<std::pair<label,label>> bottomUp1(const std::shared_ptr<Sttree> root,const std::set<label>& leaves){
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


    std::vector<std::pair<label,label>> bottomUp2(const std::shared_ptr<Sttree> root,const std::set<label>& leaves){
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

    std::vector<std::pair<label,label>> topDown2(const std::shared_ptr<Sttree> root,const std::set<label>& leaves){
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
        
        for(int i = Ts.size()-1; i >= 0; --i){
            std::shared_ptr<Sttree> root = Ts.at(i).getRoot();
            std::set<label> leaves = Ts.at(i).getLeaves();
            if(alg != AlgSignal::diag){
                // Bottom Up 1
                paths = bottomUp1(root,leaves);
                std::reverse(paths.begin(),paths.end());
                for(auto labels: paths){
                    res.push_back(CNOTGate(labels.first,labels.second));
                    if(alg != AlgSignal::phase){
                        A.mod2add(labels.second,labels.first);
                    }
                }   
            }
            // Top-Down-1
            paths = topDown1(root,leaves);
            for(auto labels: paths){
                res.push_back(CNOTGate(labels.first,labels.second));
                if(alg != AlgSignal::phase){
                    A.mod2add(labels.second,labels.first);
                }
            }
            // Bottom-Up-2
            paths = bottomUp2(root,leaves);
            std::reverse(paths.begin(),paths.end());
            for(auto labels: paths){
                res.push_back(CNOTGate(labels.first,labels.second));
                if(alg != AlgSignal::phase){
                    A.mod2add(labels.second,labels.first);
                }
            }
            if(alg != AlgSignal::diag){
                // Top-Down-2
                paths = topDown2(root,leaves);
                for(auto labels: paths){
                    res.push_back(CNOTGate(labels.first,labels.second));
                    if(alg != AlgSignal::phase){
                        A.mod2add(labels.second,labels.first);
                    }
                }   
            }
            if(alg==AlgSignal::phase){
                for(auto leaf: Ts.at(i).getLeaves()){
                    A.mod2add(Ts.at(i).getRoot()->getData(),leaf);
                }
            }
        }
        return res;
    }
}

#endif /* ALG_ROW_OP */
