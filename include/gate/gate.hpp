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
class Gate {
protected:
    int nr_param {-1};

public:
    Gate(int nr_param) : nr_param(nr_param) {}
    int getParam(){ return this->nr_param;}
    virtual ~Gate() {}
    virtual Gate* clone() const = 0;

};

inline std::string mk_idt(unsigned int indent) {
    return std::string(indent*4, ' ');
}
class CNOTGate: public Gate  {
private:
    label ctrl, targ;
public:
    CNOTGate(label ctrl, label targ) : Gate(2){
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

class XGate: public Gate{
    private:
    label pos;
public:
    XGate(label pos) : Gate(1){
        this->pos = pos;
    }

    XGate* clone() const {
        return new XGate(*this);
    }

    std::ostream& assemble(std::ostream& out) const {
        char buf[100];
        snprintf(buf, 100, "x q[%d];\n", this->pos);
        return out << buf;
    }

    std::ostream& print(std::ostream& out , unsigned int indent=0) const {
        char buf[100];
        snprintf(buf, 100, "X[%d]", this->pos);
        return out << mk_idt(indent) << buf;
    }
};

class ZGate: public Gate{
    private:
    label pos;
public:
    ZGate(label pos) : Gate(1){
        this->pos = pos;
    }

    ZGate* clone() const {
        return new ZGate(*this);
    }

    std::ostream& assemble(std::ostream& out) const {
        char buf[100];
        snprintf(buf, 100, "z q[%d];\n", this->pos);
        return out << buf;
    }

    std::ostream& print(std::ostream& out , unsigned int indent=0) const {
        char buf[100];
        snprintf(buf, 100, "Z[%d]", this->pos);
        return out << mk_idt(indent) << buf;
    }
};

#endif /* GATE */