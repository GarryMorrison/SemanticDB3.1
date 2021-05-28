//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_CONSTANTFLOAT_H
#define SDB3_1_CONSTANTFLOAT_H

#include <string>
#include "CompoundConstant.h"
#include "../Operator/SimpleOperator.h"
#include "../Function/misc.h"

class ConstantFloat : public CompoundConstant {
private:
    double value;
public:
    ConstantFloat(const double val) : value(val) { }
    const int type() const { return CFLOAT; }
    const double get_float() const {return value; }
    const int get_int() const { return static_cast<int>(value); }
    const SimpleOperator get_operator() const { return SimpleOperator(""); } // Maybe change later.
    const std::string get_string() const { return ""; }
    const std::string to_string() const { return float_to_int(value, default_decimal_places); }  // float_to_int() instead?

    void append(const std::shared_ptr<CompoundConstant> compound_constant) {
        value *= compound_constant->get_float();
    }
};


#endif //SDB3_1_CONSTANTFLOAT_H
