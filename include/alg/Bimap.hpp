#ifndef BIMAP_HPP
#define BIMAP_HPP

#include <map>
#include <set>
#include <unordered_set>
#include <stdexcept>
#include "../Config.hpp"
class label2qubit {
    private:
        std::map<lq::label, int> LabelToQubit;
        std::map<int, lq::label> QubitToLabel;
        inline bool isValuePresent(int qubit) const {
            return QubitToLabel.find(qubit) != QubitToLabel.end();
        }
    public:
        
        label2qubit(const std::set<lq::label>& labels) {
            int qubit = 0;
            for (const auto& label : labels) {
                insert(label, qubit++);
            }
        }

        label2qubit(const std::unordered_set<lq::label>& labels) {
            int qubit = 0;
            for (const auto& label : labels) {
                insert(label, qubit++);
            }
        }

        void message(){
            for (const auto& pair : LabelToQubit) {
                std::cout << "label: " << pair.first << ", qubit: " << pair.second << std::endl;
            }
        }       
        
        void insert(const lq::label& label, int qubit) {
            if (LabelToQubit.find(label) != LabelToQubit.end() || isValuePresent(qubit)) {
                throw std::invalid_argument("Duplicate label or qubit");
            }

            LabelToQubit[label] = qubit;
            QubitToLabel[qubit] = label;
        }

        lq::label getlabel(int qubit) const {
            auto it = QubitToLabel.find(qubit);
            if (it == QubitToLabel.end()) {
                throw std::out_of_range("qubit not found");
            }
            return it->second;
        }

        int getqubit(const lq::label& label) const {
            auto it = LabelToQubit.find(label);
            if (it == LabelToQubit.end()) {
                throw std::out_of_range("label not found");
            }
            return it->second;
        }
};

#endif // BIMAP_HPP