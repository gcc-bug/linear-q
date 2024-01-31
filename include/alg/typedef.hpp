/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-01-28 16:09:37
 * @Description  : this  file is about class or struct will be used in algorithms
 * @TODO         : modify class and function name to improve readability
 */
#ifndef TYPE_DEF
#define TYPE_DEF

#include <map>
#include <set>
#include <unordered_set>
#include <stdexcept>
#include "xtensor/xarray.hpp"
#include "xtensor/xview.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xbuilder.hpp" 
#include "../Config.hpp"
namespace lq{
    enum class AlgSignal{
        diag, // alg<- 1, Top Down 1, Bottom up 2
        propagated, // alg<-2,3, Bottom Up 1, Top Down 1, Bottom up 2, Top-Down-2
        phase //alg<-4, Bottom Up 1, Top Down 1, Bottom up 2, Top-Down-2
    };
    void CNOT(label control, label target) {
            std::cout << "CNOT " << control <<" " << target << std::endl;
    }
    struct SubTree {
        Sttree* root;
        std::set<label> leaves;
    };
    class LabelIndexBiMap {
        private:
            std::map<lq::label, int> LabelToIndex;
            std::map<int, lq::label> IndexToLabel;
            std::size_t size;
            inline bool isValuePresent(int Index) const {
                return IndexToLabel.find(Index) != IndexToLabel.end();
            }
            inline void insert(const lq::label& label, int Index) {
                if (this->LabelToIndex.find(label) != this->LabelToIndex.end() || this->isValuePresent(Index)) {
                    throw std::invalid_argument("Duplicate label or Index");
                }

                this->LabelToIndex[label] = Index;
                this->IndexToLabel[Index] = label;
            }
        public:
            std::size_t inline getSize() const{
                return this->size;
            }
            
            LabelIndexBiMap(const std::set<lq::label>& labels) {
                int Index = 0;
                for (const auto& label : labels) {
                    insert(label, Index++);
                }
                this->size = Index;
            }

            LabelIndexBiMap(const std::unordered_set<lq::label>& labels) {
                int Index = 0;
                for (const auto& label : labels) {
                    insert(label, Index++);
                }
                this->size = Index;
            }

            void message() const{
                std::cout << "overall: " << this->size << " pairs" << std::endl;
                for (const auto& pair : this->LabelToIndex) {
                    std::cout << "label: " << pair.first << ", Index: " << pair.second << std::endl;
                }
            }       
            
            label getLabel(int Index) const {
                auto it = this->IndexToLabel.find(Index);
                if (it == this->IndexToLabel.end()) {
                    throw std::out_of_range("Index not found: " + std::to_string(Index));
                }
                return it->second;
            }

            int getIndex(const lq::label& label) const {
                auto it = this->LabelToIndex.find(label);
                if (it == this->LabelToIndex.end()) {
                    throw std::out_of_range("label not found: " + std::to_string(label));
                }
                return it->second;
            }
    };

    class LFMatrix{
        private:
            xt::xarray<bool> data;
            LabelIndexBiMap biMap;

            bool inline SizeEqual() const{
                return this->data.dimension() == 2 && data.shape()[0] == biMap.getSize();
            }

        public:

        LFMatrix(const xt::xarray<bool>& data_, const LabelIndexBiMap& biMap_) : data(data_), biMap(biMap_) {}

        void transpose(){
            this->data = xt::transpose(this->data);
        }

        xt::xarray<bool> getData() const {
            return this->data;
        }

        LabelIndexBiMap getBiMap() const {
            return this->biMap;
        }

        bool inline at(size_t i, size_t j) const{
            return this->data(i,j);
        }

        void mod2add(label i, label j){
            /*
            input should be label
            */
            if(not this->SizeEqual()){
                throw std::invalid_argument("Erreur");
            }
            int row_i = this->biMap.getIndex(i);
            int row_j = this->biMap.getIndex(j);
            xt::view(data, row_i, xt::all()) = xt::view(data, row_i, xt::all()) ^ xt::view(data, row_j, xt::all());
            return ;
        }

        void CNOT_(label control, label target){
            CNOT(control,target);
            this->mod2add(target,control);
        }

        bool isEye() const{
            if (this->data.dimension() != 2) {
                throw std::invalid_argument("Erreur");
            }
            size_t rows = this->data.shape()[0];
            size_t cols = this->data.shape()[1];
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    if (i != j && this->data(i, j)) {
                        return false;
                    }
                }
            }
            return true;
        }

        bool isGood() const{
            if (this->data.dimension() != 2) {
                throw std::invalid_argument("Erreur");
            }
            size_t rows = this->data.shape()[0];
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < i; ++j) {
                    if (this->data(i, j)) {
                        return false;
                    }
                }
            }
            return true;
        }
    };
}
#endif // TYPE_DEF