#ifndef ALG_LINEAR_SYNTH
#define ALG_LINEAR_SYNTH

#include "../graph/graph.hpp"
#include "row-op.hpp"

namespace lq{
    void linearSynth(xt::xarray<int> A){
        // add x gate
        int n = A.shape()[0] - 1;
        for(int qubit =0; qubit < A.shape()[1]; ++qubit){
            if(A(n,qubit)){
                std::cout << qubit + 1 << " ";
                A(n,qubit) = 0; 
            }
        }
        std::cout << std::endl;

        // Cloning the graph: Create a duplicate G' of the graph G for manipulation
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
            // rowOp(A,terminals,St,1);
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
