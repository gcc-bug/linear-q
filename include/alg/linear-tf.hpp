#ifndef ALG_LINEAR_SYNTH
#define ALG_LINEAR_SYNTH

#include "graph/graph.hpp"
#include "row-op.hpp"

namespace lq{
    void TransformMatrix(LFMatrix& A,const Graph* g, int col){
        label2qubit l2q = A.get_l2q();
        label pivot = l2q.get_label(col);
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
                if(A.get_ele(l2q.get_qubit(u),col)) break;
                if(visited.find(u) == visited.end()){
                    queue.push({u,path});
                    visited.insert(u);
                }
            }
        }
        std::reverse(path.begin(), path.end());
        for(int i = 0; i < path.size()-1; ++i){
            A.CNOT_(path[i],path[i+1]);
        }
    }

    void linearSynth(LFMatrix& A,const Graph* g){
        int n = A.get_data().shape()[0];
        label2qubit l2q = A.get_l2q();
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
            if(!A.get_ele(col,col)){
                TransformMatrix(A,g_,col);
                std::cout<<col<<":\n" << A.get_data() << std::endl;
                // Path Finding: Identify a short path to a node 'j' where A(col,j) = 1.
                // Then, for the path from 'col' to 'j', apply CNOT starting from 'col' and ending at 'j', 
                // subsequently updating the matrix A accordingly.
            }
            // find terminals
            label pivot = l2q.get_label(col);
            std::cout <<"pivot: "<< pivot << std::endl;

            std::set<label> terminals;
            for(int row = col+1; row < n; ++row){
                if(A.get_ele(row,col)){
                    terminals.insert(l2q.get_label(row));
                    std::cout << l2q.get_label(row) <<" ";
                }
            }
            std::cout << std::endl;
            if(!terminals.empty()){
                auto st = g_->SteinerTree(pivot,terminals);
                st->traverse();
                std::cout << std::endl;
                auto Ts = separate(st,terminals,1);
                rowOp(A,Ts,1);
            }
            g_->deleteVertex(l2q.get_label(col));
            std::cout << A.get_data() <<std::endl;
            // After constructing the tree, remove 'col' from G'.
        }


        A.transpose();
        g_ = g->clone();
        for(int col = 0; col < n; ++col){
            if(!A.get_ele(col,col)){
                TransformMatrix(A,g_,col);
                std::cout<<col<<":\n"<< A.get_data() << std::endl;
            }
            // find terminals
            label pivot = l2q.get_label(col);
            std::cout <<"pivot: "<< pivot << std::endl;

            std::set<label> terminals;
            for(int row = col+1; row < n; ++row){
                if(A.get_ele(row,col)){
                    terminals.insert(l2q.get_label(row));
                    std::cout << l2q.get_label(row) <<" ";
                }
            }
            std::cout << std::endl;
            if(!terminals.empty()){
                auto st = g_->SteinerTree(pivot,terminals);
                st->traverse();
                std::cout << std::endl;
                auto Ts = separate(st,terminals,2);
                rowOp(A,Ts,2);
            }

            // Steiner Tree Construction: (currently commented out)
            // This section is intended to find a Steiner tree that connects the set of terminal nodes.
            // The tree minimizes the total length of the paths from each terminal to the root.
            // rowOp(A,terminals,St,2);
            std::vector<int> B(n);
            // for(int row = col+1; row < A.shape()[1]; ++row){
            //     if(terminals.find(col)!=terminals.end()){
            //         B.at(col) = col;
            //     }
            // }

            // for(path qubit in Ts) do something

            
            g_->deleteVertex(l2q.get_label(col));
            std::cout << A.get_data() <<std::endl;

        }

    }
}
#endif /* ALG_LINEAR_SYNTH */
