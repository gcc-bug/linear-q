/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-01-29 15:26:25
 * @Description  : this file is about algorithm linear synthesis
 * @TODO         : finish B[n] part
 */
#ifndef ALG_LINEAR_SYNTH
#define ALG_LINEAR_SYNTH

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
            if (A.at(row, targetColumn)) {
                terminalLabels.insert(labelMap.getLabel(row));
            }
        }
        return terminalLabels;
    }
    std::vector<CNOTGate> TransformMatrix(LFMatrix& A,const Graph& g, const size_t col){
        const LabelIndexBiMap& biMap = A.getBiMap();
        std::vector<CNOTGate> res;
        label pivot = biMap.getLabel(col);
        std::set<label> terminals = findTerminals(A,col,false);
        // TODO: if it is need to find terminals after col?
    
        if(terminals.empty()) throw std::runtime_error("erruer");
        auto path = g.findPath({pivot},terminals);
        std::reverse(path.begin(), path.end());
        
        for(int i = 0; i < path.size()-1; ++i){
            res.push_back(A.CNOT_(path[i],path[i+1]));
        }
        return res;
    }

    std::vector<CNOTGate> processColumn(LFMatrix& A,const Graph& g, const size_t col, AlgSignal signal) {
        auto terminals = findTerminals(A,col,true);
        const LabelIndexBiMap& labelMap = A.getBiMap();
        std::vector<CNOTGate> res, temp_gates;

        label pivot = labelMap.getLabel(col);
        if(!terminals.empty()){
            auto st = g.SteinerTree(pivot,terminals);
            auto Ts = separate(st,terminals,signal);
            temp_gates = rowOp(A,Ts,signal);
            res.insert(res.end(),temp_gates.begin(),temp_gates.end());

            if(signal == AlgSignal::propagated){
                // TODO: optimize code structure
                for(auto T: Ts){
                    label r = T.getRoot()->getData();
                    for(label leaf: T.getLeaves()){
                        if(labelMap.getIndex(leaf) < labelMap.getIndex(r)){
                            auto path = g.findPath({r},{leaf});
                            auto root = std::make_shared<Sttree>(r);
                            root->insertChild(g.pathToTree(path));
                            std::vector<lq::SubTree> temp = {SubTree(root,{path.back()})};
                            temp_gates = rowOp(A,temp,signal);
                            res.insert(res.end(),temp_gates.begin(),temp_gates.end());
                        }
                    }
                }
            }
        }
        return res;
    }

    std::vector<CNOTGate> processMatrix(LFMatrix& A, Graph& g, AlgSignal alg) {
        std::vector<CNOTGate> res,temp_gates;
        for (size_t col = 0; col < A.col(); ++col) {
            if(!A.at(col,col)){
                temp_gates = TransformMatrix(A, g, col);
                res.insert(res.end(), temp_gates.begin(), temp_gates.end());
            }
            temp_gates = processColumn(A, g, col, alg);
            res.insert(res.end(), temp_gates.begin(), temp_gates.end());
            g.deleteVertex(A.getBiMap().getLabel(col));
        }
        return res;
    }

    
    
    std::vector<CNOTGate> linearSynth(LFMatrix& A,const Graph* g){
        int n = A.getData().shape()[0];
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
        auto y1 = processMatrix(A,*g_,AlgSignal::diag);

        A.transpose();
        // std::cout << A.getData() << std::endl;
        g_ = g->clone();
        auto y2 = processMatrix(A,*g_,AlgSignal::propagated);
        
        std::reverse(y1.begin(),y1.end());
        for(CNOTGate& gate: y2){
            // Iterating by value
            gate.reverse();
        }
        y2.insert(y2.end(),y1.begin(),y1.end());
        return y2;
    }
}
#endif /* ALG_LINEAR_SYNTH */
