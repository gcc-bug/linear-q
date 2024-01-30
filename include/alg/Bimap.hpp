#ifndef DATA_ST
#define DATA_ST

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

}
#endif // DATA_ST