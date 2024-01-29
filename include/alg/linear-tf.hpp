#ifndef ALG_LINEAR_SYNTH
#define ALG_LINEAR_SYNTH

#include "graph/graph.hpp"
#include "row-op.hpp"

namespace lq{
    void makeTrue(xt::xarray<bool>& A,const Graph* g,const label2qubit label2qubit,int col){
        label pivot = label2qubit.getlabel(col);
        std::set<label> visited = {pivot};
        std::vector<label> path;
        std::queue<std::pair<label, std::vector<label>>> queue;
        queue.push({pivot,{pivot}});

        while(!queue.empty()){
            auto front = queue.front();
            label current_vertex = front.first;
            auto path_to_current = front.second;
            queue.pop();

            for(auto u: g->get_neigh(current_vertex)){
                path = path_to_current;
                path.push_back(u);
                if(A(label2qubit.getqubit(u),col)) break;
                if(visited.find(u) == visited.end()){
                    queue.push({u,path});
                    visited.insert(u);
                }
            }
        }
        std::reverse(path.begin(), path.end());
        for(int i = 0; i < path.size()-1; ++i){
            std::cout << path[i] <<" " << path[i+1] << std::endl;
            mod2add(A,label2qubit.getqubit(path[i+1]),label2qubit.getqubit(path[i]));
        }
    }

    void linearSynth(xt::xarray<bool>& A,const Graph* g,const label2qubit label2qubit){
        int n = A.shape()[0];
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
            if(!A(col,col)){
                makeTrue(A,g_,label2qubit,col);
                std::cout<<col<<":\n"<< A << std::endl;
                // Path Finding: Identify a short path to a node 'j' where A(col,j) = 1.
                // Then, for the path from 'col' to 'j', apply CNOT starting from 'col' and ending at 'j', 
                // subsequently updating the matrix A accordingly.
            }
            // find terminals
            label pivot = label2qubit.getlabel(col);
            std::cout <<"pivot: "<< pivot << std::endl;

            std::set<label> terminals;
            for(int row = col+1; row < A.shape()[1]; ++row){
                if(A(row,col)){
                    terminals.insert(label2qubit.getlabel(row));
                    std::cout << label2qubit.getlabel(row) <<" ";
                }
            }
            std::cout << std::endl;
            auto st = g_->SteinerTree(pivot,terminals);
            traverse(st);
            std::cout << std::endl;

            // Steiner Tree Construction: (currently commented out)
            // This section is intended to find a Steiner tree that connects the set of terminal nodes.
            // The tree minimizes the total length of the paths from each terminal to the root.
            // rowOp(A,terminals,St,1);
            rowOp(A,terminals,st,1,label2qubit);
            g_->deleteVertex(label2qubit.getlabel(col));
            std::cout << A <<std::endl;
            // After constructing the tree, remove 'col' from G'.
        }


        A = xt::transpose(A);
        for(int col = 0; col < n; ++col){
            if(!A(col,col)){
                // Path Finding: Identify a short path to a node 'j' where A(col,j) = 1.
                // Then, for the path from 'col' to 'j', apply CNOT starting from 'col' and ending at 'j', 
                // subsequently updating the matrix A accordingly.
            }
            // find terminals
            std::set<int> terminals;
            for(int qubit = col; qubit < A.shape()[1]-1; ++qubit){
                if(A(col,qubit)){
                    terminals.insert(qubit+1);
                    std::cout << col <<" ";
                }
            }
            std::cout << std::endl;

            // Steiner Tree Construction: (currently commented out)
            // This section is intended to find a Steiner tree that connects the set of terminal nodes.
            // The tree minimizes the total length of the paths from each terminal to the root.
            // rowOp(A,terminals,St,2);
            std::vector<int> B(n);
            for(int qubit = col+1; qubit < A.shape()[1]-1; ++qubit){
                if(terminals.find(col)!=terminals.end()){
                    B.at(col) = col;
                }
            }

            // for(path qubit in Ts) do something

        }

    }
}
#endif /* ALG_LINEAR_SYNTH */
