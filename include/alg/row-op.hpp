#ifndef ROW_OP
#define ROW_OP

#include <vector>
#include <set>
#include "xtensor/xarray.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xbuilder.hpp" 
#include <iostream>
#include "seprate.hpp"

namespace lq {
    void rowOp(xt::xarray<int>& A, std::set<int>& terminals, Sttree* root, size_t alg) {
        auto T = seprate(root,root->data,terminals,alg);
        int t = T.size();

        for(int i = t-1; i>=0; --i){
            if(alg!=1){
                // Bottom-Up-1
                if(alg != 4){

                }   
            }
            // Top-Down-1
            if(alg != 4){

            }
            // Bootom-Up-2
            if(alg != 4){

            }
            if(alg!=1){
                // Top-Down-2
                if(alg != 4){

                }   
            }
            if(alg==4){

            }
        }

    }
}

#endif /* ROW_OP */
