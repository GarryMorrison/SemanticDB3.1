//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_CONSTANTINTEGER_H
#define SDB3_1_CONSTANTINTEGER_H

#include <string>
#include "CompoundConstant.h"
#include "ConstantFloat.h"
#include "../Operator/SimpleOperator.h"

class ConstantInteger : public CompoundConstant {
private:
    int value;
public:
    // ConstantInteger(const ulong val) {value = val; }
    ConstantInteger(const int val) : value(val) { }
    const int type() const { return CINT; }
    const double get_float() const { return static_cast<double>(value); }
    const int get_int() const { return value; }
    const SimpleOperator get_operator() const { return SimpleOperator(""); } // Maybe change later.
    const std::string get_string() const { return ""; }
    const std::string to_string() const { return std::to_string(value); }

    void append(const std::shared_ptr<CompoundConstant> compound_constant) {
        value *= compound_constant->get_int();
    }
};


#endif //SDB3_1_CONSTANTINTEGER_H
