#include "alg/linear-tf.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"

using namespace lq;
int main(){
    xt::xarray<int> A = {{1, 1, 0, 1, 1, 0},
                         {0, 0, 1, 1, 0, 1},
                         {1, 0, 1, 0, 1, 0},
                         {1, 1, 0, 1, 0, 0},
                         {1, 1, 1, 1, 0, 0},
                         {0, 1, 0, 1, 0, 1},
                         {1, 1, 1, 0, 1, 0}};
    linearSynth(A);
    std::cout << std::endl;
    std::cout << A ;
}