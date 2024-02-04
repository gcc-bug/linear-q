/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-02-01 10:57:26
 * @Description  : this file is about quantum gate definition
 * @TODO         : 
 */
#ifndef GATE
#define GATE

#include "Config.hpp"
inline std::string mk_idt(unsigned int indent) {
    return std::string(indent*4, ' ');
}
class CNOTGate {
private:
    label ctrl, targ;
public:
    CNOTGate(label ctrl, label targ) {
        this->ctrl = ctrl;
        this->targ = targ;
    }

    CNOTGate* clone() const {
        return new CNOTGate(*this);
    }

    std::ostream& assemble(std::ostream& out) const {
        char buf[100];
        snprintf(buf, 100, "cx q[%d], q[%d];\n", ctrl, targ);
        return out << buf;
    }

    std::ostream& print(std::ostream& out , unsigned int indent=0) const {
        char buf[100];
        snprintf(buf, 100, "CNOT[%d,%d]", ctrl, targ);
        return out << mk_idt(indent) << buf;
    }

    void reverse(){
        std::swap(this->ctrl,this->targ);
    }

    std::pair<label,label> getLabel() const{
        return {this->ctrl,this->targ};
    }
};

#endif /* GATE */