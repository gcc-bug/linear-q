/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-01-29 15:26:25
 * @Description  : this file is about algorithm linear synthesis
 * @TODO         : finish B[n] part
 */
#ifndef ALG_LINEAR_SYNTH
#define ALG_LINEAR_SYNTH

#include "graph/graph.hpp"
#include "row-op.hpp"
#include "seprate.hpp"
#include "typedef.hpp"

namespace lq{
    void TransformMatrix(LFMatrix& A,const Graph* g, int col){
        const LabelIndexBiMap& biMap = A.getBiMap();
        label pivot = biMap.getLabel(col);
        std::set<label> terminals;
        for(int row = 0 ; row < A.getData().shape()[0]; ++row){
            if(A.at(row,col)){
                terminals.insert(biMap.getLabel(row));
            }
        }
        if(terminals.empty()) throw std::runtime_error("erruer");
        auto path = g->findPath({pivot},terminals);
        std::reverse(path.begin(), path.end());
        for(int i = 0; i < path.size()-1; ++i){
            A.CNOT_(path[i],path[i+1]);
        }
    }
    
    void linearSynth(LFMatrix& A,const Graph* g){
        int n = A.getData().shape()[0];
        const LabelIndexBiMap& biMap = A.getBiMap();
        // add x gate
        // for(int row =0; row < A.shape()[1]; ++row){
        //     if(A(n,row)){
        //         std::cout << row + 1 << " ";
        //         A(n,row) = 0; 
        //     }
        // }
        // std::cout << std::endl;

        // Cloning the graph: Create a duplicate G' of the graph G for manipulation
        Graph* g_ = g->clone();
        for(int col = 0; col < n; ++col){
            if(!A.at(col,col)){
                TransformMatrix(A,g_,col);
            }
            // find terminals
            label pivot = biMap.getLabel(col);
            std::set<label> terminals;
            for(int row = col+1; row < n; ++row){
                if(A.at(row,col)){
                    terminals.insert(biMap.getLabel(row));
                }
            }
            std::cout << std::endl;
            if(!terminals.empty()){
                auto st = g_->SteinerTree(pivot,terminals);
                auto Ts = separate(st,terminals,AlgSignal::diag);
                rowOp(A,Ts,AlgSignal::diag);
            }
            g_->deleteVertex(biMap.getLabel(col));
        }


        A.transpose();
        g_ = g->clone();
        std::cout << "-----------------------\n";
        for(int col = 0; col < n; ++col){
            if(!A.at(col,col)){
                TransformMatrix(A,g_,col);
            }
            // find terminals
            label pivot = biMap.getLabel(col);
            std::cout <<"pivot: "<< pivot << std::endl;

            std::set<label> terminals;
            for(int row = col+1; row < n; ++row){
                if(A.at(row,col)){
                    terminals.insert(biMap.getLabel(row));
                    std::cout << biMap.getLabel(row) <<" ";
                }
            }
            std::cout << std::endl;
            if(!terminals.empty()){
                auto st = g_->SteinerTree(pivot,terminals);
                st->traverse();
                std::cout << std::endl;
                auto Ts = separate(st,terminals,AlgSignal::propagated);
                rowOp(A,Ts,AlgSignal::propagated);

                for(auto T: Ts){
                    label r = T.root->getData();
                    for(label leaf: T.leaves){
                        if(biMap.getIndex(leaf) < biMap.getIndex(r)){
                            auto path = g_->findPath({r},{leaf});
                            Sttree* root = new Sttree(r);
                            Sttree* current = root;
                            for(auto it = std::next(path.begin()); it != path.end(); ++it) {
                                auto u = *it;
                                current->insertChild(u);
                                current = *current->getChildren().begin();
                            }
                            std::vector<lq::SubTree> temp = {{root,{path.back()}}};
                            rowOp(A,temp,AlgSignal::propagated);
                        }
                    }
                    std::cout << std::endl;
                }
            }

            // for(path qubit in Ts) do something

            
            g_->deleteVertex(biMap.getLabel(col));
            std::cout << A.getData() <<std::endl;

        }

    }
}
#endif /* ALG_LINEAR_SYNTH */
