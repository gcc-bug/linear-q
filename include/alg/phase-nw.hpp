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
    enum class Coeffs {
        COEFF_0 = 0,
        COEFF_1 = 1,
        COEFF_2 = 2,
        COEFF_3 = 3,
        COEFF_4 = 4,
        COEFF_5 = 5,
        COEFF_6 = 6,
        COEFF_7 = 7,
    };
    bool isValidCoeffValue(int value) {
        return value >= static_cast<int>(Coeffs::COEFF_0) && value <= static_cast<int>(Coeffs::COEFF_7);
    }
    class ParityTerm{
        private:
        std::vector<bool> terms;
        bool flip;
        Coeffs c;

        public:
        ParityTerm(const std::vector<bool>& terms_,bool flip_,int c_): terms(terms_),flip(flip_){
            if(isValidCoeffValue(c_)) this->c = static_cast<Coeffs>(c_);
            else throw std::invalid_argument("Erruer");
        }

        bool at(size_t pos) const{
            return this->terms[pos];
        }
        
    };
    
    bool isUniform(const std::vector<ParityTerm>& terms, size_t pos) {
        if (terms.empty()) return true;
        bool temp = terms.front().at(pos);
        for (size_t j = 1; j < terms.size(); ++j) {
            if (terms[j].at(pos) != temp) return false;
        }
        return true;
    }

    int countUniforms(const std::vector<ParityTerm>& B, const std::set<size_t>& poss) {
        if (B.empty()) return 0;
        int res = 0;
        for (auto& pos : poss) {
            if (isUniform(B, pos)) ++res;
        }
        return res;
    }

    std::pair<std::vector<ParityTerm>, std::vector<ParityTerm>> splitByValue(const std::vector<ParityTerm>& terms, size_t pos) {
        std::vector<ParityTerm> P0, P1;
        for (const auto& b : terms) {
            (b.at(pos) ? P1 : P0).push_back(b);
        }
        return {P0, P1};
    }

    std::pair<size_t, std::pair<std::vector<ParityTerm>,std::vector<ParityTerm>>> findOptimal(const std::vector<ParityTerm>& terms, const std::set<size_t>& poss) {
        size_t max_pos = *poss.begin(); // Safe default if I is guaranteed to be non-empty.
        int max = -1; // Start with a value that any result will surpass.
        std::vector<ParityTerm> P0, P1;

        for (auto pos : poss) {
            std::cout << pos << std::endl;
            auto [P0_, P1_] = splitByValue(terms, pos);
            std::cout << P0_.size() <<" " << P1_.size() << std::endl;
            int temp = countUniforms(P0_, poss) + countUniforms(P1_, poss);
            std::cout << "cout: " << temp << std::endl;
            if (max < temp) {
                max = temp;
                max_pos = pos;
                P0 = P0_;
                P1 = P1_;
            }
        }
        return {max_pos,{P0,P1}};
    }
    void phaseNW(std::vector<ParityTerm> B, Graph g){

    }
}

#endif /* ALG_PN */