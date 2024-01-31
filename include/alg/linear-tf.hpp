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
    std::set<label> findTerminals(const LFMatrix& A, const size_t targetColumn, bool searchAfterColumn) {
        std::set<label> terminalLabels;
        const LabelIndexBiMap& labelMap = A.getBiMap();
        int startRow = searchAfterColumn ? targetColumn + 1 : 0;
        int totalRows = A.row(); 
        for (int row = startRow; row < totalRows; ++row) {
            std::cout << row << std::endl;
            if (A.at(row, targetColumn)) {
                terminalLabels.insert(labelMap.getLabel(row));
                std::cout << labelMap.getLabel(row) << " ";
            }
        }
        return terminalLabels;
    }
    void TransformMatrix(LFMatrix& A,const Graph& g, const size_t col){
        const LabelIndexBiMap& biMap = A.getBiMap();
        label pivot = biMap.getLabel(col);
        std::set<label> terminals = findTerminals(A,col,false);
    
        if(terminals.empty()) throw std::runtime_error("erruer");
        auto path = g.findPath({pivot},terminals);
        std::reverse(path.begin(), path.end());
        for(int i = 0; i < path.size()-1; ++i){
            A.CNOT_(path[i],path[i+1]);
        }
    }

    void processColumn(LFMatrix& A,const Graph& g, const size_t col, AlgSignal signal) {
        //TODO: fix bug here
        auto terminals = findTerminals(A,col,true);
        const LabelIndexBiMap& labelMap = A.getBiMap();
        label pivot = labelMap.getLabel(col);
        std::cout << "pivot: " << pivot << std::endl;
        if(!terminals.empty()){
            for(auto it: terminals){
                std::cout << it << " ";
            }
            std::cout << std::endl;
            auto st = g.SteinerTree(pivot,terminals);
            auto Ts = separate(st,terminals,signal);
            rowOp(A,Ts,signal);

            if(signal == AlgSignal::propagated){
                for(auto T: Ts){
                    label r = T.root->getData();
                    for(label leaf: T.leaves){
                        if(labelMap.getIndex(leaf) < labelMap.getIndex(r)){
                            auto path = g.findPath({r},{leaf});
                            Sttree* root = new Sttree(r);
                            root->insertChild(g.pathToTree(path));
                            std::vector<lq::SubTree> temp = {{root,{path.back()}}};
                            rowOp(A,temp,signal);
                        }
                    }
                }
            }
        }
    }

    void processMatrix(LFMatrix& A, Graph& g, AlgSignal alg) {
        for (size_t col = 0; col < A.col(); ++col) {
            if(!A.at(col,col)){
                TransformMatrix(A,g,col);
            }
            processColumn(A, g, col, alg);
            g.deleteVertex(A.getBiMap().getLabel(col));
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
        processMatrix(A,*g_,AlgSignal::diag);
        A.transpose();
        g_ = g->clone();
        processMatrix(A,*g_,AlgSignal::propagated);
    }
}
#endif /* ALG_LINEAR_SYNTH */
