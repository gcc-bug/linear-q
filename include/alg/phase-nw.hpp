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
#include "seprate.hpp"
#include "row-op.hpp"
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
            xt::xarray<bool> termsData;
            std::vector<bool> flips;
            std::vector<Coeffs> coeffs;
            std::set<size_t> workTerms;
            LabelIndexBiMap biMap;

            bool inline isValid() const {
                auto dims = termsData.shape();
                if (dims.size() != 2) return false; // Ensure there are exactly two dimensions

                size_t termsNumber = dims[1]; 
                size_t qubitNumber = dims[0];

                return termsNumber == flips.size() &&
                    termsNumber == coeffs.size() &&
                    termsNumber > workTerms.size() &&
                    qubitNumber == biMap.getSize();
            }
        public:
        ParityTerm(const xt::xarray<bool>& termsData_, std::vector<bool> flip_, std::vector<int> cfs, const LabelIndexBiMap& biMap_): termsData(termsData_),flips(flip_), biMap(biMap_){
            for(const auto& c: cfs){
                if(isValidCoeffValue(c)) this->coeffs.push_back(static_cast<Coeffs>(c));
                else throw std::invalid_argument("Erruer");
            }
            for (int term = 0; term < flip_.size(); ++term) {
                this->workTerms.insert(term);
            }
        }
        ParityTerm(const xt::xarray<bool>& termsData_, std::vector<bool> flip_, std::vector<Coeffs> cfs, const LabelIndexBiMap& biMap_): termsData(termsData_),flips(flip_), biMap(biMap_),coeffs(cfs){
            for (int term = 0; term <= flip_.size(); ++term) {
                this->workTerms.insert(term);
            }
        }

        ParityTerm(const xt::xarray<bool>& termsData_, const LabelIndexBiMap& biMap_) : termsData(termsData_), biMap(biMap_) {}

        std::set<size_t> getAllTerms() const{
            return this->workTerms;
        }

        std::set<size_t> getAllIndexs() const{
            std::set<size_t> indexs;
            for(int i = 0; i < this->biMap.getSize(); i++){
                indexs.insert(i);
            }
            return indexs;
        }

        inline bool onWork(const std::set<size_t>& comparedTerms) const{
            return std::includes(this->workTerms.begin(), this->workTerms.end(), comparedTerms.begin(), comparedTerms.end());
        }

        inline bool at(size_t term,size_t pos) const{
            return this->termsData(pos,term);
        }

        bool isUniform(const std::set<size_t>& compareTerms, const size_t pos) const {
            if (compareTerms.empty()) return true; // Handle empty set case

            auto firstTermIt = compareTerms.begin();
            bool temp = this->termsData(pos,*firstTermIt);

            // Iterate over the set starting from the second element
            for (auto it = std::next(firstTermIt); it != compareTerms.end(); ++it) {
                if (this->termsData(pos,*it) != temp) return false;
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
            for (const auto& term : compareTerms) {
                (this->termsData(pos,term) ? P1 : P0).insert(term);
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
            if(!onWork(compareTerms)) throw std::runtime_error("erruer");
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

        bool finish(size_t term) const{
            int count = 0; 
            for(size_t pos = 0; pos < this->termsData.shape()[0]; ++pos){
                if(this->termsData(pos, term)){
                    ++count;
                    if(count > 1) return false; // Return early if more than one truthy value is found.
                }
            }
            return count == 1; 
        }

        std::vector<size_t> findFinish() const {
            std::vector<size_t> finishTerms;
            for(auto& term: this->workTerms){
                if(finish(term)){
                    finishTerms.push_back(term);
                }
            }
            return finishTerms;
        }

        std::set<size_t> findTrue(const size_t term) const{
            std::set<size_t> res;
            for(size_t pos = 0; pos < this->termsData.shape()[0]; ++pos){
                if(this->termsData(pos, term)){
                    res.insert(pos);
                }
            }
            return res;
        }

        /**
         * @brief: find the index where terms[index] == true, term in compare terms
         * @param: a std::set<size_t> compare terms contain all term search
         * @return {std::set<size_t>} 
         */
        std::set<size_t> findIndexs(const std::set<size_t>& compareTerms) const{
            if(!onWork(compareTerms)) throw std::runtime_error("erruer");

            std::set<size_t> terminals;
            for(const auto& term: compareTerms){
                auto temp = findTrue(term);
                terminals.insert(temp.begin(),temp.end());
            }
            return terminals;
        }

        std::set<label> getLabel(std::set<size_t>& Indexs) const{
            std::set<label> res;
            for(auto const& index: Indexs){
                res.insert(this->biMap.getLabel(index));
            }
            return res;
        }
        inline label getLabel(const size_t& Index) const{
            return this->biMap.getLabel(Index);
        }

        inline LFMatrix getLFmatrix() const{
            return LFMatrix(this->termsData,this->biMap);
        }

        void modify(const xt::xarray<bool>& newData){
            this->termsData = newData;
        }

        void erase(size_t term){
            this->workTerms.erase(term);
        }
    };

    
    std::vector<CNOTGate> phaseNW(ParityTerm& pt, Graph& g){
        std::vector<CNOTGate> res;
        using StackType = std::tuple<std::set<size_t>, std::set<size_t>, size_t>;
        std::stack<StackType> myStack;
        // label
        std::set<size_t> workTerms = pt.getAllTerms();
        std::set<size_t> livedIndexs = pt.getAllIndexs();
        size_t inValid = *workTerms.rbegin() + 1;
        size_t pos;

        AlgSignal alg = AlgSignal::phase;
        myStack.push({workTerms,livedIndexs,inValid});

        while(!myStack.empty()){
            auto [workTerms,livedIndexs,pos] = myStack.top();
            myStack.pop();
            std::cout << "-------------\n";
            std::cout << "pos: " <<pos << std::endl;

            if(pos != inValid){
                auto indexs = pt.findIndexs(workTerms);
                indexs.erase(pos);
                if(!indexs.empty()){
                    auto terminals = pt.getLabel(indexs);
                    label pivot = pt.getLabel(pos);
                    std::cout << "pivot: " << pivot << std::endl;
                    std::cout << "terminals:" << std::endl;
                    for(auto p: terminals){
                        std::cout << p << " ";
                    }
                    std::cout << std::endl;
                    auto st = g.SteinerTree(pivot,terminals);
                    st->traverse();
                    auto Ts = separate(st,terminals,alg);
                    auto tf = pt.getLFmatrix();
                    auto gates = rowOp(tf,Ts,alg);
                    res.insert(res.end(),gates.begin(),gates.end());
                    pt.modify(tf.getData());
                    for(auto& gate: gates){
                        gate.assemble(std::cout);
                    }
                    auto finishTerms = pt.findFinish();
                    if(!finishTerms.empty()){
                        for(const auto& term: finishTerms){
                            std::cout << term << " finish" << std::endl;
                            // TODO: add gate here
                            pt.erase(term);
                            workTerms.erase(term);
                        }
                    }
                }

            }

            auto [nex_pos, sep] = pt.findOptimal(workTerms,livedIndexs);
            std::cout << "work terms:" << std::endl;
            for(auto p: workTerms){
                std::cout << p << " ";
            }
            std::cout << std::endl;
            std::cout << "lived terms:" << std::endl;
            for(auto p: livedIndexs){
                std::cout << p << " ";
            }
            std::cout << std::endl;
            std::cout << "use pos: " << nex_pos << std::endl;
            auto [P0,P1] = sep;
            std::cout << "P0:" << std::endl;
            for(auto p: P0){
                std::cout << p << " ";
            }
            std::cout << std::endl;
            std::cout << "P1:" << std::endl;
            for(auto p: P1){
                std::cout << p << " ";
            }
            std::cout << std::endl;

            livedIndexs.erase(nex_pos);
            if(!P1.empty()){
                if(pos==inValid){
                    myStack.push({P1,livedIndexs,nex_pos});
                }
                else{
                    myStack.push({P1,livedIndexs,pos});
                }
            }
            if(!P0.empty()){
                myStack.push({P0,livedIndexs,pos});
            }

        }
    }
}

#endif /* ALG_PN */