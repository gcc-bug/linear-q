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
    class label2qubit {
        private:
            std::map<lq::label, int> LabelToQubit;
            std::map<int, lq::label> QubitToLabel;
            std::size_t size;
            inline bool isValuePresent(int qubit) const {
                return QubitToLabel.find(qubit) != QubitToLabel.end();
            }
            void insert(const lq::label& label, int qubit) {
                if (this->LabelToQubit.find(label) != this->LabelToQubit.end() || this->isValuePresent(qubit)) {
                    throw std::invalid_argument("Duplicate label or qubit");
                }

                this->LabelToQubit[label] = qubit;
                this->QubitToLabel[qubit] = label;
            }
        public:
            std::size_t inline get_size() const{
                return this->size;
            }
            
            label2qubit(const std::set<lq::label>& labels) {
                int qubit = 0;
                for (const auto& label : labels) {
                    insert(label, qubit++);
                }
                this->size = qubit;
            }

            label2qubit(const std::unordered_set<lq::label>& labels) {
                int qubit = 0;
                for (const auto& label : labels) {
                    insert(label, qubit++);
                }
                this->size = qubit;
            }

            void message() const{
                std::cout << "overall: " << this->size << "pairs" << std::endl;
                for (const auto& pair : this->LabelToQubit) {
                    std::cout << "label: " << pair.first << ", qubit: " << pair.second << std::endl;
                }
            }       
            
            

            label get_label(int qubit) const {
                auto it = this->QubitToLabel.find(qubit);
                if (it == this->QubitToLabel.end()) {
                    throw std::out_of_range("qubit not found: " + std::to_string(qubit));
                }
                return it->second;
            }

            int get_qubit(const lq::label& label) const {
                auto it = this->LabelToQubit.find(label);
                if (it == this->LabelToQubit.end()) {
                    throw std::out_of_range("label not found: " + std::to_string(label));
                }
                return it->second;
            }
    };

    class LFMatrix{
        private:
            xt::xarray<bool> A;
            label2qubit l2q;

            bool inline SizeEqual() const{
                return this->A.dimension() == 2 && A.shape()[0] == l2q.get_size();
            }

        public:

        LFMatrix(const xt::xarray<bool>& A_, const label2qubit& l2q_) : A(A_), l2q(l2q_) {}

        void transpose(){
            this->A = xt::transpose(this->A);
        }

        xt::xarray<bool> get_data() const {
            return this->A;
        }

        label2qubit get_l2q() const {
            return this->l2q;
        }

        bool inline get_ele(size_t i, size_t j) const{
            return this->A(i,j);
        }

        void mod2add(label i, label j){
            /*
            input should be label
            */
            if(not this->SizeEqual()){
                throw std::invalid_argument("Erreur");
            }
            int row_i = this->l2q.get_qubit(i);
            int row_j = this->l2q.get_qubit(j);
            xt::view(A, row_i, xt::all()) = xt::view(A, row_i, xt::all()) ^ xt::view(A, row_j, xt::all());
            return ;
        }

        void CNOT_(label control, label target){
            CNOT(control,target);
            this->mod2add(target,control);
        }

        bool isEye() const{
            if (this->A.dimension() != 2) {
                throw std::invalid_argument("Erreur");
            }
            size_t rows = this->A.shape()[0];
            size_t cols = this->A.shape()[1];
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    if (i != j && this->A(i, j)) {
                        return false;
                    }
                }
            }
            return true;
        }

        bool isGood() const{
            if (this->A.dimension() != 2) {
                throw std::invalid_argument("Erreur");
            }
            size_t rows = this->A.shape()[0];
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < i; ++j) {
                    if (this->A(i, j)) {
                        return false;
                    }
                }
            }
            return true;
        }
    };
}
#endif // TYPE_DEF