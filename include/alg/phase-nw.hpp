/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-02-04 20:41:42
 * @Description  : this file is about algorithm phase network synthesis
 * @TODO         : 
 */
#ifndef ALG_PN
#define ALG_PN
#include "typedef.hpp"

namespace lq{
    enum class coefficients {
        COEFF_0 = 0,
        COEFF_1 = 1,
        COEFF_2 = 2,
        COEFF_3 = 3,
        COEFF_4 = 4,
        COEFF_5 = 5,
        COEFF_6 = 6,
        COEFF_7 = 7,
    };
    class phase{
        private:
        std::vector<bool> data;
        bool flip;
        int c;

        public:
        phase(const std::vector<bool>& data_,bool flip_,int c_): data(data_),flip(flip_),c(c_){}

        bool at(size_t i) const{
            return this->data[i];
        }
        
    };
    
    bool same(std::vector<phase> B,size_t i){
        bool temp = B[0].at(i);
        for(int i = 1; i < B.size(); i++){
            if(B[i].at(i) != temp) return false;
        }
        return true;
    }
    int get_same(std::vector<phase> B,std::set<size_t> I){
        int res = 0;
        for(auto& i: I){
            if(same(B,i)) res++;
        }
        return res;
    }
    std::pair<std::vector<phase>,std::vector<phase>> spB(std::vector<phase> B, size_t j){
        std::vector<phase> B0, B1;
        for(auto& b: B){
            if(b.at(j)){
                B1.push_back(b);
            } 
            else{
                B0.push_back(b);
            }
        }
        return {B0,B1};
    }
    size_t choose_j(std::vector<phase> B,std::set<size_t> I){
        size_t max_j = 0;
        int max = 0;

        for(auto& i:I){
            std::cout << i << std::endl;
            auto [B0,B1] = spB(B,i);
            std::cout << B0.size() << " " << B1.size() << std::endl;
            auto temp = get_same(B0,I)+get_same(B1,I);
            std::cout << temp << std::endl;
            if(max < temp) {
                max = temp;
                max_j = i;
            }
        }
        return max_j;

    }
    void phaseNW(std::vector<phase> B, Graph g){

    }
}

#endif /* ALG_PN */