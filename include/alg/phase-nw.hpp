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
#include <stack>
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
            xt::xarray<bool> terms;
            std::vector<bool> flips;
            std::vector<Coeffs> coeffs;
            std::set<size_t> workTerms;
            LabelIndexBiMap biMap;

            bool inline isValid() const {
                auto dims = terms.shape();
                if (dims.size() != 2) return false; // Ensure there are exactly two dimensions

                size_t cols = dims[1]; 
                size_t rows = dims[0];

                return cols == flips.size() &&
                    cols == coeffs.size() &&
                    cols == workTerms.size() &&
                    rows == biMap.getSize();
            }
        public:
        ParityTerm(const xt::xarray<bool>& terms_, std::vector<bool> flip_, std::vector<int> cfs, const LabelIndexBiMap& biMap_): terms(terms_),flips(flip_), biMap(biMap_){
            for(const auto& c: cfs){
                if(isValidCoeffValue(c)) this->coeffs.push_back(static_cast<Coeffs>(c));
                else throw std::invalid_argument("Erruer");
            }
            for (int i = 0; i <= flip_.size(); ++i) {
                this->workTerms.insert(i);
            }
        }
        ParityTerm(const xt::xarray<bool>& terms_, std::vector<bool> flip_, std::vector<Coeffs> cfs, const LabelIndexBiMap& biMap_): terms(terms_),flips(flip_), biMap(biMap_),coeffs(cfs){
            for (int i = 0; i <= flip_.size(); ++i) {
                this->workTerms.insert(i);
            }
        }

        ParityTerm(const xt::xarray<bool>& terms_, const LabelIndexBiMap& biMap_) : terms(terms_), biMap(biMap_) {}

        bool at(size_t i,size_t pos) const{
            return this->terms(pos,i);
        }

        bool isUniform(const std::set<size_t>& compareTerms, const size_t pos) const {
            if (compareTerms.empty()) return true; // Handle empty set case

            auto firstTermIt = compareTerms.begin();
            bool temp = this->terms(pos,*firstTermIt);

            // Iterate over the set starting from the second element
            for (auto it = std::next(firstTermIt); it != compareTerms.end(); ++it) {
                if (this->terms(pos,*it) != temp) return false;
            }
            return true;
        }

        int countUniforms(const std::set<size_t>& compareTerms, const std::set<size_t>& poss) const{
            if (compareTerms.empty()) return 0;
            int res = 0;
            for (auto& pos : poss) {
                if (isUniform(compareTerms, pos)) ++res;
            }
            return res;
        }

        std::pair<std::set<size_t>, std::set<size_t>> splitByValue(const std::set<size_t>& compareTerms, size_t pos) const{
            std::set<size_t> P0, P1;
            for (const auto& c : compareTerms) {
                (this->terms(pos,c) ? P1 : P0).insert(c);
            }
            // std::cout << "P0:" << std::endl;
            // for(auto p: P0){
            //     std::cout << p << " ";
            // }
            // std::cout << std::endl;
            // std::cout << "P1:" << std::endl;
            // for(auto p: P1){
            //     std::cout << p << " ";
            // }
            // std::cout << std::endl;
            return {P0, P1};
        }

        std::pair<size_t, std::pair<std::set<size_t>,std::set<size_t>>> findOptimal(const std::set<size_t>& compareTerms, const std::set<size_t>& poss ) const{
            size_t max_pos = *poss.begin(); // Safe default if I is guaranteed to be non-empty.
            int max = -1; // Start with a value that any result will surpass.
            std::set<size_t> P0, P1;

            for (auto pos : poss) {
                // std::cout << pos << std::endl;
                auto [P0_, P1_] = splitByValue(compareTerms, pos);
                // std::cout << P0_.size() <<" " << P1_.size() << std::endl;
                int temp = countUniforms(P0_, poss) + countUniforms(P1_, poss);
                // std::cout << "cout: " << temp << std::endl;
                if (max < temp) {
                    max = temp;
                    max_pos = pos;
                    P0 = P0_;
                    P1 = P1_;
                }
            }
            return {max_pos,{P0,P1}};
        }
    };

    
    void phaseNW(std::vector<ParityTerm> B, Graph g){
        using StackType = std::tuple<std::vector<ParityTerm>, std::set<label>, label>;
        std::stack<StackType> myStack;
        // label 
    }
}

#endif /* ALG_PN */